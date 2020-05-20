//#pragma region Includes
//
//#include"game_definitions.h"
//#include"menu_definitions.h"
//#include"card_definitions.h"
//#include<iostream>
//
//#pragma endregion
//
//using namespace std;
//
//int main()
//{
//	short unsigned int  choice;
//	bool application, condition = false;
//	Game* client;
//	while (!condition) {
//		cout << "                        BEST POKER OF THE WORLD " << endl;
//		cout << "                    by DEGRANGE Alexis / RAYNAL Julien" << endl << endl;
//		cout << endl << "=================================================================" << endl;
//
//		application = false;
//		//start the game
//
//		MenuPokerStart mstart;
//		mstart.Execute();
//		client = new Game(application);
//		MenuPokerGame mgame(client);
//		mgame.Execute();
//
//		//en of the game
//		condition = true;
//
//
//		cout << endl << "Do you want to restart a game with your friend ?" << endl;
//		cout << "=== Write '1' to restart ===" << endl;
//		cin >> choice;
//		if (choice == 1) {
//			condition = false;
//			system("cls");
//		}
//	}
//
//	delete client;
//
//	return 0;
//}
