#ifndef _MENU
#define _MENU

#include <string>
#include <vector>

using namespace std;


class OptionMenu
{
    string nom_;
    string description_;
  public:
    OptionMenu(const string &nom,const string &description);
    string getNom() const {return nom_;}
    string getDescription() const {return description_;}
};


class Menu
{
    string title_;
    vector<OptionMenu> listeOptions_;
  public:
    Menu(const string & title);
    void SetTittle(const string &title);
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
    MenuPokerGame();
};

/*class MenuMoyenne : public Menu
{
    Semestre* semestre_;
public:
    MenuMoyenne(Semestre* semestre);
    void executerOption(const string& nom, bool& fin);
    void saisirMatieres();
    void saisirNotes();
    void calculerMoyenne();
};*/
#endif

