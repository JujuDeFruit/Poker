#include "menu_definitions.h"
#include "odrive.h"
#include <iostream>
#include <fstream>

using namespace std;

//======================================= Menu1 =========================================/
#pragma region MenuPokerGame

MenuPokerStart::MenuPokerStart(Game* game) : Menu("New round :") {
	game_ = game;
	AddOption("help", "Show help");
	AddOption("start", "Start the new Round.");
}

void MenuPokerStart::ExecuteOption(const string &name, bool &end)
{
	end = false;
	//menupokerstart options
	if (name == "start") {
		end = true;
		/*game_->SetRound(*/new Round(game_->GetPlayer(), game_->GetCurrentRoundId());
	}
	else if (name == "help") Help();
	else {
		cout << "Option not defined" << endl;
		system("pause");
	}
}

#pragma endregion

//======================================= Menu2 =========================================/
#pragma region MenuPokerGame

MenuPokerGame::MenuPokerGame(Round* round, Deck river, Deck hand) : Menu("Round :") {
	round_ = round;
	gameIsCreate_ = true;

	river_ = river;
	hand_ = hand;

	AddOption("bet", "Bet a sum");
	AddOption("follow", "Follow your oppenant");
	AddOption("all in", "Bet everything, all in !");
	AddOption("check", "Check 0$");
	AddOption("fold", "Fold your cards");
	AddOption("leave", "Leave the game");
}

/*
 * ExecuteOption overload.
 */
void MenuPokerGame::ExecuteOption(const string &name, bool &end)
{
	end = false;
	//menupokergame options
	if (name == "bet") round_->Bet();
	else if (name == "follow") round_->Follow();
	else if (name == "all in") round_->All_In();
	else if (name == "check");
	else if (name == "fold");
	else if (name == "leave") end = Leave();
	else {
		cout << "Option not defined" << endl;
		system("pause");
	}
}

/*
 * howMenu overload.
 */
void MenuPokerGame::ShowMenu() {
	cout << "River" << endl;
	river_.PrintDeck();
	cout << endl;
	Menu::ShowMenu();
	if (gameIsCreate_ == true) {
		round_->GetPlayer()->PrintMoneyAndTokens();
		cout << "Money plays by the opponent : " << round_->GetMoneyPlayedOpponent() << "$" << endl;
		cout << "Money you played : " << round_->GetMoneyPlayedByYou() << "$" << endl;
	}
	cout << "Your hand" << endl;
	hand_.PrintDeck();
	cout << endl;
}

/*
 * Execute overload.
 */
void MenuPokerGame::Execute()
{
	bool end = false;
	while (!end) {
		system("cls");
		ShowMenu();
		if (!round_->GetYourTurn()) {
			ODrive od;
			od.waitForChange("Turn.txt");
			round_->SetYourTurn(true);
		}
		int choice = AskChoix();
		if (choice >= 0 && choice < listeOptions_.size()) {
			ExecuteOption(listeOptions_[choice].GetNom(), end);
			round_->ChangeTurn();
		}
		else {
			cout << "Wrong choice" << endl;
			system("pause");
		}
	}
}

#pragma endregion