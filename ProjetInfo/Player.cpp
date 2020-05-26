#include "menu_definitions.h"
#include "game_definitions.h"
#include <iostream>
#include <string>

using namespace std;

#pragma region Constructor

Player::Player(bool isServer) {	
	isServer_ = isServer;
	cout << "What is your name ?" << endl;
	getline(cin >> ws, name_);
	initialMoney_ = tokens_[0] * 1 + tokens_[1] * 5 + tokens_[2] * 25 + tokens_[3] * 50 + tokens_[4] * 100;		// Set initial money to know when a player won. 
}

#pragma endregion

#pragma region Print money

unsigned int Player::GetAllMoneys() {		
	allMyMoney_ = tokens_[0] * 1 + tokens_[1] * 5 + tokens_[2] * 25 + tokens_[3] * 50 + tokens_[4] * 100;
	return allMyMoney_;
}

void Player::PrintMoneyAndTokens() {
	cout << endl; 
	cout << "Token 1$: " << tokens_[0] << "  Token 5$: " << tokens_[1] << "  Token 25$: " << tokens_[2] << "  Token 50$: " << tokens_[3] << "  Token 100$: " << tokens_[4] << endl;	
	cout << endl;
	cout << "Money you have : " << GetAllMoneys() << " $" << endl;
}

#pragma endregion

#pragma region Destructor

Player::~Player() {
	delete hand_;
}

#pragma endregion