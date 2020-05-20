#include "menu_definitions.h"
#include "odrive.h"
#include <iostream>

using namespace std;

//======================================= Menu1 =========================================/

MenuPokerStart::MenuPokerStart() : Menu("MenuPokerStart :") {    
    AddOption("help", "Show help");
    AddOption("start", "Start the game");    
}

//======================================= Menu2 =========================================/

MenuPokerGame::MenuPokerGame(Game* game) : Menu("MenuPokerGame :") {
    Menu::game_= game;
    gameIsCreate_ = true;

	//string file = "/Is_Game_Ready.txt";
    string file = "Google Drive\\Mini Projet Info\\odrive_poker\\Is_Game_Ready.txt";
	ODrive od;
	od.writeInFile(file, game->GetPlayer()->GetName() + ":ready", ios_base::app);
	od.waitForChange(file);
	od.writeInFile(file, "Ok", ios_base::app);

    AddOption("bet", "Bet a sum");
    AddOption("follow", "Follow your oppenant");
    AddOption("all in", "Bet everything, all in !");
    AddOption("check", "Check 0$");
    AddOption("fold", "Fold your cards"); 
    AddOption("leave", "Leave the game");
}