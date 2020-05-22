#include "menu_definitions.h"
#include "game_definitions.h"
#include "odrive.h"
#include "dirfilelist.h"
#include <iostream>
#include <list>
#include <fstream>

using namespace std;

/*
 * Contructor of the game.
 * @param application : if true, create a game as a client else create a game as a server.
 */
Game::Game(bool application) {
	cout << "Waiting for your partner !" << endl;

	ODrive od;
	od.clearAllFiles();

	server_ = application;

	Synchronisation();
	player_ = new Player(application);
	Start();
}

/*
 * Start the game as a client or a server.
 */
void Game::Start() {
	ODrive od;
	od.writeInErrorLogFile("Game begin.");

	bool winner = false;

	while (!winner) {
		MenuPokerStart mstart(this);
		mstart.Execute();
		currentRoundId_ += 1;
	}
}

/*
 * Synchronise 2 players.
 */
void Game::Synchronisation() {
	string file = "/__init__.txt";
	ODrive od;

	if (!server_) {
		od.writeInErrorLogFile("Client connection.");
		od.waitForChange(file);
	}
	else {
		od.writeInErrorLogFile("Server connection.");
		od.writeInFile(file, "ready", ios_base::app);
	}
}

/*
 * Destructor of the class Game.
 */
Game::~Game() {
	delete player_;
}