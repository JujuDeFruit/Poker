#include "constantes_files_name.h"
#include "menu_definitions.h"
#include "odrive.h"
#include <iostream>
#include <fstream>

using namespace std;

//======================================= Menu1 =========================================/
#pragma region MenuPokerStart

/*
 * MenuPokerStart Constructor.
 */
MenuPokerStart::MenuPokerStart(Game* game) : Menu("New round :") {
	game_ = game;
	AddOption("help", "Show help");
	AddOption("start", "Start the new Round.");
}

/*
 * ExecuteOption overload.
 */
void MenuPokerStart::ExecuteOption(const string &name, bool &end)
{
	end = false;
	//menupokerstart options
	if (name == "start") {
		end = true;
		new Round(game_->GetPlayer(), game_->GetCurrentRoundId());
	}
	else if (name == "help") Help();
	else {
		cout << "Option not defined" << endl;
		system("pause");
	}
}

void MenuPokerStart::Execute()
{
	bool end = false;
	while (!end) {
		if (!game_->GetPlayer()->GetAllMoneys() || game_->GetPlayer()->GetAllMoneys() == 2 * game_->GetPlayer()->GetInitialMoney()) break;
		system("cls");
		ShowMenu();
		int choice = AskChoix();
		if (choice >= 0 && choice < listeOptions_.size())
			ExecuteOption(listeOptions_[choice].GetNom(), end);
		else {
			cout << "Wrong choice" << endl;
			system("pause");
		}
	}
}

/*
 * ShowMenu overload.
 */
void MenuPokerStart::ShowMenu() {
	Menu::ShowMenu();
	game_->GetPlayer()->PrintMoneyAndTokens();
}

#pragma endregion

//======================================= Menu2 =========================================/
#pragma region MenuPokerGame

/*
 * MenuPokerGame Constructor.
 */
MenuPokerGame::MenuPokerGame(Round* round) : Menu("Round :") {
	round_ = round;
	gameIsCreate_ = true;

	AddOption("bet", "Bet a sum");
	AddOption("follow", "Follow your oppenant");
	AddOption("all in", "Bet everything, all in !");
	AddOption("check", "Check 0$");
	AddOption("fold", "Fold your cards");
	//AddOption("leave", "Leave the game");
}

/*
 * ExecuteOption overload.
 */
void MenuPokerGame::ExecuteOption(const string &name, bool &end)
{
	end = false;
	//menupokergame options
	if (name == "bet") round_->Bet();
	else if (name == "follow") end = round_->Follow();
	else if (name == "all in") round_->All_In();
	else if (name == "check") end = round_->Check();
	else if (name == "fold") end = round_->Fold();
	//else if (name == "leave") end = Leave();
	else {
		cout << "Option not defined" << endl;
		system("pause");
	}
}

/*
 * ShowMenu overload.
 */
void MenuPokerGame::ShowMenu(string action) {
	cout << "River" << endl;
	round_->GetRiver().PrintDeck();
	cout << endl;
	cout << "The current pot : " << round_->GetPot() << " $" << endl;
	Menu::ShowMenu();
	if (gameIsCreate_ == true) {
		round_->GetPlayer()->PrintMoneyAndTokens();
		cout << "Money played by the opponent : " << round_->GetMoneyPlayedOpponent() << " $" << endl;
		cout << "Money you played : " << round_->GetMoneyPlayedByYou() << " $" << endl;
	}
	cout << "Your hand" << endl;
	round_->GetPlayer()->GetHand().PrintDeck();
	cout << endl;
	if (action != "") cout << "Your opponent used " << action << endl;
}

/*
 * Execute overload.
 */
void MenuPokerGame::Execute()
{
	bool end = false;
	while (!end) {
		system("cls");
		
		end = round_->GetInfoFromOpponent(this);

		if (!end) {
			int choice = AskChoix();
			if (choice >= 0 && choice < listeOptions_.size()) {
				ExecuteOption(listeOptions_[choice].GetNom(), end);
			}
			else {
				cout << "Wrong choice" << endl;
				system("pause");
			}
		}
		
	}
}

#pragma endregion