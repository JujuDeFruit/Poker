#pragma once
#include "card_definitions.h"
#include<string>
#include <vector>
using namespace std;

class MenuPokerGame;

#pragma region Player

// ============================= PLAYER ============================= //
class Player {
private:
	string name_;
	bool isServer_;
	Deck* hand_;
	int initialMoney_ = 0;
	vector<unsigned int> tokens_{ 20, 10,4,2,1 };	//index0: token 1$ - index1: token 5$ - index2: token 25$ - index3: token 50$ - index4: token 100$			
	unsigned int allMyMoney_;					//esay way to find all the moneys of the player
public:
	Player(bool isServer);

	#pragma region Get / Set
	int GetInitialMoney() const { return initialMoney_; };
	vector<unsigned int> GetTokens() { return tokens_; };
	void SetTokens(unsigned int value, unsigned index) { tokens_[index] = tokens_[index] - value; };
	string GetName() const { return name_; };
	Deck GetHand() { return *hand_; };
	void SetHand(Deck* d) { hand_ = d; };
	bool GetServer() { return isServer_; };
	#pragma endregion

	unsigned int GetAllMoneys();
	void PrintMoneyAndTokens();

	~Player();
};

#pragma endregion

#pragma region Round

// ============================= ROUND ============================= //
class Round {
private:
	Player* player_;							// Player on the round.
	BeginDeck beginDeck_;						// Deck of 52 cards.
	Deck river_;								// The river. 
	unsigned int pot_ = 0;						// The common tokens available for all the players. The winner of the round earns all.
	unsigned int moneyPlayedOpponent_ = 0;
	unsigned int moneyPlayedByYou_ = 0;
	vector<unsigned int> tokensPlayedByYou_{ 0,0,0,0,0 };  //to know how many tokens you played
	string opponentAction_;						// Get opponent action to know if it is a double check. Then reveal a part of the river or end the round.
	bool yourTurn_;	// To know who played first then second..
	int roundId_;	// Round number to know which player plays first.
public:
	Round(Player* player, int id);

	void InitialiseRound();			// Initialise the round.
	void Start();					// Start the round.

	#pragma region Get / Set
	Player* GetPlayer() const { return player_; };
	Deck GetRiver() const { return river_; };
	void SetYourTurn(bool turn) { yourTurn_ = turn; };
	unsigned int GetPot() const { return pot_; };
	unsigned int GetMoneyPlayedOpponent() { return moneyPlayedOpponent_; };
	unsigned int GetMoneyPlayedByYou() { return moneyPlayedByYou_ = tokensPlayedByYou_[0] * 1 + tokensPlayedByYou_[1] * 5 + tokensPlayedByYou_[2] * 25 + tokensPlayedByYou_[3] * 50 + tokensPlayedByYou_[4] * 100; };

	#pragma endregion

	bool Follow();
	void All_In();
	bool Check();
	void Bet();
	bool Fold();

	void WriteActionInFile(const string);		// Write the current action of the player in the appropriate file.

	void Flop();
	void Turn();
	void River();

	void DrawHand();		// Draw hands.
	void ChangeTurn();		//Invert the bool to alert drive it is the other player to play. Bool specify if app is waiting from the drive, or if this one write on it.
	bool GetInfoFromOpponent(MenuPokerGame*);	// Get info from the opponent and act consequently. 
	string DetermineWinner();					// Determine which player is the winner of the round. Return "1" if the server is the winner, "0" for the client.
	void GiveTokensToWinner(string);		// Give tokens to the winner
};

#pragma endregion

#pragma region Game

// ============================= GAME ============================= //
class Game {
private:
	bool server_;
	Player* player_;
	int currentRoundId_ = 1; // Round ID to dertermine which player plays first.
public:
	Game(bool server);
	
	#pragma region Get / Set
	bool GetServer() const { return server_; };
	Player* GetPlayer() const { return player_; };
	void SetPlayer(Player* player) { player_ = player; };
	int GetCurrentRoundId() const { return currentRoundId_; };
	#pragma endregion

	void Start();
	void Synchronisation();
	~Game();
};

#pragma endregion