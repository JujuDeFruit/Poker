#include <iostream>
using namespace std;
#include "menu.h"



//======================================= Menu1 =========================================/

MenuPokerStart::MenuPokerStart() : Menu("MenuPokerStart :") {    
    AddOption("help", "Show help");
    AddOption("start", "Start the game");    
   
}

//======================================= Menu2 =========================================/

MenuPokerGame::MenuPokerGame(Game& Game) : Menu("MenuPokerGame :") {
    Menu::Game_= &Game;
    gameIsCreate_ = true;
    AddOption("bet", "Bet a sum");
    AddOption("follow", "Follow your oppenant");
    AddOption("all in", "Bet everything, all in !");
    AddOption("check", "Check 0$");
    AddOption("fold", "Fold your cards"); 
    AddOption("leave", "Leave the game");
}