#include"card_definitions.h"
#include<string>
#include<vector>
#include <algorithm>    // std::random_shuffle, reverse
#include <ctime>        // std::time
#include<iostream>

using namespace std;

#pragma region Functions

/* 
 * Function returns a random number between 0 and i - 1 
 * @param i : range of the random number.
 */
int RandomNumber(int i) { return rand() % i; };

#pragma endregion

#pragma region Constructor

/*
 * Constructor of the begin deck.
 * @param createDeck : Distinguish declaration to construction.
 */
BeginDeck::BeginDeck(bool createDeck) {

	if (createDeck) {
		const vector<string> valueList = Deck::ValueList();
		const vector<string> suitList = Deck::SuitList();

		/* This part will create the 52 cards deck. Instead of settings all cards manually, we decided to create the deck from the previous lists : NumberList and SuitList*/
		for each (string suit in suitList) {
			for each (string value in valueList) {
				Card card(value, suit);
				cardList_.push_back(card); // Adding the card to the deck
			}
		}
		isShaked_ = false;
		/* Each card of the deck is unique. */
	}
}

#pragma endregion

#pragma region Methods to interact with the deck

/*
 * This method permit to draw one or many cards. The number of cards to draw is given in parameters.
 * Each picked card is draw from the end of the vector (Ex : for a full deck, 2 cards picked are deck[51] and deck [50], the card deck[52] is the burned cards). 
 * @param number : number of card to pick from the deck.
*/
Deck BeginDeck::DrawCard(int number)
{
	/* If the deck is empty, return void. */
	if (cardList_.size() <= 1) {
		cout << "End of the deck." << endl;
		return {};
	}

	/* In poker rules, 5 cards max can be drawn at the same time. */
	number = number > 5 ? 5 : number;

	vector<Card> cards; // This entity is the collection of picked cards will be returned. 

	if (!isShaked_) ShakeDeck(); // Shake the deck if this one wasn't shaked
	cardList_.pop_back(); // Burn the first card of the deck before to pick a card.

	int deckSize = cardList_.size(); // Initiale size of the deck.
	for (unsigned int i = deckSize - 1; i >= deckSize - number; i--) {
		cards.push_back(cardList_[i]); // Add the card to the list of card to be returned.
		cardList_.pop_back(); // Remove the card from the deck.
	}

	/* Reverse the vector because this one was filled upside down. */
	reverse(cards.begin(), cards.end());

	return ToDeck({ cards });
}

/* 
 * This method shake the deck. Initially, the deck isn't shaked. 
 */
void BeginDeck::ShakeDeck()
{
	srand(unsigned(time(0))); // Modifying the seed depending on time (to always be different).
	random_shuffle(cardList_.begin(), cardList_.end(), RandomNumber); // Shaking the deck depending on the seed.
	isShaked_ = true;
}

/* 
 * Convert a deck to a BeginDeck.
 * 
 * @param Deck : deck to convert.
 */
BeginDeck BeginDeck::ToBeginDeck(Deck deck) {
	BeginDeck callback;
	callback.cardList_ = *deck.GetCardList();
	callback.isShaked_ = true;
	return callback;
}

#pragma endregion