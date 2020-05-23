#include "constantes_files_name.h"
#include "menu_definitions.h"
#include "game_definitions.h"
#include "odrive.h"
#include "dirfilelist.h"
#include <iostream>
#include <list>
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

/*
 * Contructor of the game.
 * @param application : if true, create a game as a client else create a game as a server.
 */
Game::Game(bool application) {
	cout << "Deleting files !" << endl;

	ODrive od;
	if (application) od.deleteAllFiles(); // TODO2

	server_ = application;

	cout << "Waiting for your partner !" << endl;

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
	ODrive od;

	if (!server_) {
		od.refresh("");
		/*while (!fileAlreadyExists(od, ConstFiles::INITFILE + ".cloud")) {
			od.refresh("");
		}*/ //TODO
		od.sync(ConstFiles::INITFILE);
		vector<string> callback = od.readFile(ConstFiles::INITFILE);
		if (!callback.size()) od.waitForChange(ConstFiles::INITFILE);
		od.writeInErrorLogFile("Client connection.");
	}
	else {
		od.writeInFile(ConstFiles::INITFILE, "ready", ios_base::out);
		od.writeInErrorLogFile("Server connection.");
	}
}

/*
 * Destructor of the class Game.
 */
Game::~Game() {
	delete player_;
}