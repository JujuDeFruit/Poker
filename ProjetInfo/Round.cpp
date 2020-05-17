#include "menu_definitions.h"
#include "game_definitions.h"
#include "card_definitions.h"
#include <iostream>
using namespace std;

#pragma region Constructor

/* Constructor of the class round. Create the deck and shake it. */
Round::Round() {
	BeginDeck beginDeck;
	beginDeck.ShakeDeck();
	beginDeck_ = beginDeck;
}

#pragma endregion

#pragma region Player actions

void Round::Follow() {
	cout << " test on suit " << endl;

}

#pragma endregion

#pragma region River Evolution

/* Reveal the first 3 cards of the river. */
void Round::Flop() {
	/* If any cards are face up, then reveal the 3 first ones. */
	if (!river_.GetCardList()->size()) river_ += beginDeck_.DrawCard(3);
}

/* Reveal the 4th card of the river. */
void Round::Turn() {
	if (beginDeck_.GetCardList()->size() == 3) river_ += beginDeck_.DrawCard();
}

/* Reveal the last card of the river. */
void Round::River() {
	if (river_.GetCardList()->size() == 4) river_ += beginDeck_.DrawCard();
}

#pragma endregion

#pragma region Usefull Methods

/* Print the cards in the river. */
void Round::PrintRiver() {
	river_.PrintDeck();

}

#pragma endregion
