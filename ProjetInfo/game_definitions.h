#pragma once
#include<string>
#include <vector>
using namespace std;


// ============================= PLAYER ====================*========= //
class Player {
private:
	string name_;
	vector<unsigned int> tokens_{ 20, 10,4,2,1 };	//index0: token 1$ - index1: token 5$ - index2: token 25$ - index3: token 50$ - index4: token 100$			
	unsigned int allMyMoney_;					//esay way to find all the moneys of the player
	//Deck main;
public:	
	Player();
	vector<unsigned int>  GetTokens() { return tokens_; };
	void SetTokens(unsigned int value, unsigned index) { tokens_[index] = tokens_[index]-value; };
	unsigned int GetAllMoneys();
	void PrintMoneyAndTokens();
	//~Player();
};

// ============================= ROUND ============================= //
class Round {	
private:
	//Card midCards_[3];	//the 4 cards in the middle
	//unsigned int pot_;	//the winner of the round, win the pot, 
	Player* player_;
	unsigned int moneyPlayedOpponent_ = 100;
	unsigned int moneyPlayedByYou_ = 0;
	vector<unsigned int> tokensPlayedByYou_{ 0,0,0,0,0 };  //to know how many tokens you played
	bool yourTurn_;	//to know who played first then second..
public:
	Round(Player* player);			
	void SetYourTurn(bool turn) { yourTurn_ = turn; };
	bool GetYourTurn() { return yourTurn_; };
	unsigned int GetMoneyPlayedOpponent() { return moneyPlayedOpponent_; };
	unsigned int GetMoneyPlayedByYou() { return moneyPlayedByYou_ = tokensPlayedByYou_[0]*1+ tokensPlayedByYou_[1]*5+ tokensPlayedByYou_[2]*25+ tokensPlayedByYou_[3]*50+ tokensPlayedByYou_[4]*100; };
	void Follow();
	void All_In();
	void Check();
	void Fold();
	void Bet();
};


// ============================= GAME ============================= //
class Game {
private:
	bool server_;
	Round* round_;
	Player* player_;
public:
	Game(bool server);
	bool GetServer() { return server_; };
	Round* GetRound() { return round_; };
	void SetRound(Round* round) { round_ = round; };
	Player* GetPlayer() { return player_; };
	void SetPlayer(Player* player) { player_ = player; };
	void Start();
};