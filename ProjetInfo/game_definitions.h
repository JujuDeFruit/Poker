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
	string name_;
protected:
	unsigned int* tokens_;		//index0: token 1$ - index1: token 5$ - index2: token 25$ - index3: token 50$ - index4: token 100$
	unsigned int allMyMoney_;	//esay way to find all the moneys of the player
public:
	Player();
	void GetAllMoneys();
	void PrintMoneyAndTokens();
	//~Player();
};

// ============================= GAME ============================= //
class Game {
private:
	bool server_;
public:
	Round* round_;
	Player* player_;
	Game(bool server);
	bool GetServer() { return server_; };
	void Start();
};