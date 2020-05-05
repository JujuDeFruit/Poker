#include"game_definitions.h"
#include"menu_definitions.h"
#include"card_definitions.h"
#include<iostream>

using namespace std;

int main()
{
    short unsigned int  choice;
    bool application, condition = false;
    while (condition == false) {
        cout << "                        BEST POKER OF THE WORLD " << endl;
        cout << "                    by DEGRANGE Alexis / RAYNAL Julien" << endl << endl;
        cout << endl << "====| Client write 0 ===== Server write 1 |====" << endl;
        cin >> choice;
        if (choice == 0) {
            application = false;
            //start a game as client

			Deck hand({ Card("7", "club"), Card("2", "heart") });
			Deck river({ Card("7", "diamond"), Card("7", "spade"), Card("2", "club"), Card("2", "diamond"), Card("8", "heart") });

			//cout << hand.CountCombinaison(combinaisonTypes::quads) << endl;
			Combinaison(hand, river);

            MenuPokerStart mstart;
            mstart.Execute();
            Game client(application);
            MenuPokerGame mgame(client);
            mgame.Execute();
              

            //en of the game
            condition = true;				
        }
        else if (choice == 1) {
            application = true;
            // start a game as server
            Game serveur(application);

         

            //end of the game
            condition = true;
        }
        else {
            cout << "Input Error, retype" << endl;  
        }




        cout << endl << "Do you want to restart a game with your friend ?" << endl;
        cout << "=== Write '1' to restart ===" << endl;
        cin >> choice;
        if (choice == 1) {
            condition = false;
            system("cls");
        }
    }



    return 0;
}
