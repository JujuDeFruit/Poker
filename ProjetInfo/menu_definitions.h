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
    bool gameIsCreate_ = false;			//to know if the game is created or not.
public:    
    Menu(const string & title);
    void SetTitle(const string &title);
    void AddOption(const string &nom,const string &description);
    virtual void ShowMenu();
    int AskChoix();
    virtual void Execute() = 0;
    virtual void ExecuteOption(const string &nom,bool &fin) = 0;
    bool Leave();
    virtual void Help();
};

/*
 * @author : Alexis Degrange 
 * Menu to start a round. 
 */
class MenuPokerStart : public Menu {
private:
	Game* game_ = NULL;		// The starting round menu is linked to a game.
public:
    MenuPokerStart(Game*);
	virtual void ExecuteOption(const string &nom, bool &fin);
	virtual void ShowMenu();
	virtual void Execute();
};

/* 
 * @author : Alexis Degrange
 * Manage round menu, to print all actions a player can do (bet, fllow ...) 
 */
class MenuPokerGame : public Menu { 
private :
	Round* round_;
public :
    MenuPokerGame(Round* round);
	virtual void ExecuteOption(const string &nom, bool &fin);
	virtual void ShowMenu(string);		// Set the action of the opponent as parameter.
	virtual void Execute();
};

#endif

