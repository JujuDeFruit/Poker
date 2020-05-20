#pragma region Includes
#include "odrive.h"
#include"game_definitions.h"
#include"menu_definitions.h"
#include"card_definitions.h"
#include<iostream>

#pragma endregion

using namespace std;

int main()
{
	ODrive od;
	od.refresh("");

    short unsigned int  choice;
    bool application, condition = false;
    while (condition == false) {
        cout << "                        BEST POKER OF THE WORLD " << endl;
        cout << "                    by DEGRANGE Alexis / RAYNAL Julien" << endl << endl;
        cout << endl << "====| Client write 0 ===== Server write 1 |====" << endl;
        cin >> choice;
        if (!choice) {
            application = false;
            //start a game as client

            MenuPokerStart mstart;
            mstart.Execute();           
            Game* client = new Game(application);
            MenuPokerGame mgame(client);
            mgame.Execute();              

            //en of the game
            condition = true;				
        }
        else if (choice) {
            application = true;
            // start a game as server
            MenuPokerStart mstart;
            mstart.Execute();
            Game* client = new Game(application);
            MenuPokerGame mgame(client);
            mgame.Execute();
         
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
