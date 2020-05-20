#ifndef _MENU
#define _MENU

#include <string>
#include <vector>
#include "game_definitions.h"

using namespace std;


class OptionMenu
{
    string name_;
    string description_;
  public:
    OptionMenu(const string &nom,const string &description);
    string GetNom() const {return name_;}
    string GetDescription() const {return description_;}
};


class Menu{      
protected : 
	string title_;
	vector<OptionMenu> listeOptions_;
    bool gameIsCreate_ = false;			//to know if the game is created
    Game* game_ = NULL;
public:    
    Menu(const string & title);
    void SetTitle(const string &title);
    void AddOption(const string &nom,const string &description);
    void ShowMenu();
    int AskChoix();
    void Execute();
    virtual void ExecuteOption(const string &nom,bool &fin);
    bool Leave();
    virtual void Help();
};

class MenuPokerStart : public Menu
{
public:
    MenuPokerStart();

};

class MenuPokerGame : public Menu { 

public :
    MenuPokerGame(Game* game);
};

#endif

