#include"Definitions.h"
#include<iostream>
#include <algorithm>    // std::random_shuffle, reverse
#include <ctime>        // std::time
#include<vector>
#include<string>

/* Function returns a random number between 0 and i - 1 */
int RandomNumber(int i) { return rand() % i; };

/* Constructor of the class Deck, build the 52 cards */
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

/* This method return all suits a card can have */
vector<string> Deck::SuitList()
{
	vector<string> suitList = { "spade", "club", "heart", "diamond" }; // List of each (4) suits in a deck of cards
	return suitList;
}

/* This method return all values a card can have */
vector<string> Deck::ValueList()
{
	vector<string> valueList(13); // From 1 to 13 : from 1 to King (list with all the values a card can get) 
	for (unsigned int i = 1; i < 11; i++) valueList[i - 1] = to_string(i); // Let's full the list to 10 value string
	valueList[10] = "Jack";
	valueList[11] = "Queen";
	valueList[12] = "King";

	return valueList;
}

/* This method permit to draw one or many cards. The number of cards to draw is given in parameters. 
	Each picked card is draw from the end of the vector (Ex : for a full deck, 2 cards picked are deck[52] and deck [51]. */
vector<Card> Deck::DrawCard(int number)
{
	/* In poker rules, 5 cards max can be draw at the same time. */
	number = number > 5 ? 5 : number;

	vector<Card> cards; // This entity is the collection of picked cards will be returned. 

	if (!deckShaked_) // Shake the deck if this one wasn't shaked
		ShakeDeck();
	cardList_.pop_back(); // Burn the first card of the deck before to pick a card.
	
	int deckSize = cardList_.size(); // Initiale size of the deck.
	for (unsigned int i = deckSize - 1; i >= deckSize - number; i--)
	{
		cards.push_back(cardList_[i]); // Add the card to the list of card to be returned.
		cardList_.pop_back(); // Remove the card from the deck.
	}

	/* Reverse the vector because this one was filled upside down. */
	reverse(cards.begin(), cards.end());

	return cards;
}

/* This method shake the deck. Initially, the deck isn't shaked. */
void Deck::ShakeDeck()
{
	srand(unsigned(time(0))); // Modifying the seed depending on time (to always be different).
	random_shuffle(cardList_.begin(), cardList_.end(), RandomNumber); // Shaking the deck depending on the seed.
	deckShaked_ = true;
}

/* Print the cards of the whole deck. */
void Deck::PrintDeck()
{
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) (*it).PrintCard();
}