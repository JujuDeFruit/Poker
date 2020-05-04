#include"card_definitions.h"
#include<string>
#include<vector>
#include <algorithm>    // std::random_shuffle, reverse
#include <ctime>        // std::time

/* Function returns a random number between 0 and i - 1 */
int RandomNumber(int i) { return rand() % i; };

BeginDeck::BeginDeck() {
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
	isShaked_ = false;
	/*With this method, each card of the deck is unique*/
}

/* This method permit to draw one or many cards. The number of cards to draw is given in parameters.
Each picked card is draw from the end of the vector (Ex : for a full deck, 2 cards picked are deck[52] and deck [51]. */
vector<Card> BeginDeck::DrawCard(int number)
{
	/* In poker rules, 5 cards max can be drawn at the same time. */
	number = number > 5 ? 5 : number;

	vector<Card> cards; // This entity is the collection of picked cards will be returned. 

	if (!!isShaked_) // Shake the deck if this one wasn't shaked
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
void BeginDeck::ShakeDeck()
{
	srand(unsigned(time(0))); // Modifying the seed depending on time (to always be different).
	random_shuffle(cardList_.begin(), cardList_.end(), RandomNumber); // Shaking the deck depending on the seed.
	isShaked_ = true;
}