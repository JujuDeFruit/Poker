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
		//od.refresh(""); TODO
		/*while (!fileAlreadyExists(od, HANDFILECLIENT + ".cloud")) {
			od.refresh("");
		}*/ //TODO
		// od.sync(clientHandFile); TODO
		vector<string> callback = od.readFile(ConstFiles::HANDFILECLIENT);		// Get client's hand.
		if (!callback.size()) od.waitForChange(ConstFiles::HANDFILECLIENT);
		callback = od.readFile(ConstFiles::HANDFILECLIENT);
		Deck* hand = new Deck(Deck::DeserializeCards(callback));
		player_->SetHand(hand);

		/*while (!fileAlreadyExists(od, ConstFiles::DECKFILESERVER + ".cloud")) {
			od.refresh("");
		}*/ //TODO
		// od.sync(ConstFiles::DECKFILESERVER); TODO
		vector<string> cards = od.readFile(ConstFiles::DECKFILESERVER);			// Set the deck.
		if (!cards.size()) od.waitForChange(ConstFiles::DECKFILESERVER);
		cards = od.readFile(ConstFiles::DECKFILESERVER);
		beginDeck_ = BeginDeck::ToBeginDeck(Deck::DeserializeCards(cards));

		od.writeInFile(ConstFiles::DECKFILECLIENT, cards);

	}

	Start();
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
	const string turnFile = player_->GetServer() ? ConstFiles::TURNFILESERVER : ConstFiles::TURNFILECLIENT;
	od.writeInFile(turnFile, message, ofstream::out);
	yourTurn_ = false;
}

#pragma region Options

void Round::Follow() {
	if (!yourTurn_) return;

	if (player_->GetAllMoneys() < moneyPlayedOpponent_ || moneyPlayedOpponent_ == 0) {
		cout << endl << "You can't follow" << endl;
		system("pause");
		return;
	}
	bool condition = false;
	unsigned int nbTokens, token100 = 0, token50 = 0, token25 = 0, token5 = 0, token1 = 0, yourBet = 0;
	const string text = GetMoneyPlayedByYou() == 0 ? "Choose the tokens : " : "Choose the tokens to add to your previous bet : ";
	cout << text << endl;
	while (yourBet != moneyPlayedOpponent_ - GetMoneyPlayedByYou()) {
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
		if (yourBet != moneyPlayedOpponent_) {
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

	cout << "Loading follow." << endl;

	const string file = player_->GetServer() ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
	ODrive od;
	od.writeInFile(file, to_string(yourBet), ofstream::out);

	WriteActionInFile("Follow");

	ChangeTurn();
}

void Round::All_In() {
	if (!yourTurn_) return;

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

	cout << "Loading all-in." << endl;

	const string file = player_->GetServer() ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
	ODrive od;
	od.writeInFile(file, to_string(yourBet), ofstream::out);

	WriteActionInFile("All_In");

	ChangeTurn();
}

void Round::Bet() {
	if (!yourTurn_) return;

	if (player_->GetAllMoneys() < moneyPlayedOpponent_) {
		cout << endl << "You can't bet" << endl;
		system("pause");
		return;
	}
	bool condition = false;
	unsigned int nbTokens, token100 = 0, token50 = 0, token25 = 0, token5 = 0, token1 = 0, yourBet = 0;
	const string text = GetMoneyPlayedByYou() == 0 ? "Choose the tokens : " : "Choose the tokens to add to your previous bet : ";
	cout << text << endl;
	while (yourBet <= moneyPlayedOpponent_ - GetMoneyPlayedByYou()) {
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

	cout << "Loading bet." << endl;

	const string file = player_->GetServer() ? ConstFiles::MONEYFILESERVER : ConstFiles::MONEYFILECLIENT;
	ODrive od;
	od.writeInFile(file, to_string(yourBet), ofstream::out);

	WriteActionInFile("Bet");

	ChangeTurn();
}

bool Round::Check() {
	bool end = false;

	if (!yourTurn_) return end;

	if (moneyPlayedByYou_ < moneyPlayedOpponent_) {
		cout << "You cannot check. You must have the same ammount as your opponent." << endl;
		system("pause");
		return end;
	}

	if (opponentAction_ == "Check") {
		int riverSize = river_.GetCardList()->size();
		pot_ = moneyPlayedByYou_ + moneyPlayedOpponent_;
		switch (riverSize) {
		case 0: Flop();
			break;
		case 3: Turn();
			break;
		case 4: River();
			break;
		case 5: DetermineWinner();
			end = true;
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
		moneyPlayedByYou_ = 0;
		moneyPlayedOpponent_ = 0;
	}

	/* Action of the player. */
	WriteActionInFile("Check");

	/* Change the turn. */
	ChangeTurn();

	/* If end == true, there is a winner. */
	return end;
}

bool Round::Fold() {
	if (!yourTurn_) return false;
	/* Player is folding. */
	WriteActionInFile("Fold");
	cout << "You lose." << endl;
	ChangeTurn();
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

	Deck cardsToDraw = beginDeck_.DrawCard(4);

	Deck* serverHand = new Deck({ cardsToDraw[0], cardsToDraw[2] });
	vector<string> serverCards = Deck::SerializeCards(*serverHand);
	od.writeInFile(ConstFiles::HANDFILESERVER, serverCards);
	player_->SetHand(serverHand);

	Deck* clientHand = new Deck({ cardsToDraw[1], cardsToDraw[3] });
	vector<string> clientCards = Deck::SerializeCards(*clientHand);
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
		vector<string> riv = Deck::SerializeCards(river_);
		const string riverFile = player_->GetServer() ? ConstFiles::RIVERFILESERVER : ConstFiles::RIVERFILECLIENT;
		od.writeInFile(riverFile, riv);

		const string deckFile = player_->GetServer() ? ConstFiles::DECKFILESERVER : ConstFiles::DECKFILECLIENT;
		od.writeInFile(deckFile, "NULL", ofstream::out);
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
		Deck cardToAdd = beginDeck_.DrawCard();
		river_ += cardToAdd;
		vector<string> river = Deck::SerializeCards(river_);
		const string riverFile = player_->GetServer() ? ConstFiles::RIVERFILESERVER : ConstFiles::RIVERFILECLIENT;
		od.writeInFile(riverFile, "NULL", ofstream::out);
		od.writeInFile(riverFile, river);

		const string deckFile = player_->GetServer() ? ConstFiles::DECKFILESERVER : ConstFiles::DECKFILECLIENT;
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
		river_ += cardToAdd;
		vector<string> river = Deck::SerializeCards(river_);
		const string riverFile = player_->GetServer() ? ConstFiles::RIVERFILESERVER : ConstFiles::RIVERFILECLIENT;
		od.writeInFile(riverFile, "NULL", ofstream::out);
		od.writeInFile(riverFile, river);

		const string deckFile = player_->GetServer() ? ConstFiles::DECKFILESERVER : ConstFiles::DECKFILECLIENT;
		od.writeInFile(deckFile, "NULL", ofstream::out);
		od.writeInFile(deckFile, Deck::SerializeCards(beginDeck_));
	}
	else {
		od.writeInErrorLogFile("River : Error revealing flop.");
	}
}

/*
 * Determine the winner of the round.
 */
void Round::DetermineWinner() {
	Combinaison handThisCombinaison(player_->GetHand(), river_);
	ODrive od;
	const string file = player_->GetServer() ? ConstFiles::HANDFILECLIENT : ConstFiles::HANDFILESERVER;
	Deck handOpponent = Deck::DeserializeCards(od.readFile(file));
	Combinaison handOpponentCombinaison(handOpponent, river_);
	string winner;
	if ((handThisCombinaison < handOpponentCombinaison) == -1) winner = "-1";	// It is a tie game (combinaison are equals)
	else if ((handThisCombinaison < handOpponentCombinaison) == 1 /*True*/) winner = player_->GetServer() ? "0" : "1";	// The opponent has a better combinaison (write '1' if the winner is the server, '0' for the client).
	else if ((handThisCombinaison < handOpponentCombinaison) == 0 /*False*/) winner = player_->GetServer() ? "1" : "0";
	else od.writeInErrorLogFile("Error evaluating winner.");

	GiveTokensToWinner(winner);

	const string fileWinner = player_->GetServer() ? ConstFiles::WINNERFILESERVER : ConstFiles::WINNERFILESERVER;
	od.writeInFile(fileWinner, winner, ofstream::out);
}

#pragma endregion

/*
 * Write the current action of the player in the appropriate file.
 *
 * @param action : action of the player (fold, bet ...).
 */
void Round::WriteActionInFile(const string action) {
	ODrive od;
	if (action != "Fold" && action != "Bet" && action != "All_In" && action != "Follow" && action != "Check") {
		od.writeInErrorLogFile("Wrong action : " + action);
		return;
	}
	string file = player_->GetServer() ? ConstFiles::ACTIONFILESERVER : ConstFiles::ACTIONFILECLIENT;
	od.writeInFile(file, action, ofstream::out);
}

/*
 * Get informations from other player and act consequently. Return true, if the round is over.
 * @param mPG : address of the current menu.
 */
bool Round::GetInfoFromOpponent(MenuPokerGame* mPG) {
	bool end = false;

	bool isServer = player_->GetServer();

	ODrive od;
	//od.refresh(""); TODO

#pragma region Turn info
	/* While it is not the player's turn, he cannot play. */
	if (!yourTurn_) {
		cout << "Your partner is playing." << endl;
		const string turnFile = isServer ? ConstFiles::TURNFILECLIENT : ConstFiles::TURNFILESERVER;
		/*while (!fileAlreadyExists(od, turnFile + ".cloud")) {
			od.refresh("");
		}
		od.sync(turnFile); TODO */
		od.waitForChange(turnFile);
		yourTurn_ = true;
	}
#pragma endregion

#pragma region Pot info

	const string potFile = isServer ? ConstFiles::POTFILECLIENT : ConstFiles::POTFILESERVER;
	/*if (!fileAlreadyExists(od, potFile)) {
		while (!fileAlreadyExists(od, potFile + ".cloud")) {
			od.refresh("");
		}
		od.sync(potFile);
	} TODO2 */
	vector<string> pot = od.readFile(potFile);
	if (pot.size()) {
		pot_ = stoi(pot[0]);
		moneyPlayedByYou_ = 0;
		moneyPlayedOpponent_ = 0;
	}
	else pot_ = 0;

#pragma endregion

#pragma region Winner Info

	const string fileWinner = isServer ? ConstFiles::WINNERFILECLIENT : ConstFiles::WINNERFILESERVER; // Check if the winner file is filled. If it is then, there is a winner (or a tie Game).
	/*if (!fileAlreadyExists(od, fileWinner)) {
		while (!fileAlreadyExists(od, fileWinner + ".cloud")) {
			od.refresh("");
		}
		od.sync(fileWinner);
	} TODO2 */
	vector<string> winners = od.readFile(fileWinner);
	string winner = "";
	if (winners.size()) {
		winner = winners[0];
		GiveTokensToWinner(winner);
		end = true;
	}

#pragma endregion

#pragma region Action info
	const string file = isServer ? ConstFiles::ACTIONFILECLIENT : ConstFiles::ACTIONFILESERVER; // Get the current action of the other player.
	/*while (!fileAlreadyExists(od, file + ".cloud")) {
		od.refresh("");
	}
	od.sync(file); TODO */
	vector<string> actions = od.readFile(file);
	string action;
	if (actions.size()) action = actions[0];

	opponentAction_ = action;

	if (action == "Fold") {
		system("cls");
		cout << "You won the round, your opponent foldded." << endl;
		string winner = isServer ? "1" : "0";	// Set the winner.
		GiveTokensToWinner(winner);
		system("pause");
		end = true;
	}
	else if (action != "") {		// Retrieve the initial case.

#pragma region Deck info
		const string deckFile = isServer ? ConstFiles::DECKFILECLIENT : ConstFiles::DECKFILESERVER;
		/*while (!fileAlreadyExists(od, deckFile + ".cloud")) {
		od.refresh("");
		}
		od.sync(deckFile); TODO */
		vector<string> deck = od.readFile(deckFile);
		if (beginDeck_.GetCardList()->size() > deck.size()) beginDeck_ = BeginDeck::ToBeginDeck(Deck::DeserializeCards(deck));
#pragma endregion

#pragma region River info
		const string riverFile = isServer ? ConstFiles::RIVERFILECLIENT : ConstFiles::RIVERFILESERVER;
		/*while (!fileAlreadyExists(od, riverFile + ".cloud")) {
		od.refresh("");
		}
		od.sync(riverFile); TODO */
		vector<string> river = od.readFile(riverFile);
		if (river_.GetCardList()->size() < river.size()) river_ = Deck::DeserializeCards(river);
#pragma endregion

#pragma region Money info
		const string fileMoney = isServer ? ConstFiles::MONEYFILECLIENT : ConstFiles::MONEYFILESERVER;
		/*while (!fileAlreadyExists(od, fileMoney + ".cloud")) {
			od.refresh("");
		}
		od.sync(fileMoney); TODO */

		vector<string> moneys = od.readFile(fileMoney);
		if (moneys.size()) moneyPlayedOpponent_ = stoi(moneys[0]);
		else moneyPlayedOpponent_ = 0;
#pragma endregion

	}
#pragma endregion

	system("cls");
	if (winner == "") mPG->ShowMenu(action);	// If there is not winner, then show the menu.

	if (end) od.clearFiles();

	return end;
}

/*
 * Separate the pot in differents token, for the winner to earn it.
 */
void Round::GiveTokensToWinner(string winner) {
	/* Add the value pot as token to the actual player if this one won. */
	if ((winner == "1" && player_->GetServer()) || (winner == "0" && !player_->GetServer())) {
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
}