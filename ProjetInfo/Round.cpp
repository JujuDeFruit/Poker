#include "menu_definitions.h"
#include "game_definitions.h"
#include "card_definitions.h"
#include <iostream>
#include <vector>

using namespace std;

#pragma region Constructor

Round::Round(Player* player) {
	player_ = player;
	BeginDeck beginDeck;
	beginDeck.ShakeDeck();
	beginDeck_ = beginDeck;
}

#pragma endregion

void Round::Follow() {
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

#pragma region Card gestion

/* Reveal the first 3 cards of the river. */
void Round::Flop() {
	/* If any cards are face up, then reveal the 3 first ones. */
	if (!river_.GetCardList()->size()) river_ += beginDeck_.DrawCard(3);
	PrintRiver();
}

/* Reveal the 4th card of the river. */
void Round::Turn() {
	if (beginDeck_.GetCardList()->size() == 3) river_ += beginDeck_.DrawCard();
	PrintRiver();
}

/* Reveal the last card of the river. */
void Round::River() {
	if (river_.GetCardList()->size() == 4) river_ += beginDeck_.DrawCard();
	PrintRiver();
}

/* Print the cards in the river. */
void Round::PrintRiver() {
	river_.PrintDeck();
}

#pragma endregion

void Round::Check() {
	//do nothing ?
}