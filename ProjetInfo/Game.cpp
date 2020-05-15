#include "menu_definitions.h"
#include "game_definitions.h"
#include <iostream>
using namespace std;







Game::Game(bool application) {
	server_ = application;
	//Player player1;
	//Player* player = 
	player_ = new Player;
	//Round round1;
	round_ = new Round;

	// définir correctement le round aussi

	// on lance les cartes ici ?
	
}

void Game::Start() {
	cout << "La partie ce lance" << endl;



}
