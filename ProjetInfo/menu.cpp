#include "menu_definitions.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

// Definition des methodes de la classe OptionMenu

OptionMenu::OptionMenu(const string &name,const string &description)
  : name_(name), description_(description)
{
}


// Definition des methodes de la classe Menu

Menu::Menu(const string & title) : title_(title)
{
}

void Menu::SetTitle(const string &title)
{
  title_=title;
}

void Menu::AddOption(const string &nom,const string &description)
{
  listeOptions_.push_back(OptionMenu(nom,description));
}


void Menu::ShowMenu()
{
  cout<<title_<<endl;
  for (int i=0;i<listeOptions_.size();i++) {
    cout<<"- "<<setw(2)<<i<<" : "<<listeOptions_[i].GetDescription()<<endl;
  }
}

int Menu::AskChoix()
{
  int choice;
  cout<<"Your choice : ";
  cin>>choice;
  cout<<endl;
  return choice;
}

void Menu::Execute()
{
  bool end=false;
  while(!end) {
    system("cls");
    ShowMenu();
    int choice=AskChoix();
    if (choice>=0 && choice<listeOptions_.size())
      ExecuteOption(listeOptions_[choice].GetNom(),end);
    else {
      cout<<"Wrong choice"<<endl;
      system("pause");
    }
  }
}



void Menu::ExecuteOption(const string &name,bool &end)
{
	Round* round = game_->GetRound();
	
	end=false;
	//menupokerstart
	if (name == "leave") end = Leave(); 
	else if (name == "start") end = Leave();
	else if (name == "help") Help();
	//menupokergame
	else if (name == "follow") round->Follow();
	else if (name == "all in");
	else if (name == "check");
	else if (name == "fold");
	//
	else {
    cout<<"Option not defined"<<endl;
    system("pause");
    }
}

bool Menu::Leave()
{
  bool end=false;
  char answer;
  cout<<"Do you want to leave (o/n) ?";
  cin>>answer;
  if ((answer=='o')||(answer=='O')) end=true;
  return end;
}

void Menu::Help()
{
  cout<<"La liste ci-dessus vous propose les options disponibles pour ce menu."<<endl;
  cout<<"Vous devez entrer un numero qui correspond a votre choix et valider en"<<endl;
  cout<<"appuyant sur la touche <Entree>."<<endl;
  cout<<"Une fois l'option executee, vous aurez a nouveau acces au menu."<<endl<<endl;
  system("pause");
}



