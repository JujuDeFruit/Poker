#pragma once
#include<string>
#include "card_definitions.h"

using namespace std;

// ============================= ROUND ============================= //

class Round {
	BeginDeck beginDeck_;
	Deck river_;						// The river.
	unsigned int pot_ = 0;				// The common tokens available for all the players.
	unsigned int currentStake_ = 0;		// Attribute to know what is the current stake bet by players.
	bool isRoundOver_ = false;			// Bool to know when the round is finished.
public:
	Round();
	void Follow();
	void All_In();
	void Check();
	void Fold();
	void Flop();
	void Turn();
	void River();
	void PrintRiver();
};

// ============================= PLAYER ============================= //
class Player {
private:
	string name_;
	Round* currentRound_;
	unsigned int tokens_[5] = { 20, 10,4,2,1 };
	//unsigned int *tokens_;		//index0: token 1$ - index1: token 5$ - index2: token 25$ - index3: token 50$ - index4: token 100$
	unsigned int allMyMoney_;	//esay way to find all the moneys of the player
	Deck main;
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