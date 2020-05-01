#include"Definitions.h"
#include<iostream>

Deck::Deck()
{
	const vector<string> valueList = Deck::ValueList();
	const vector<string> suitList = Deck::SuitList();

	/* This part will create the 52 cards deck. Instead of settings all cards manually, we decided to create the deck from the previous lists : NumberList and SuitList*/
	for (vector<string>::const_iterator itS = suitList.begin(); itS != suitList.end(); itS++)
	{
		for (vector<string>::const_iterator itV = valueList.begin(); itV != valueList.end(); itV++)
		{
			Card card(*itV, *itS);
			cardList_.push_back(card); // Adding the card to the deck
		}
	}
	/*With this method, each card of the deck is unique*/
}

vector<string> Deck::SuitList()
{
	vector<string> suitList = { "spade", "club", "heart", "diamond" }; // List of each (4) suits in a deck of cards
	return suitList;
}

vector<string> Deck::ValueList()
{
	vector<string> valueList(13); // From 1 to 13 : from 1 to King (list with all the values a card can get) 
	for (unsigned int i = 1; i < 11; i++) valueList[i - 1] = to_string(i); // Let's full the list to 10 value
	valueList[10] = "Jack";
	valueList[11] = "Queen";
	valueList[12] = "King";

	return valueList;
}

void Deck::PrintDeck()
{
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) (*it).PrintCard();
}