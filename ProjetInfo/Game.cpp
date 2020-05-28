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

const int FILENUMBER = 17;

/*
 * Contructor of the game.
 * @param application : if true, create a game as a client else create a game as a server.
 */
Game::Game(bool application) {
	server_ = application;

	string local = "";
	cout << "Local (1) ?" << endl;
	cin >> local;

	if ((local == "1" && server_) || (local != "1")) {	// local variable for a local or online demo.
		cout << "Deleting files !" << endl;
		ODrive od;
		/* Delete all previous files to start a new game. */
		od.deleteAllFiles();
	}

	if (local != "1") {
		cout << "Initialise files" << endl;
		InitialiseFiles();
	}

	cout << "Waiting for your partner !" << endl;

	/* Waiting for 2 players. */
	Synchronisation();
	player_ = new Player(application);
	Start();
}

/*
 * Synchronise 2 players.
 */
void Game::Synchronisation() {
	ODrive od;

	if (!server_) {
		od.refresh("");
		od.sync(ConstFiles::INITFILE);
		/* The client wait until the server get connected. */
		vector<string> callback = od.readFile(ConstFiles::INITFILE);
		if (!callback.size()) od.waitForChange(ConstFiles::INITFILE);
		od.writeInErrorLogFile("Client connection.");
	}
	else {
		/* Server write 'ready' in init file to synchronize the client. */
		od.writeInFile(ConstFiles::INITFILE, "ready", ios_base::out);
		od.writeInErrorLogFile("Server connection.");
	}
}

/*
 * Start the game as a client or a server.
 */
void Game::Start() {
	ODrive od;
	/* Write in report log. (error.txt) */
	od.writeInErrorLogFile("Game begin.");

	while (1) {
		/* Menu to start a round. */
		MenuPokerStart mstart(this);
		mstart.Execute();
		// End of a round.
		currentRoundId_ += 1;	// Round Id, to switch the beginner of the game.  
		/* If a player has not money anymore, or if he has all the money enable. Then the whole game is over. */
		if (!player_->GetAllMoneys() || player_->GetAllMoneys() == 2 * player_->GetInitialMoney()) break;
	}

	/* End of the game. Print a message. */
	if (!player_->GetAllMoneys()) {
		system("cls");
		cout << "You lose the game over your opponent. You are the LOSER !" << endl;
		system("pause");
	}
	else if (player_->GetAllMoneys() == 2 * player_->GetInitialMoney()) {
		system("cls");
		cout << "You won the game over your opponent. You are the WINNER !" << endl;
		system("pause");
	}
}

/*
 * Get all files, and synchronise them in local. 
 */
void Game::InitialiseFiles() {

	ODrive od;
	list<string> files = Files(server_);
	/* Open the file without writing in it to create the file. */
	for each (string file in files) {
		od.writeInFile(file, "NULL", ofstream::out);
	}

	list<string> fileList;
	bool error = getDirectoryFileList(od.getOdDrivePath(), fileList);
	while (fileList.size() != FILENUMBER && !error) {
		od.refresh("");
		error = getDirectoryFileList(od.getOdDrivePath(), fileList);
	}
	od.syncAll();
}

/*
 * Destructor of the class Game.
 */
Game::~Game() {
	delete player_;	// Delete player. 
}