#include "menu_definitions.h"
#include "game_definitions.h"
#include <iostream>
using namespace std;







Game::Game(bool application) {
	server_ = application;	
	player_ = new Player;	
	round_ = new Round(player_);

	//initialization of the game
	if (server_ == true) { //to know who played first
		round_->SetYourTurn(true);
	}
	else {
		round_->SetYourTurn(false);
	}

	
	
}

void Game::Start() {
	cout << "La partie ce lance" << endl;



}
