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
public:    
    Menu(const string & title);
    void SetTitle(const string &title);
    void AddOption(const string &nom,const string &description);
    virtual void ShowMenu();
    int AskChoix();
    virtual void Execute();
    virtual void ExecuteOption(const string &nom,bool &fin) = 0;
    bool Leave();
    virtual void Help();
};

class MenuPokerStart : public Menu {
private:
	Game* game_ = NULL;
public:
    MenuPokerStart(Game*);
	virtual void ExecuteOption(const string &nom, bool &fin);
	virtual void ShowMenu();
};

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

