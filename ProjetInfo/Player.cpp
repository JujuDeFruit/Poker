#include "menu_definitions.h"
#include "game_definitions.h"
#include <iostream>
#include <string>
using namespace std;




Player::Player() {	
	cout << "What is your name ?" << endl;
	cin >> name_;
	
	PrintMoneyAndTokens();
}

unsigned int Player::GetAllMoneys() {		
	allMyMoney_ = tokens_[0] * 1 + tokens_[1] * 5 + tokens_[2] * 25 + tokens_[3] * 50 + tokens_[4] * 100;
	return allMyMoney_;
}

void Player::PrintMoneyAndTokens() {
	cout << endl; 
	cout << "Token 1$: " << tokens_[0] << "  Token 5$: " << tokens_[1] << "  Token 25$: " << tokens_[2] << "  Token 50$: " << tokens_[3] << "  Token 100$: " << tokens_[4] << endl;	
	cout << "Money you have : " << GetAllMoneys() << " $" << endl;
}



//Player::~Player() {
//	delete tokens_;
//}