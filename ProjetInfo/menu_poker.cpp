#include "menu_definitions.h"
#include "odrive.h"
#include <iostream>
#include <fstream>

using namespace std;

//======================================= Menu1 =========================================/

MenuPokerStart::MenuPokerStart() : Menu("MenuPokerStart :") {    
    AddOption("help", "Show help");
    AddOption("start", "Start the game");    
}

//======================================= Menu2 =========================================/

MenuPokerGame::MenuPokerGame(Game* game) : Menu("Game :") {
    Menu::game_= game;
    gameIsCreate_ = true;

	string file = "/__init__.txt";
	ODrive od;

	if (!game->GetServer()) {
		od.sync(file);
		od.waitForChange(file);
	}
	else {
		od.writeInFile(file,"ready", ios_base::app);
	}

    AddOption("bet", "Bet a sum");
    AddOption("follow", "Follow your oppenant");
    AddOption("all in", "Bet everything, all in !");
    AddOption("check", "Check 0$");
    AddOption("fold", "Fold your cards"); 
    AddOption("leave", "Leave the game");
}