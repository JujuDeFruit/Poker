#pragma once

class Round {

public:
	void Follow();
};

class Game {
private:
	bool server_;
	Round* round_;
public:
	Game(bool server);
	bool GetServer() { return server_; };
	Round* GetRound() { return round_; };
	void SetRound(Round* round) { round_ = round; };
	void Start();
};