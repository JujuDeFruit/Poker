#include"Definitions.h"
#include"menu.h"
#include<iostream>

using namespace std;

int main()
{

    short unsigned int  choix;
    bool application, condition = false;

	Deck deck;
	deck.PrintDeck();
    while (condition == false) {
        cout << endl << "| Client taper 0 | Serveur taper 1 |" << endl;
        cin >> choix;
        if (choix == 0) {
            application = false;
            // lancer une partie client
            MenuPoker p1;
            p1.executer();

            //la partie ce finie
            condition = true;

			
			
        }
        else if (choix == 1) {
            application = true;
            // lancer une partie serveur
            MenuPoker p2;
            p2.executer();

            // la partie ce finie
            condition = true;
        }
        else {
            cout << "Erreur de saisie, retaper :" << endl;
        }




        cout << endl << "Voulez-vous relancer une partie avec votre amis ?" << endl;
        cout << "Taper '1' si oui, n'importe sinon" << endl;
        cin >> choix;
        if (choix == 1) {
            condition = false;
        }
    }



    return 0;
}
