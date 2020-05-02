#include"game_definitions.h"
#include"menu_definitions.h"
#include"card_definitions.h"
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

			Card a("Jack", "diamond"), b("8", "club"), c("King", "heart"), d("2", "club"), e("9", "spade"), f("Queen", "club"), g("7", "club");
			vector<Card> hand = { a, b };
			vector<Card> river = {c, d, e, f, g};

			Combinaison(hand, river);

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
