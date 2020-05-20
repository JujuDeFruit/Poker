#include "menu_definitions.h"
#include "game_definitions.h"
#include "odrive.h"
#include "dirfilelist.h"
#include <iostream>
#include <list>
#include <fstream>

using namespace std;

Game::Game(bool application) {
	ClearAllFiles();

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

/**
 * Clear all the files in the drive when the game is over. 
 */
void Game::ClearAllFiles() {
	ODrive od;
	const string odDrivePath = od.getOdDrivePath();

	list<string> fileList;
	bool error = getDirectoryFileList(odDrivePath, fileList);

	if (error) od.writeInErrorLogFile("An error occured when game closed.");
	else {
		fileList.sort();
		for each (string file in fileList) {
			od.writeInFile(file, "NULL", ofstream::out);
		}
	}
}