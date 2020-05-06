#include "menu_definitions.h"
#include "game_definitions.h"
#include <iostream>
#include <string>
using namespace std;




Player::Player() {	
	cout << "What is your name ?" << endl;
	cin >> name_;
	/*tokens_ = new unsigned int[5];
	*tokens_ = 20;		// 1$
	*(tokens_+1) = 10;	// 5$
	*(tokens_+2) = 4;   // 25$
	*(tokens_+3) = 2;	// 50$
	*(tokens_+4) = 1;	// 100$*/
	//tokens_[5] = { 20,10,4,2,1 };

	//GetAllMoneys();
	PrintMoneyAndTokens();



}

unsigned int Player::GetAllMoneys() {
	//allMyMoney_ = *tokens_ * 1 + *(tokens_ + 1) * 5 + *(tokens_ + 1) * 25 + *(tokens_ + 3) * 50 + *(tokens_ + 4) * 10; tokens_[]
	allMyMoney_ = tokens_[0] * 1 + tokens_[1] * 5 + tokens_[2] * 25 + tokens_[3] * 50 + tokens_[4] * 10;
	return allMyMoney_;
}

void Player::PrintMoneyAndTokens() {
	cout << endl; 
	cout << "Token 1$: " << tokens_[0] << "  Token 5$: " << tokens_[1] << "  Token 25$: " << tokens_[2] << "  Token 50$: " << tokens_[3] << "  Token 100$: " << tokens_[4] << endl;	
	cout << "Your sum : " << GetAllMoneys() << " $" << endl;
}


//Player::~Player() {
//	delete tokens_;
//}