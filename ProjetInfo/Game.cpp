#include "menu.h"
#include "Definitions.h"
#include <iostream>
using namespace std;







Game::Game(bool application) {
	server_ = application;
	Player player1;
	player_ = &player1;
	Round round1;
	round_ = &round1;

	// définir correctement le round aussi

	// on lance les cartes ici ?
	
}

void Game::Start() {
	cout << "La partie ce lance" << endl;



}
