#include"Definitions.h"
#include"menu.h"
#include<iostream>

using namespace std;

int main()
{

    short unsigned int  choix;
    bool application, condition = false;
    while (condition == false) {
        cout << endl << "| Client taper 0 | Serveur taper 1 |" << endl;
        cin >> choix;
        if (choix == 0) {
            application = false;

            //start a game as client
            MenuPokerStart mstart;
            mstart.Execute();
            Game client(application);
            MenuPokerGame mgame(client);
            mgame.Execute();
              

            //en of the game
            condition = true;				
        }
        else if (choix == 1) {
            application = true;
            // start a game as server
            Game serveur(application);

         

            //end of the game
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
