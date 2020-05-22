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
	roundId = id;

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
	const string deckFile = "Deck.txt";
	if (player_->GetServer()) {			// If the player is the master, then it manage the deck. 
		BeginDeck beginDeck(true);
		beginDeck.ShakeDeck();
		beginDeck_ = beginDeck;
		DrawHand();
		vector<string> deck = Deck::SerializeCards(beginDeck_);
		od.writeInFile(deckFile, deck);
	}
	else {
		vector<string> callback = od.readFile(deckFile);
		if (!callback.size()) {
			od.waitForChange(deckFile);
			callback = od.readFile(deckFile);
			beginDeck_ = BeginDeck::ToBeginDeck(Deck::DeserializeCards(callback));
		}
		DrawHand();
		od.writeInFile(deckFile, "NULL", ofstream::out);
		vector<string> deck = Deck::SerializeCards(beginDeck_);
		od.writeInFile(deckFile, deck);
	}

	Start();
}

void Round::Start() {
	MenuPokerGame mgame(this, river_, player_->GetHand());
	mgame.Execute();	
}

void Round::ChangeTurn() {
	ODrive od;
	const string file = "Turn.txt";
	string message = player_->GetServer() ? "0" : "1"; // Write '0' if server just played, else write '1'.
	od.writeInFile(file, message, ofstream::out);
	yourTurn_ = false;
}

#pragma region Options

void Round::Follow() {
	if (!yourTurn_) return;

	if (player_->GetAllMoneys() < moneyPlayedOpponent_) {
		cout << endl << "You can't follow" << endl;
		system("pause");
		return;
	}
	bool condition = false;
	unsigned int nbTokens, token100 = 0, token50 = 0, token25 = 0, token5 = 0, token1 = 0, yourBet = 0;
	cout << "Choose the tokens : " << endl;
	while (yourBet != moneyPlayedOpponent_) {
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
			cout << "Tokens of 25% :" << endl;
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
			cout << "Tokens of 5% :" << endl;
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
			cout << "Tokens of 1% :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[0] < nbTokens) {
				cout << "Not enought 1$" << endl;
			}
			else {
				token1 = nbTokens;
				condition = true;
			}
		}
		yourBet = token1 * 1 + token5 * 5 + token25 * 25 + token50 * 50 + token100 * 100;
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

}

void Round::All_In() {
	if (!yourTurn_) return;

	tokensPlayedByYou_[0] = tokensPlayedByYou_[0] + player_->GetTokens()[0];
	tokensPlayedByYou_[1] = tokensPlayedByYou_[1] + player_->GetTokens()[1];
	tokensPlayedByYou_[2] = tokensPlayedByYou_[2] + player_->GetTokens()[2];
	tokensPlayedByYou_[3] = tokensPlayedByYou_[3] + player_->GetTokens()[3];
	tokensPlayedByYou_[4] = tokensPlayedByYou_[4] + player_->GetTokens()[4];
	player_->SetTokens(player_->GetTokens()[0], 0);
	player_->SetTokens(player_->GetTokens()[1], 1);
	player_->SetTokens(player_->GetTokens()[2], 2);
	player_->SetTokens(player_->GetTokens()[3], 3);
	player_->SetTokens(player_->GetTokens()[4], 4);

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
	cout << "Choose the tokens : " << endl;
	while (yourBet <= moneyPlayedOpponent_) {
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
			cout << "Tokens of 25% :" << endl;
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
			cout << "Tokens of 5% :" << endl;
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
			cout << "Tokens of 1% :" << endl;
			cin >> nbTokens;
			if (player_->GetTokens()[0] < nbTokens) {
				cout << "Not enought 1$" << endl;
			}
			else {
				token1 = nbTokens;
				condition = true;
			}
		}
		yourBet = token1 * 1 + token5 * 5 + token25 * 25 + token50 * 50 + token100 * 100;
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

}

void Round::Check() {
	//do nothing ?
	if (!yourTurn_) return;
}

#pragma endregion

#pragma region Card gestion

/* 
 * Draw 2 cards to compose player's hand.
 */
void Round::DrawHand() {
	Deck* hand = new Deck(beginDeck_.DrawCard(2));
	player_->SetHand(hand);
	vector<string> cards = Deck::SerializeCards(*hand);
	ODrive od;
	string booleanString = player_->GetServer() ? "1Hand" : "0Hand";
	od.writeInFile(booleanString + ".txt", cards);
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
		od.writeInFile("River.txt", riv);
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
	if (beginDeck_.GetCardList()->size() == 3) {
		Deck cardToAdd = beginDeck_.DrawCard();
		river_ += cardToAdd;
		vector<string> card = Deck::SerializeCards(cardToAdd);
		od.writeInFile("River.txt", card);
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
		vector<string> card = Deck::SerializeCards(cardToAdd);
		od.writeInFile("River.txt", card);
	}
	else {
		od.writeInErrorLogFile("River : Error revealing flop.");
	}
}

#pragma endregion
