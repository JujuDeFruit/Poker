#include <iostream>
using namespace std;
#include "menu_definitions.h"



//======================================= Menu1 =========================================/

MenuPokerStart::MenuPokerStart() : Menu("MenuPokerStart :") {    
    AddOption("help", "Show help");
    AddOption("start", "Start the game");    
    //AddOption("leave", "Leave the game");
}

//======================================= Menu2 =========================================/

MenuPokerGame::MenuPokerGame(Game& game) : Menu("MenuPokerGame :") {
    Menu::game_= &game;
    AddOption("bet", "Bet a sum");
    AddOption("follow", "Follow your oppenant");
    AddOption("all in", "Bet everything, all in !");
    AddOption("check", "Check 0$");
    AddOption("fold", "Fold your cards");
}