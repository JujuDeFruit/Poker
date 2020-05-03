#include "menu.h"
#include "Definitions.h"
#include <iostream>
#include <string>
using namespace std;




Player::Player() {	
	cout << "What is your name ?" << endl;
	cin >> name_;
	tokens_ = new unsigned int[5];
	*tokens_ = 20;		// 1$
	*(tokens_+1) = 10;	// 5$
	*(tokens_+2) = 4;   // 25$
	*(tokens_+3) = 2;	// 50$
	*(tokens_+4) = 1;	// 100$
	GetAllMoneys();
	PrintMoneyAndTokens();
}

void Player::GetAllMoneys() {
	allMyMoney_ = *tokens_ * 1 + *(tokens_ + 1) * 5 + *(tokens_ + 1) * 25 + *(tokens_ + 3) * 50 + *(tokens_ + 4) * 10;
}

void Player::PrintMoneyAndTokens() {
	cout << endl; 
	cout << "Token 1$: " << *tokens_<< "  Token 5$: " << *(tokens_ + 1) << "  Token 25$: " << *(tokens_ + 2) << "  Token 50$: " << *(tokens_ + 3) << "  Token 100$: " << *(tokens_ + 4) << endl;
	GetAllMoneys();
	cout << "Your sum : " << allMyMoney_ << " $" << endl;
}


//Player::~Player() {
//	delete tokens_;
//}