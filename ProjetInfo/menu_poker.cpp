#include "constantes_files_name.h"
#include "menu_definitions.h"
#include "odrive.h"
#include <iostream>
#include <fstream>

using namespace std;

#pragma region MenuPokerStart

/*
 * MenuPokerStart Constructor.
 */
MenuPokerStart::MenuPokerStart(Game* game) : Menu("New round :") {
	game_ = game;
	AddOption("help", "Show help");				// Show help.
	AddOption("start", "Start the new Round.");	// Start the game.
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
		new Round(game_->GetPlayer(), game_->GetCurrentRoundId());	// Start a new round with the current round Id, that increase by  each round.
	}
	else if (name == "help") Help();
	else {
		cout << "Option not defined" << endl;
		system("pause");
	}
}

/*
 * Execute the game menu.
 */
void MenuPokerStart::Execute()
{
	bool end = false;
	while (!end) {
		/* Check if the player has not money anymore or if the player has all money enable. If it is, then break the current game. */
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
	Menu::ShowMenu();							// Print the menu
	game_->GetPlayer()->PrintMoneyAndTokens();	// Print money.
}

#pragma endregion

#pragma region MenuPokerGame

/*
 * MenuPokerGame Constructor.
 */
MenuPokerGame::MenuPokerGame(Round* round) : Menu("Round :") {	// Printed  menu in round.
	round_ = round;
	gameIsCreate_ = true;

	AddOption("bet", "Bet a sum");
	AddOption("follow", "Follow your oppenant");
	AddOption("all in", "Bet everything, all in !");
	AddOption("check", "Check 0$");
	AddOption("fold", "Fold your cards");
	AddOption("hiddenOption", "");
}

/*
 * ExecuteOption overload.
 */
void MenuPokerGame::ExecuteOption(const string &name, bool &end)
{
	end = false;
	//menupokergame options
	if (name == "bet") round_->Bet();						// Bet a sum.
	else if (name == "follow") end = round_->Follow();		// Follow your opponent.
	else if (name == "all in") round_->All_In();			// Make a all in.
	else if (name == "check") end = round_->Check();		// Check 
	else if (name == "fold") end = round_->Fold();			// Fold.
	else if (name == "hiddenOption") round_->PrintDeck();	// Hidden option for demo.
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
	round_->GetRiver().PrintDeck();			// Print the river (0, 3, 4 or 5 cards).
	cout << endl;
	cout << "The current pot : " << round_->GetPot() << " $" << endl;	// Print ammount of the pot.
	Menu::ShowMenu();
	if (gameIsCreate_ == true) {
		round_->GetPlayer()->PrintMoneyAndTokens();														// Print your tokens.
		cout << "Money played by the opponent : " << round_->GetMoneyPlayedOpponent() << " $" << endl;	// Print money in game (opponent).
		cout << "Money you played : " << round_->GetMoneyPlayedByYou() << " $" << endl;					// Print money in game (you).
	}
	cout << "Your hand" << endl;
	round_->GetPlayer()->GetHand().PrintDeck();		// Print player's hand.
	cout << endl;
	if (action != "") cout << "Your opponent used " << action << endl;			// Action of your opponent (Fold, bet, follow, ...).
}

/*
 * Execute overload.
 */
void MenuPokerGame::Execute()
{
	bool end = false;
	while (!end) {
		system("cls");
		
		end = round_->GetInfoFromOpponent(this);	// Get all info coming back from you opponent (river, pot, is there a winner ? ...).

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
	ODrive od;
	od.clearFiles(round_->GetPlayer()->GetServer());	// Reset all the files execpt the __init__.txt one.;
}

#pragma endregion