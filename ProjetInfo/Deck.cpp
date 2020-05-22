#pragma region Includes

#include"card_definitions.h"
#include<iostream>
#include <algorithm>    // std::random_shuffle, reverse
#include<vector>
#include<string>
#include "sstream"

#pragma endregion


#pragma region Constructors of the Deck class
Deck::Deck(vector<Card> cards) { cardList_ = cards; }

Deck::Deck(vector<Card>::iterator begin, vector<Card>::iterator end) {
	vector<Card> cards(begin, end);
	cardList_ = cards;
}

Deck::Deck(const Deck& deck) :Deck(deck.cardList_) {}

#pragma endregion


#pragma region Overdefinition

/* Overdefinition od + operator to fit to deck class. */
Deck Deck::operator+(Deck deck) {
	Deck d(*this);
	d.cardList_.insert(d.End(), deck.Begin(), deck.End());
	return d;
}

#pragma endregion


#pragma region Basic methods

/* Method converts a collection of cards into a deck. */
Deck Deck::ToDeck(vector<Card> cards) {
	Deck deck(cards);
	return deck;
}

/* Print the cards of the whole deck. */
void Deck::PrintDeck()
{
	for each (Card card in cardList_) card.PrintCard();
}

#pragma endregion


#pragma region Suits and Values

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

/* This method return all suits a card can have */
vector<string> Deck::SuitList()
{
	vector<string> suitList = { "spade", "club", "heart", "diamond" }; // List of each (4) suits in a deck of cards
	return suitList;
}

/* Sort the card by values of cards. */
void Deck::SortCardListByValue() {

	vector<Card> sortByValue; // The sorted cards to return.

	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++)
	{
		if (sortByValue.empty()) // Push the first card 
			sortByValue.push_back(*it);
		else {
			int value = Card::ConvertCardValueToNumber((*it).GetValue());
			if (Card::ConvertCardValueToNumber(sortByValue.back().GetValue()) <= value) {	// If the current card value is higher than the last value of the sorted list, 
																							// then push the current card at the back of the sorted list.
				sortByValue.push_back(*it);
			}
			else {
				for (vector<Card>::iterator sBV = sortByValue.begin(); sBV != sortByValue.end(); sBV++) {
					int valueSBV = Card::ConvertCardValueToNumber((*sBV).GetValue());

					if (valueSBV > value) { // Check the position to insert the current card. 
						sortByValue.insert(sBV, *it);
						break;
					}
				}
			}
		}
	}

	cardList_ = sortByValue; // Return the sorted list.
}

/* Gather same suit cards. */
void Deck::SortCardListBySuit() {
	vector<Card> sortedList;
	const vector<string> suitList = Deck::SuitList();
	for (vector<string>::const_iterator it = suitList.begin(); it != suitList.end(); it++) {
		for (vector<Card>::iterator itCard = cardList_.begin(); itCard != cardList_.end(); itCard++) {
			if ((*itCard).GetSuit() == *it) sortedList.push_back(*itCard);
		}
	}

	cardList_ = sortedList;
}

#pragma endregion


#pragma region Extract and Erase cards

/* Extract a subvector all cards contain string key. */
void Deck::ExtractCards(string key) {
	vector<Card> extractedList; // The subvector to return.
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) {
		if ((*it).GetValue() == key || (*it).GetSuit() == key) {	// If the current card contains the key as a number or as a suit
																	// then add it to the list to return.
			extractedList.push_back(*it);
		}
	}
	cardList_ = extractedList;
}

/* Return a subvector which does not contain a card containning the key. */
void Deck::EraseCards(string key) {
	vector<Card> returnedList;
	Deck list(cardList_); // All the cards from the list that contain the key.
	list.ExtractCards(key);

	for (vector<Card>::iterator it = Begin(); it != End(); it++) {
		if (!count(list.Begin(), list.End(), *it)) { // If the current card is not contained in the extracted list (contains all the card to not return).
			returnedList.push_back(*it);
		}
	}

	cardList_ = returnedList;
}

/* Method that erase cards from a vector. */
void Deck::EraseCards(Deck cardsToErase) {
	Deck cards = cardList_;
	for (vector<Card>::iterator it = cardsToErase.Begin(); it != cardsToErase.End(); it++) {
		vector<Card>::iterator itCard = find(cards.Begin(), cards.End(), *it);
		if (itCard != cards.End()) {
			cards.GetCardList()->erase(itCard);
		}
	}
	cardList_ = *cards.GetCardList();
}

#pragma endregion


#pragma region About deck

/* Method returns true if the current deck contains all the cards of the deck in parameter. */
bool Deck::Has(Deck deck) {
	bool has = true;
	for each(Card card in deck.cardList_) {
		if (!Count(card)) {
			has = false;
			break;
		}
	}
	return has;
}

/* Count the number of cards that contains the key. */
int Deck::Count(string key) {
	int count = 0;
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) {
		if ((*it).GetValue() == key || (*it).GetSuit() == key) count += 1; // If the current card from the collection contains the key as number or suit, then increase count.
	}
	return count;
}

/* Count the number of cards of the deck that correspond to the card in parameter. */
int Deck::Count(Card c) {
	int counter = 0;
	for each(Card card in cardList_) {
		if (card == c) counter += 1;
	}
	return counter;
}

/* Check if a collection of cards is straight or not. */
bool Deck::IsStraight() {
	bool isStraight = true;
	for (unsigned int i = 0; i < 4; i++) {
		// The boolean is true while a card value equals the next card value less 1.
		if (Card::ConvertCardValueToNumber(cardList_[i].GetValue()) != Card::ConvertCardValueToNumber(cardList_[i + 1].GetValue()) - 1) {
			isStraight = false;
			break;
		}
	}
	if (Count("1") && Count("2") && Count("3") && Count("4") && Count("5")) isStraight = true;
	return isStraight;
}

/* Method that remove same multiple cards value. */
void Deck::RemoveSameValueCards() {
	Deck deck(cardList_);
	vector<string> values;

	/* Returned list containnig any card with the same value. */
	Deck uniqueValueList;

	/* For each value a card can have. */
	vector<string> valuesList = Deck::ValueList();
	for each (string val in valuesList) {
		Deck deckExtracted(cardList_);
		/* If a card of the deck has the value. */
		if (Count(val)) {
			/* Add a card to the returned list. */
			deckExtracted.ExtractCards(val);
			uniqueValueList.GetCardList()->push_back(deckExtracted.GetCardList()->front());
		}
	}

	*this = uniqueValueList;
}

/* Method counts the number of combinaison in a deck of cards. */
int Deck::CountCombinaison(combinaisonTypes e) {

	int nbCards;

	switch (e) {
	case combinaisonTypes::pair: nbCards = 2;	// 2 cards are needed to have a trip (Max 3).
		break;
	case combinaisonTypes::trips: nbCards = 3;	// 3 cards are needed to have a trip (Max 2).
		break;
	case combinaisonTypes::quads: nbCards = 4;	// 4 cards are needed to have a quad (Max 1). 
		break;
	default: return (int)-1; // Wrong enum value. This method can only count pairs, trips and quads.
	}

	/* The number of pairs. */
	int counter = 0;
	/* Vector to store pair value already checked. */
	vector<string> values;

	for each(Card card in cardList_) {
		string val = card.GetValue();
		/*	Count the number of cards having the same value & searching current card value in the values vector.
			If the value is not already in the vector and the card appears two times in the deck it is an unchecked pair.*/
		if ( Count(val) == nbCards && find(values.begin(), values.end(), val) == values.end() ) {
			/* Store the value to not increment counter for the other card of the pair. */
			values.push_back(card.GetValue());
			counter += 1;
		}
	}

	return counter;
}

/* 
 * Return true if two deck are equivalent : they have the same size and the same card in a different order. 
 * @param deck : deck to try equivalence.
 */
bool Deck::Equivalent(Deck deck) {
	bool equivalent = true;
	
	/* If the two decks have not the same size, they are not equivalent. */
	if (cardList_.size() != deck.GetCardList()->size()) return false;

	/* If the current deck and the deck in parameter has not the same cards then equivalent = false. */
	for each(Card card in deck.cardList_) {
		if (find(cardList_.begin(), cardList_.end(), card) == cardList_.end()) {
			equivalent = false;
			break;
		}
	}

	/* If the two deck are equal, then they are equivalent. */
	if (deck == *this) equivalent = true;
	/* If the two decks have the same cards and the same size, then they are equivalent. */
	else if (deck.GetCardList()->size() == cardList_.size() && equivalent) equivalent = true;
	/* Else, they are not equivalent. */
	else equivalent = false;

	return equivalent;
}

/*
 * Concat 2 decks, and return the concatenation. 
 * @param deck : deck to add at the end of the object.
 */
Deck Deck::Concat(Deck deck) {
	Deck d(cardList_);
	d.GetCardList()->insert(d.End(), deck.Begin(), deck.End());
	return d;

}

/*
 * Serialize cards to write them in files.
 * @param deck : deck to serialize.
 */
vector<string> Deck::SerializeCards(Deck deck) {
	vector<string> cards;
	for each (Card card in *deck.GetCardList()) {
		string json = card.GetValue() + "+" + card.GetSuit();
		cards.push_back(json);
	}
	return cards;
}

/*
 * Deserialize cards and turn in into a deck.
 * @param cards : list to deserialize.
 */
Deck Deck::DeserializeCards(vector<string> cards) {
	Deck deck;
	for each (string cardJson in cards) {
		size_t pos = cardJson.find("+");
		cardJson.replace(pos, 1, " ");
		string value, suit;
		istringstream iss(cardJson);
		iss >> value >> suit;
		Card card(value, suit);
		deck.GetCardList()->push_back(card);
	}
	return deck;
}

#pragma endregion