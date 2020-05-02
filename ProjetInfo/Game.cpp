#include "menu_definitions.h"
#include "game_definitions.h"
#include <iostream>
using namespace std;







Game::Game(bool application) {
	server_ = application;
	Round* round = new Round();
	SetRound(round);
	//round_ = &round1;
	// définir correctement le round aussi

	// on lance les cartes ici ?
	
}

void Game::Start() {
	cout << "La partie ce lance" << endl;



}
