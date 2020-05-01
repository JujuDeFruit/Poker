#include"Definitions.h"
#include<iostream>
#include<vector>
#include<string>

Deck::Deck()
{
	const vector<int> numberList = Card::NumberList();
	const vector<string> suitList = Card::SuitList();

	/* This part will create the 52 cards deck. Instead of settings all cards manually, we decided to create the deck from the previous lists : NumberList and SuitList*/
	for (vector<string>::const_iterator itS = suitList.begin(); itS != suitList.end(); itS++)
	{
		for (vector<int>::const_iterator itN = numberList.begin(); itN != numberList.end(); itN++)
		{
			Card card(*itN, *itS);
			cardList_.push_back(card); // Adding the card to the deck
		}
	}
	/*With this method, each card of the deck is unique*/
}

void Deck::PrintDeck()
{
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) (*it).PrintCard();
}