#include "menu.h"
#include "Definitions.h"
#include <iostream>
using namespace std;







Game::Game(bool application) {
	server_ = application;
	Round round1;
	round_ = &round1;
	// d�finir correctement le round aussi

	// on lance les cartes ici ?
	
}

void Game::Start() {
	cout << "La partie ce lance" << endl;



}
