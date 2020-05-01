#include "Definitions.h"
#include<iostream>
#include<string>
#include<vector>

/*Each card is unique and defined by a number and a suit*/
Card::Card(int number, string suit)
{
	number_ = number;
	suit_ = suit;
}

vector<string> Card::SuitList()
{
	vector<string> suitList = {"spade", "club", "heart", "diamond" }; // List of each (4) suits in a deck of cards
	return suitList;
}

vector<int> Card::NumberList()
{
	vector<int> numberList(13); // From 1 to 13 : from 1 to King (list with all the values a card can get) 
	for (unsigned int i = 1; i < 14; i++) numberList[i - 1] = i; // Let's full the list
	return numberList;
}

void Card::PrintCard()
{
	cout << number_ << " of " << suit_ << endl;
}