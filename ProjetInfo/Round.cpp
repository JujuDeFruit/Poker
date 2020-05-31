#include "constantes_files_name.h"
#include "menu_definitions.h"
#include "game_definitions.h"
#include "card_definitions.h"
#include "odrive.h"
#include <fstream>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>

using namespace std;

#pragma region Constructor

Round::Round(Player* player, int id) {
	player_ = player;
	roundId_ = id;

	// To know who played first.
	if (player_->GetServer()) {
		if (id % 2 == 0) {
			yourTurn_ = false;
		}
		else {
			yourTurn_ = true;
		}
	}
	else {
		if (id % 2 == 0) {
			yourTurn_ = true;
		}
		else {
			yourTurn_ = false;
		}
	}


	InitialiseRound();

}

#pragma endregion

/*
 * Initialise the round, create the deck and draw hands.
 */
void Round::InitialiseRound() {
	ODrive od;
	if (player_->GetServer()) {			// If the player is the master, then it manage the deck. 
		BeginDeck beginDeck(true);		// Initialise the deck.
		beginDeck.ShakeDeck();
		beginDeck_ = beginDeck;
		DrawHand();						// Set client and server hands.
		vector<string> deck = Deck::SerializeCards(beginDeck_);
		od.writeInFile(ConstFiles::DECKFILESERVER, deck);
	}
	else {
		vector<string> callback = od.readFile(ConstFiles::HANDFILECLIENT);		// Get client's hand, set by the server application.
		if (!callback.size()) od.waitForChange(ConstFiles::HANDFILECLIENT);
		callback = od.readFile(ConstFiles::HANDFILECLIENT);
		Deck* hand = new Deck(Deck::DeserializeCards(callback));				// Affect the hand deck to the local variable.
		player_->SetHand(hand);

		vector<string> cards = od.readFile(ConstFiles::DECKFILESERVER);			// Get the deck from server.
		if (!cards.size()) od.waitForChange(ConstFiles::DECKFILESERVER);
		cards = od.readFile(ConstFiles::DECKFILESERVER);
		beginDeck_ = BeginDeck::ToBeginDeck(Deck::DeserializeCards(cards));		// Set the begin Deck.

		od.writeInFile(ConstFiles::DECKFILECLIENT, cards);

	}

	Start();			// Start the gaem.
}

/*
 * Start the round.
 */
void Round::Start() {
	MenuPokerGame mgame(this);
	mgame.Execute();
}

/*
 * Change the turn, write it in the appropriate file.
 */
void Round::ChangeTurn() {
	ODrive od;
	string message = player_->GetServer() ? "0" : "1"; // Write '0' if server just played, else write '1'.
	const string turnFile = player_->GetServer() ? ConstFiles::TURNFILESERVER : ConstFiles::TURNFILECLIENT;		// Write the message for the player to play in the appropriate file.
	od.writeInFile(turnFile, message, ofstream::out);
	yourTurn_ = false;
}

#pragma region Options

/*
 * Follow your opponent. Bet the same ammount. Return a bool false to stop the round if you follow all-in.
 */
bool Round::Follow() {
	if (!yourTurn_) return false;

	unsigned int nbTokens, token100 = 0, token50 = 0, token25 = 0, token5 = 0, token1 = 0, yourBet = 0;

	bool allIn = false;

	/* Try if your opponent did not make a all in. Your opponent cannot follow if he does not have enought money. */
	if (opponentAction_ != "All_In") {
		if (player_->GetAllMoneys() < moneyPlayedOpponent_ || moneyPlayedOpponent_ == 0) {
			cout << endl << "You can't follow" << endl;
			system("pause");
			return false;
		}
	}
	else {			// You follow all-in. Bet everything.
		allIn = true;
		token1 = player_->GetTokens()[0];
		token5 = player_->GetTokens()[1];
		token25 = player_->GetTokens()[2];
		token50 = player_->GetTokens()[3];
		token100 = player_->GetTokens()[4];
		yourBet = token1 * 1 + token5 * 5 + token25 * 25 + token50 * 50 + token100 * 100;
	}

	bool condition = false;
	const string text = GetMoneyPlayedByYou() == 0 ? "Choose the tokens : " : "Choose the tokens to add to your previous bet : ";
	if (!allIn) cout << text << endl;
	unsigned int initialMoney = GetMoneyPlayedByYou();

	while (yourBet != moneyPlayedOpponent_ && !allIn) {
		while (condition == false) { //Tokens of 100$
			//<< "Exemple : 8 4 1 0 1 ==> 8* 1$ +  4*5$ +  1*25$ +  0*50$ + 1*100$" << endl;
			cout << "Tokens of 100$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[4] < nbTokens) {
				cout << "Not enought 100$" << endl;
			}
			else {
				token100 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$
			cout << "Tokens of 50$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[3] < nbTokens) {
				cout << "Not enought 50$" << endl;
			}
			else {
				token50 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$	
			cout << "Tokens of 25$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[2] < nbTokens) {
				cout << "Not enought 25$" << endl;
			}
			else {
				token25 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$		
			cout << "Tokens of 5$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[1] < nbTokens) {
				cout << "Not enought 5$" << endl;
			}
			else {
				token5 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$
			cout << "Tokens of 1$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[0] < nbTokens) {
				cout << "Not enought 1$" << endl;
			}
			else {
				token1 = nbTokens;
				condition = true;
			}
		}
		yourBet = token1 * 1 + token5 * 5 + token25 * 25 + token50 * 50 + token100 * 100 + initialMoney;
		if (yourBet != moneyPlayedOpponent_) {	// If you did not bet enought. 
			cout << "Error : you have to chose the same amount of money" << endl;
			condition = false;
		}
	}

	//to know how many tokens you played and update the actually tokens you will have with you		
	tokensPlayedByYou_[0] = tokensPlayedByYou_[0] + token1;
	tokensPlayedByYou_[1] = tokensPlayedByYou_[1] + token5;
	tokensPlayedByYou_[2] = tokensPlayedByYou_[2] + token25;
	tokensPlayedByYou_[3] = tokensPlayedByYou_[3] + token50;
	tokensPlayedByYou_[4] = tokensPlayedByYou_[4] + token100;
	player_->SetTokens(token1, 0);
	player_->SetTokens(token5, 1);
	player_->SetTokens(token25, 2);
	player_->SetTokens(token50, 3);
	player_->SetTokens(token100, 4);

	cout << "Loading follow, " << yourBet << " $." << endl;

	if (!allIn) {
		const string file = player_->GetServer() ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
		ODrive od;
		od.writeInFile(file, to_string(yourBet), ofstream::out);	//	Erase file and write player's bet.
	}
	else {
		pot_ = pot_ + yourBet + moneyPlayedOpponent_ + moneyPlayedByYou_;	//	Gather all money in the pot before sending it to opponent. 
		string pot = to_string(pot_);

		const string file = player_->GetServer() ? ConstFiles::POTFILESERVER : ConstFiles::POTFILECLIENT;
		ODrive od;
		od.writeInFile(file, pot, ofstream::out);	// Send pot to opponent.

		/* Evolve the river. */
		if (!river_.GetCardList()->size()) {
			river_ += beginDeck_.DrawCard(3);
		}
		if (river_.GetCardList()->size() == 3) {
			river_ += beginDeck_.DrawCard(1);
		}
		if (river_.GetCardList()->size() == 4) {
			river_ += beginDeck_.DrawCard(1);
		}

		/* Determine the winner because it's a all-in. */
		string win = DetermineWinner();
		system("cls");
		GiveTokensToWinner(win);		//	Give money to the winner, else the loser lose all.
		WriteActionInFile("Follow");	// write my action to inform opponent.
		ChangeTurn();					// Change my turn, i cannot play until other player plays.
		system("pause");
	}

	if (allIn) return true;				//	End the round.
	else {
		WriteActionInFile("Follow");	//	If  followed without all-in.
		ChangeTurn();
		return false;
	}
}

/* 
 * Make a all-in. 
 */
void Round::All_In() {
	if (!yourTurn_) return;

	/* If your opponent made a all-in, then you just can follow or fold. */
	if (opponentAction_ == "All_In") {
		cout << endl << "Your opponent made a All In. You just can follow or fold." << endl;
		system("pause");
		return;
	}

	/* Confirm all-in. */
	string allin;
	cout << "You are making a All In. Are you sure ? Type All In to confirm." << endl;
	getline(cin >> ws, allin);

	if (allin != "all in" && allin != "All in" && allin != "all In" && allin != "All In") {
		cout << "Operation cancelled." << endl;
		return;
	}


	tokensPlayedByYou_[0] = tokensPlayedByYou_[0] + player_->GetTokens()[0];
	tokensPlayedByYou_[1] = tokensPlayedByYou_[1] + player_->GetTokens()[1];
	tokensPlayedByYou_[2] = tokensPlayedByYou_[2] + player_->GetTokens()[2];
	tokensPlayedByYou_[3] = tokensPlayedByYou_[3] + player_->GetTokens()[3];
	tokensPlayedByYou_[4] = tokensPlayedByYou_[4] + player_->GetTokens()[4];

	int yourBet = player_->GetTokens()[0] * 1 + player_->GetTokens()[1] * 5 + player_->GetTokens()[2] * 25 + player_->GetTokens()[3] * 50 + player_->GetTokens()[4] * 100;

	player_->SetTokens(player_->GetTokens()[0], 0);
	player_->SetTokens(player_->GetTokens()[1], 1);
	player_->SetTokens(player_->GetTokens()[2], 2);
	player_->SetTokens(player_->GetTokens()[3], 3);
	player_->SetTokens(player_->GetTokens()[4], 4);

	cout << "Loading all-in, " << yourBet << " $." << endl;

	const string file = player_->GetServer() ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
	ODrive od;
	od.writeInFile(file, to_string(yourBet + moneyPlayedByYou_), ofstream::out);	//	Inform your opponent about your bet.

	WriteActionInFile("All_In");	//	Inform your opponent of your action.

	ChangeTurn();
}

/*
 * Make a bet.
 */
void Round::Bet() {
	if (!yourTurn_) return;

	/* If your opponent made a all-in, then you just can follow or fold. */
	if (opponentAction_ == "All_In") {
		cout << endl << "Your opponent made a All In. You just can follow or fold." << endl;
		system("pause");
		return;
	}

	/* If you do not have engought money, you cannot bet. */
	if (player_->GetAllMoneys() < moneyPlayedOpponent_) {
		cout << endl << "You can't bet" << endl;
		system("pause");
		return;
	}

	bool condition = false;
	unsigned int nbTokens, token100 = 0, token50 = 0, token25 = 0, token5 = 0, token1 = 0, yourBet = 0;
	const string text = GetMoneyPlayedByYou() == 0 ? "Choose the tokens : " : "Choose the tokens to add to your previous bet : ";
	cout << text << endl;
	unsigned int initialMoney = GetMoneyPlayedByYou();
	while (yourBet <= moneyPlayedOpponent_ - initialMoney) {
		while (condition == false) { //Tokens of 100$
			//<< "Exemple : 8 4 1 0 1 ==> 8* 1$ +  4*5$ +  1*25$ +  0*50$ + 1*100$" << endl;
			cout << "Tokens of 100$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[4] < nbTokens) {
				cout << "Not enought 100$" << endl;
			}
			else {
				token100 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$
			cout << "Tokens of 50$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[3] < nbTokens) {
				cout << "Not enought 50$" << endl;
			}
			else {
				token50 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$	
			cout << "Tokens of 25$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[2] < nbTokens) {
				cout << "Not enought 25$" << endl;
			}
			else {
				token25 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$		
			cout << "Tokens of 5$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[1] < nbTokens) {
				cout << "Not enought 5$" << endl;
			}
			else {
				token5 = nbTokens;
				condition = true;
			}
		}
		condition = false;
		while (condition == false) { //Tokens of 100$
			cout << "Tokens of 1$ :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[0] < nbTokens) {
				cout << "Not enought 1$" << endl;
			}
			else {
				token1 = nbTokens;
				condition = true;
			}
		}
		yourBet = token1 * 1 + token5 * 5 + token25 * 25 + token50 * 50 + token100 * 100 + GetMoneyPlayedByYou();
		if (yourBet <= moneyPlayedOpponent_) {
			cout << "Error : You have to bet bigger" << endl;
			condition = false;
		}
	}

	//to know how many tokens you played and update the actually tokens you will have with you		
	tokensPlayedByYou_[0] = tokensPlayedByYou_[0] + token1;
	tokensPlayedByYou_[1] = tokensPlayedByYou_[1] + token5;
	tokensPlayedByYou_[2] = tokensPlayedByYou_[2] + token25;
	tokensPlayedByYou_[3] = tokensPlayedByYou_[3] + token50;
	tokensPlayedByYou_[4] = tokensPlayedByYou_[4] + token100;
	player_->SetTokens(token1, 0);
	player_->SetTokens(token5, 1);
	player_->SetTokens(token25, 2);
	player_->SetTokens(token50, 3);
	player_->SetTokens(token100, 4);

	cout << "Loading bet, " << yourBet << " $." << endl;

	const string file = player_->GetServer() ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
	ODrive od;
	od.writeInFile(file, to_string(yourBet), ofstream::out);	//	Inform your opponent of your current bet. 

	WriteActionInFile("Bet");	// inform your opponent of your action.

	ChangeTurn();
}

/* 
 * Make a check. 
 */
bool Round::Check() {
	bool end = false;

	if (!yourTurn_) return end;

	string winner = "";	//	Contains the winner. If '1', server won, else '0' client won the round.

	/* If your opponent made a all-in, then you just can follow or fold. */
	if (opponentAction_ == "All_In") {
		cout << endl << "Your opponent made a All In. You just can follow or fold." << endl;
		system("pause");
		return end;
	}

	/*	If ammount are not equals, you cannot check. */
	if (GetMoneyPlayedByYou() < moneyPlayedOpponent_) {
		cout << "You cannot check. You must have the same ammount as your opponent." << endl;
		system("pause");
		return end;
	}

	/* If you check, and your opponent followed or checked, then evolve the game. */
	if (opponentAction_ == "Check" || opponentAction_ == "Follow") {
		int riverSize = river_.GetCardList()->size();
		pot_ = GetMoneyPlayedByYou() + moneyPlayedOpponent_ + pot_;
		switch (riverSize) {
		case 0: Flop();		// If river is not revealed, then flop.
			break;
		case 3: Turn();		// If river is currently flop, then turn.
			break;
		case 4: River();	// If river is currently turn, thenriver.
			break;
		case 5: winner = DetermineWinner();		//	If river is river, then and the rouond and determine winner.
			system("cls");
			GiveTokensToWinner(winner);			// Give the pot to the winner.
			end = true;			// End the round.
			break;
		default:
			ODrive od;
			od.writeInErrorLogFile("Error river size.");
		}
		/* Write the evolution of pot in the file. */
		ODrive od;
		const string potFile = player_->GetServer() ? ConstFiles::POTFILESERVER : ConstFiles::POTFILECLIENT;
		od.writeInFile(potFile, to_string(pot_), ofstream::out);

		/* Set to zero, money played by player. Money doesn't belong to them anymore : it is the pot. */
		tokensPlayedByYou_[0] = tokensPlayedByYou_[1] = tokensPlayedByYou_[2] = tokensPlayedByYou_[3] = tokensPlayedByYou_[4] = 0;
		const string fileMoney = player_->GetServer() ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
		od.writeInFile(fileMoney, "0", ofstream::out);
	}

	if (winner == "") cout << "Loading check." << endl;	

	/* Action of the player. */
	WriteActionInFile("Check");

	/* Change the turn. */
	ChangeTurn();

	if (winner != "") system("pause");

	/* If end == true, there is a winner. */
	return end;
}

/*
 * Make a fold.
 */
bool Round::Fold() {
	if (!yourTurn_) return false;
	pot_ = GetMoneyPlayedByYou() + moneyPlayedOpponent_ + pot_;
	system("cls");
	/* Player is folding. */
	WriteActionInFile("Fold");
	cout << "You lose " << pot_ - moneyPlayedOpponent_ << " $." << endl;
	
	/* Write the evolution of pot in the file. */
	ODrive od;
	const string potFile = player_->GetServer() ? ConstFiles::POTFILESERVER : ConstFiles::POTFILECLIENT;
	od.writeInFile(potFile, to_string(pot_), ofstream::out);

	ChangeTurn();		// Change the turn.
	system("pause");
	return true;
}

#pragma endregion

#pragma region Card gestion

/*
 * Draw 2 cards to compose player's hand.
 */
void Round::DrawHand() {
	ODrive od;

	/* Pick 4 cards from the deck. */
	Deck cardsToDraw = beginDeck_.DrawCard(4);

	Deck* serverHand = new Deck({ cardsToDraw[0], cardsToDraw[2] });	// First and third cards comopose server hand.
	vector<string> serverCards = Deck::SerializeCards(*serverHand);		// Serialize cards to send them in odrive.
	od.writeInFile(ConstFiles::HANDFILESERVER, serverCards);
	player_->SetHand(serverHand);	//	Set player's hand.

	Deck* clientHand = new Deck({ cardsToDraw[1], cardsToDraw[3] });
	vector<string> clientCards = Deck::SerializeCards(*clientHand);		// Serialize client's hand cards to send them in odrive, to client get it.
	od.writeInFile(ConstFiles::HANDFILECLIENT, clientCards);
}

/*
 * Reveal the first 3 cards of the river.
 */
void Round::Flop() {
	ODrive od;
	/* If any cards are face up, then reveal the 3 first ones. */
	if (!river_.GetCardList()->size()) {
		river_ += beginDeck_.DrawCard(3);
		vector<string> riv = Deck::SerializeCards(river_);	//	Send the current river to inform opponent.
		const string riverFile = player_->GetServer() ? ConstFiles::RIVERFILESERVER : ConstFiles::RIVERFILECLIENT;
		od.writeInFile(riverFile, riv);

		/* Update the current date. */
		const string deckFile = player_->GetServer() ? ConstFiles::DECKFILESERVER : ConstFiles::DECKFILECLIENT;
		od.writeInFile(deckFile, "NULL", ofstream::out);	//	Erase file, before writing.
		od.writeInFile(deckFile, Deck::SerializeCards(beginDeck_));
	}
	else {
		od.writeInErrorLogFile("River : Error revealing flop.");
	}
}

/*
 * Reveal the 4th card of the river.
 */
void Round::Turn() {
	ODrive od;
	if (river_.GetCardList()->size() == 3) {
		Deck cardToAdd = beginDeck_.DrawCard();		//	draw a card.
		river_ += cardToAdd;	// Add card to river. 
		vector<string> river = Deck::SerializeCards(river_);
		const string riverFile = player_->GetServer() ? ConstFiles::RIVERFILESERVER : ConstFiles::RIVERFILECLIENT;		//	Update river.
		od.writeInFile(riverFile, "NULL", ofstream::out);
		od.writeInFile(riverFile, river);

		const string deckFile = player_->GetServer() ? ConstFiles::DECKFILESERVER : ConstFiles::DECKFILECLIENT;			//	Update deck.
		od.writeInFile(deckFile, "NULL", ofstream::out);
		od.writeInFile(deckFile, Deck::SerializeCards(beginDeck_));
	}
	else {
		od.writeInErrorLogFile("River : Error revealing turn.");
	}
}

/*
 * Reveal the last card of the river.
 */
void Round::River() {
	ODrive od;
	if (river_.GetCardList()->size() == 4) {
		Deck cardToAdd = beginDeck_.DrawCard();
		river_ += cardToAdd;			//	Add card to river.
		vector<string> river = Deck::SerializeCards(river_);
		const string riverFile = player_->GetServer() ? ConstFiles::RIVERFILESERVER : ConstFiles::RIVERFILECLIENT;		//	Update river.
		od.writeInFile(riverFile, "NULL", ofstream::out);
		od.writeInFile(riverFile, river);

		const string deckFile = player_->GetServer() ? ConstFiles::DECKFILESERVER : ConstFiles::DECKFILECLIENT;			//	Update deck.
		od.writeInFile(deckFile, "NULL", ofstream::out);	// Empty the file : clear the file, before to fill it again with the new Combinaison.
		od.writeInFile(deckFile, Deck::SerializeCards(beginDeck_));
	}
	else {
		od.writeInErrorLogFile("River : Error revealing flop.");
	}
}

/*
 * Determine the winner of the round.
 */
string Round::DetermineWinner() {
	Combinaison handThisCombinaison(player_->GetHand(), river_);	// Create my combinaison.
	ODrive od;
	const string file = player_->GetServer() ? ConstFiles::HANDFILECLIENT : ConstFiles::HANDFILESERVER;
	Deck handOpponent = Deck::DeserializeCards(od.readFile(file));	// Get opponent's hand.
	Combinaison handOpponentCombinaison(handOpponent, river_);	// Create opponent combinaison.
	string winner;
	if ((handThisCombinaison < handOpponentCombinaison) == -1 /*Tie Game*/) winner = "-1";	// It is a tie game (combinaison are equals)
	else if ((handThisCombinaison < handOpponentCombinaison) == 1 /*True*/) winner = player_->GetServer() ? "0" : "1";	// The opponent has a better combinaison (write '1' if the winner is the server, '0' for the client).
	else if ((handThisCombinaison < handOpponentCombinaison) == 0 /*False*/) winner = player_->GetServer() ? "1" : "0";
	else od.writeInErrorLogFile("Error evaluating winner.");

	/* If the current player is the winner, he gets the pot. Else player loses everything. */
	//GiveTokensToWinner(winner);

	const string fileWinner = player_->GetServer() ? ConstFiles::WINNERFILESERVER : ConstFiles::WINNERFILECLIENT;
	od.writeInFile(fileWinner, winner, ofstream::out);	// Write the winner in the appropriate file.

	return winner;
}

/* 
 * Hidden option used just for the demo to print deck. 
 * @param sorted : sort the deck ?
 */
void Round::PrintDeck() {
	system("cls");
	bool sorted = false;
	cout << "Sorted ?" << endl;
	cin >> sorted;							// Sort the deck by value.
	BeginDeck beginDeck = beginDeck_;		// Copy the deck to not sort it.
	if (sorted) beginDeck.SortCardListByValue();
	beginDeck.PrintDeck();					//	Print deck.
	system("pause");
}

#pragma endregion

/*
 * Write the current action of the player in the appropriate file.
 *
 * @param action : action of the player (fold, bet ...).
 */
void Round::WriteActionInFile(const string action) {
	ODrive od;
	if (action != "Fold" && action != "Bet" && action != "All_In" && action != "Follow" && action != "Check") {	//	If acton is not error.
		od.writeInErrorLogFile("Wrong action : " + action);
		return;
	}
	string file = player_->GetServer() ? ConstFiles::ACTIONFILESERVER : ConstFiles::ACTIONFILECLIENT;
	od.writeInFile(file, action, ofstream::out);		// Erase and write action.	
}

/*
 * Get informations from other player and act consequently. Return true, if the round is over.
 * @param mPG : address of the current menu.
 */
bool Round::GetInfoFromOpponent(MenuPokerGame* mPG) {
	bool end = false;

	bool isServer = player_->GetServer();

	ODrive od;

#pragma region Turn info
	/* While it is not the player's turn, he cannot play. */
	if (!yourTurn_) {
		cout << "Your partner is playing." << endl;
		const string turnFile = isServer ? ConstFiles::TURNFILECLIENT : ConstFiles::TURNFILESERVER;
		od.waitForChange(turnFile);
		yourTurn_ = true;
	}
#pragma endregion

#pragma region Pot info

	string potFile = isServer ? ConstFiles::POTFILECLIENT : ConstFiles::POTFILESERVER;
	vector<string> pot = od.readFile(potFile);
	if (pot.size() && stoi(pot[0]) > pot_) {
		pot_ = stoi(pot[0]);
		// Write in my appropriate file, the current pot.
		potFile = isServer ? ConstFiles::POTFILESERVER : ConstFiles::POTFILECLIENT;
		od.writeInFile(potFile, pot[0], ofstream::out);
		// Reset the money played by the player. It belongs to the pot now.
		tokensPlayedByYou_[0] = tokensPlayedByYou_[1] = tokensPlayedByYou_[2] = tokensPlayedByYou_[3] = tokensPlayedByYou_[4] = 0;
		const string moneyFile = isServer ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
		od.writeInFile(moneyFile, "0", ofstream::out);
	}
	else if (!pot.size()) pot_ = 0;

#pragma endregion

	const string fileWinner = isServer ? ConstFiles::WINNERFILECLIENT : ConstFiles::WINNERFILESERVER; // Check if the winner file is filled. If it is then, there is a winner (or a tie Game).
	vector<string> winners = od.readFile(fileWinner);
	string winner = "";
	if (winners.size()) {
#pragma region Winner Info
		system("cls");
		winner = winners[0];			// If ther is a winner, get it.
		GiveTokensToWinner(winner);		// Give token if the current player is the winner.
		system("pause");
		end = true;						// End the round.
#pragma endregion
	}
	else {	// If there is not winner. 
#pragma region Action info
		const string file = isServer ? ConstFiles::ACTIONFILECLIENT : ConstFiles::ACTIONFILESERVER; // Get the current action of the other player.
		vector<string> actions = od.readFile(file);
		string action;
		if (actions.size()) action = actions[0];		// If there is an action, get it.

		opponentAction_ = action;

		if (action == "Fold") {
			system("cls");
			cout << "Your opponent folded." << endl;
			string winner = isServer ? "1" : "0";	// Set the winner.
			GiveTokensToWinner(winner);		// If the current player is the winner
			system("pause");
			end = true;		//	End the game.
		}
		else if (action != "") {		// Retrieve the initial case.

#pragma region Deck info
			/* Get the deck. */
			const string deckFile = isServer ? ConstFiles::DECKFILECLIENT : ConstFiles::DECKFILESERVER;
			vector<string> deck = od.readFile(deckFile);
			if (beginDeck_.GetCardList()->size() > deck.size()) beginDeck_ = BeginDeck::ToBeginDeck(Deck::DeserializeCards(deck));
#pragma endregion

#pragma region River info
			/* If the river, evolved the update it. */
			const string riverFile = isServer ? ConstFiles::RIVERFILECLIENT : ConstFiles::RIVERFILESERVER;
			vector<string> river = od.readFile(riverFile);
			if (river_.GetCardList()->size() < river.size()) river_ = Deck::DeserializeCards(river);
#pragma endregion

#pragma region Money info
			/* Get money played by opponent. */
			const string fileMoney = isServer ? ConstFiles::MONEYFILECLIENT : ConstFiles::MONEYFILESERVER;
			vector<string> moneys = od.readFile(fileMoney);
			if (moneys.size()) moneyPlayedOpponent_ = stoi(moneys[0]);
			else moneyPlayedOpponent_ = 0;
#pragma endregion

#pragma endregion
		}
		system("cls");
		if (winner == "" && action != "Fold") mPG->ShowMenu(action);	// If there is not winner, then show the menu.
	}

	return end;		//	If end == true, end the round.
}

/*
 * Separate the pot in differents token, for the winner to earn it.
 */
void Round::GiveTokensToWinner(string winner) {
	/* Add the value pot as token to the actual player if this one won. */
	if ((winner == "1" && player_->GetServer()) || (winner == "0" && !player_->GetServer())) {
		cout << "You won the round. You earn " << pot_ - moneyPlayedByYou_ << " $." << endl;
		unsigned int tokens100ToAdd = pot_ / (unsigned int)100;
		player_->SetTokens(-tokens100ToAdd, 4);	// 100 $ tokens
		pot_ -= tokens100ToAdd * 100;
		unsigned int tokens50ToAdd = pot_ / (unsigned int)50;
		player_->SetTokens(-tokens50ToAdd, 3);	// 50 $ tokens
		pot_ -= tokens50ToAdd * 50;
		unsigned int tokens25ToAdd = pot_ / (unsigned int)25;
		player_->SetTokens(-tokens25ToAdd, 2);	// 25 $ tokens
		pot_ -= tokens25ToAdd * 25;
		unsigned int tokens5ToAdd = pot_ / (unsigned int)5;
		player_->SetTokens(-tokens5ToAdd, 1);	// 5 $ tokens
		pot_ -= tokens5ToAdd * 5;
		player_->SetTokens(-pot_, 0);			// 1 $ tokens
	}
	else if ((winner == "1" && !player_->GetServer()) || (winner == "0" && player_->GetServer())) { // If you losed the game.
		cout << "You losed the round. You lose " << pot_ - moneyPlayedOpponent_ << " $." << endl;
	}
	else if (winner == "-1") {	// Tie Game : each player has the half of the pot. Pot is always a pair number, on the ground that it is a sum of 2 numbers.
		unsigned int pot = pot_ / (unsigned int)2;

		unsigned int tokens100ToAdd = pot / (unsigned int)100;
		player_->SetTokens(-tokens100ToAdd, 4);	// 100 $ tokens
		pot -= tokens100ToAdd * 100;
		unsigned int tokens50ToAdd = pot / (unsigned int)50;
		player_->SetTokens(-tokens50ToAdd, 3);	// 50 $ tokens
		pot -= tokens50ToAdd * 50;
		unsigned int tokens25ToAdd = pot / (unsigned int)25;
		player_->SetTokens(-tokens25ToAdd, 2);	// 25 $ tokens
		pot -= tokens25ToAdd * 25;
		unsigned int tokens5ToAdd = pot / (unsigned int)5;
		player_->SetTokens(-tokens5ToAdd, 1);	// 5 $ tokens
		pot -= tokens5ToAdd * 5;
		player_->SetTokens(-pot, 0);			// 1 $ tokens
		cout << "Tie Game." << endl;
	}	
}