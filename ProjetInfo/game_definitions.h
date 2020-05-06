#pragma once
#include<string>

using namespace std;

// ============================= ROUND ============================= //

class Round {
	//Card midCards_[3];	//the 4 cards in the middle
	//unsigned int pot_;	//the winner of the round, win the pot, 
public:
	Round();
	void Follow();
	void All_In();
	void Check();
	void Fold();
};

// ============================= PLAYER ============================= //
class Player {
private:
	string name_;
	unsigned int tokens_[5] = { 20, 10,4,2,1 };
	//unsigned int *tokens_;		//index0: token 1$ - index1: token 5$ - index2: token 25$ - index3: token 50$ - index4: token 100$
	unsigned int allMyMoney_;	//esay way to find all the moneys of the player
	//Deck main;
public:	
	Player();
	
	unsigned int GetAllMoneys();
	void PrintMoneyAndTokens();
	//~Player();
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