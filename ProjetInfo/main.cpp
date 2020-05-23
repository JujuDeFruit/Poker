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

    short unsigned int  choice;
    bool condition = false;
    while (condition == false) {
        cout << "                        BEST POKER OF THE WORLD " << endl;
        cout << "                    by DEGRANGE Alexis / RAYNAL Julien" << endl << endl;
        cout << endl << "====| Client write 0 ===== Server write 1 |====" << endl;
        cin >> choice;
        if (choice == 0) {

            //start a game as client
			Game client(false);

            //end of the game
            condition = true;				
        }
        else if (choice == 1) {

			Game server(true);
         
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
