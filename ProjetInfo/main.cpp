#pragma region Includes

#include"game_definitions.h"
#include"menu_definitions.h"
#include"card_definitions.h"
#include<iostream>

#pragma endregion

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

			Deck hand1({ Card("7", "club"), Card("2", "heart") }), hand2({Card("2","diamond"), Card("4","heart")});
			Deck river({ Card("7", "diamond"), Card("4", "spade"), Card("King", "club"), Card("2", "spade"), Card("King", "heart") });

			Combinaison combi1(hand1, river);
			Combinaison combi2(hand2, river);

			bool3States myBool = combi1 > combi2;
			system("pause");

            MenuPokerStart mstart;
            mstart.Execute();           
            Game* client = new Game(application);
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
