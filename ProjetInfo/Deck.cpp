#include"card_definitions.h"
#include<iostream>
#include <algorithm>    // std::random_shuffle, reverse
#include<vector>
#include<string>

/* Constructor of the class Deck. */
Deck::Deck(vector<Card> cards) { cardList_ = cards; }

Deck::Deck(vector<Card>::iterator begin, vector<Card>::iterator end) {
	vector<Card> cards(begin, end);
	cardList_ = cards;
}

Deck::Deck(const Deck& deck):Deck(deck.cardList_) {}

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

/* Method converts a collection of cards into a deck. */
Deck Deck::ToDeck(vector<Card> cards) {
	Deck deck(cards);
	return deck;
}

/* Print the cards of the whole deck. */
void Deck::PrintDeck()
{
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) (*it).PrintCard();
}

/* Sort the card by values of cards. */
void Deck::SortCardListByValue() {

	vector<Card> sortByValue; // The sorted cards to return.

	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++)
	{
		if (sortByValue.empty()) // Push the first card 
			sortByValue.push_back(*it);
		else {
			int value = Card::ConvertCardValueToNumber((*it).GetNumber());
			if (Card::ConvertCardValueToNumber(sortByValue.back().GetNumber()) <= value) {	// If the current card value is higher than the last value of the sorted list, 
																							// then push the current card at the back of the sorted list.
				sortByValue.push_back(*it);
			}
			else {
				for (vector<Card>::iterator sBV = sortByValue.begin(); sBV != sortByValue.end(); sBV++) {
					int valueSBV = Card::ConvertCardValueToNumber((*sBV).GetNumber());

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

/* Count the number of cards that contains the key. */
int Deck::Count(string key) {
	int count = 0;
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) {
		if ((*it).GetNumber() == key || (*it).GetSuit() == key) count += 1; // If the current card from the collection contains the key as number or suit, then increase count.
	}
	return count;
}

/* Check if a collection of cards is straight or not. */
bool Deck::IsStraight() {
	bool isStraight = true;
	for (unsigned int i = 0; i < 4; i++) {
		// The boolean is true while a card value equals the next card value less 1.
		if (Card::ConvertCardValueToNumber(cardList_[i].GetNumber()) != Card::ConvertCardValueToNumber(cardList_[i + 1].GetNumber()) - 1) {
			isStraight = false;
			break;
		}
	}
	return isStraight;
}

/* Extract a subvector all cards contain string key. */
void Deck::ExtractCards(string key) {
	vector<Card> extractedList; // The subvector to return.
	for (vector<Card>::iterator it = cardList_.begin(); it != cardList_.end(); it++) {
		if ((*it).GetNumber() == key || (*it).GetSuit() == key) {	// If the current card contains the key as a number or as a suit
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