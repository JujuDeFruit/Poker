#include"card_definitions.h"
#include<iostream>

using namespace std;

/* Return the highest combinaison possible with  player's hand and the river. */
Combinaison::Combinaison(vector<Card> hand, vector<Card> river) {
	vector<Card> cards(hand);
	cards.insert(cards.end(), river.begin(), river.end());

	vector<Card> sortByValue = SortCardListByValue(cards);

	if (RoyalFlush(sortByValue).isCombinaison_ && !RoyalFlush(river).isCombinaison_) combinaisonComposition_ = RoyalFlush(sortByValue);
	else if (Quads(sortByValue).isCombinaison_ && !Quads(river).isCombinaison_)	combinaisonComposition_ = Quads(sortByValue);
	else if (Fullhouse(sortByValue).isCombinaison_ && !Fullhouse(river).isCombinaison_) combinaisonComposition_ = Fullhouse(sortByValue);
	/*else if (TwoPairs(sortByValue) && !TwoPairs(river)) { cout << "Two Pairs" << endl; combinaison_ = combinaisonTypes::twoPairs; }*/
	else if (Pair(sortByValue).isCombinaison_ && !Pair(river).isCombinaison_) 
	{ 
		combinaisonComposition_ = Pair(sortByValue);
	}
	else {
		combinaisonComposition_ = HighCard(hand);
		for (vector<Card>::iterator it = combinaisonComposition_.onCards_.begin(); it != combinaisonComposition_.onCards_.end(); it++) (*it).PrintCard();
	}

	system("pause");
}

int Combinaison::ConvertCardValueToNumber(string cardValue) {

	int val = 0;
	if (cardValue == "King") {
		val = 13;
	}
	else if (cardValue == "Queen") {
		val = 12;
	}
	else if (cardValue == "Jack") {
		val = 11;
	}
	else {
		val = stoi(cardValue);
	}
	return val;
}

vector<Card> Combinaison::SortCardListByValue(vector<Card> cards) {

	vector<Card> sortByValue;

	for (vector<Card>::iterator it = cards.begin(); it != cards.end(); it++)
	{
		if (sortByValue.empty())
			sortByValue.push_back(*it);
		else {
			int value = ConvertCardValueToNumber((*it).GetNumber());
			if (ConvertCardValueToNumber(sortByValue.back().GetNumber()) <= value) {
				sortByValue.push_back(*it);
			}
			else {
				for (vector<Card>::iterator sBV = sortByValue.begin(); sBV != sortByValue.end(); sBV++) {
					int valueSBV = ConvertCardValueToNumber((*sBV).GetNumber());

					if (valueSBV > value) {
						sortByValue.insert(sBV, *it);
						break;
					}
				}
			}
		}
	}

	return sortByValue;
}

CombinaisonComposition Combinaison::RoyalFlush(vector<Card> cards) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	vector<string> suitList = Deck::SuitList();
	for (vector<string>::iterator itSL = suitList.begin(); itSL != suitList.end(); itSL++)
	{
		if (count(cards.begin(), cards.end(), Card("9", *itSL))
			&& count(cards.begin(), cards.end(), Card("10", *itSL))
			&& count(cards.begin(), cards.end(), Card("Jack", *itSL))
			&& count(cards.begin(), cards.end(), Card("Queen", *itSL))
			&& count(cards.begin(), cards.end(), Card("King", *itSL)))
		{
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::straightFlush;
			combinaisonComposition.onCards_ = { Card("9", *itSL) , Card("10", *itSL) , Card("Jack", *itSL) , Card("Queen", *itSL) , Card("King", *itSL) };
			break;
		}
	}
	return combinaisonComposition;
}

//CombinaisonComposition Combinaison::StraightFlush(vector<Card> cards) {
//
//}

CombinaisonComposition Combinaison::Quads(vector<Card> cards) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	vector<Card> quads;

	vector<string> valueList = Deck::ValueList();
	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countQuads = 0;
		quads.clear();
		for (vector<Card>::iterator itCard = cards.begin(); itCard != cards.end(); itCard++) {
			if ((*itCard).GetNumber() == *itString) {
				countQuads += 1;
				quads.push_back(*itCard);
			}
			if (countQuads == 4)
			{
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::quads;
				combinaisonComposition.onCards_ = quads;
				break;
			}
		}
	}
	return combinaisonComposition;
}

CombinaisonComposition Combinaison::Fullhouse(vector<Card> cards) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	vector<Card> cardListTrips;

	bool trips = false;
	string value;

	vector<string> valueList = Deck::ValueList();
	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countTrips = 0;
		cardListTrips.clear();
		for (vector<Card>::iterator itCard = cards.begin(); itCard != cards.end(); itCard++) {
			if ((*itCard).GetNumber() == *itString) {
				countTrips += 1;
				cardListTrips.push_back(*itCard);
			}
			if (countTrips == 3) {
				trips = true;
				value = (*itCard).GetNumber();
				break;
			}
		}
		if (countTrips == 3)
			break;
	}

	vector<Card> cardListPair;

	bool pair = false;

	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countPair = 0;
		cardListPair.clear();
		for (vector<Card>::iterator itCard = cards.begin(); itCard != cards.end(); itCard++) {
			if ((*itCard).GetNumber() == *itString && (*itCard).GetNumber() != value) {
				countPair += 1;
				cardListPair.push_back(*itCard);
			}
			if (countPair == 2) {
				pair = true;
				break;
			}
		}
	}

	if (pair && trips) {
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::fullhouse;
		vector<Card> cardsList(cardListTrips);
		cardsList.insert(cardsList.end(), cardListPair.begin(), cardListPair.end());
		combinaisonComposition.onCards_ = cardsList;
	}

	return combinaisonComposition;
}

//bool Combinaison::Flush(vector<Card> cards) {
//
//}
//
//bool Combinaison::Straight(vector<Card> cards) {
//
//}
//
//bool Combinaison::Trips(vector<Card> cards) {
//
//}

//bool Combinaison::TwoPairs(vector<Card> cards) {
//
//	bool pair1 = false;
//	string value;
//
//	vector<string> valueList = Deck::ValueList();
//	/* Reverse he value list to detect the highest pairs first. */
//	reverse(valueList.begin(), valueList.end());
//
//	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
//		int countTrips = 0;
//		for (vector<Card>::iterator itCard = cards.begin(); itCard != cards.end(); itCard++) {
//			if ((*itCard).GetNumber() == *itString) {
//				countTrips += 1;
//			}
//			if (countTrips == 2) {
//				pair1 = true;
//				value = (*itCard).GetNumber();
//			}
//		}
//	}
//
//	bool pair2 = false;
//
//	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
//		int countPair = 0;
//		for (vector<Card>::iterator itCard = cards.begin(); itCard != cards.end(); itCard++) {
//			const string cardValue = (*itCard).GetNumber();
//			if (cardValue == *itString && cardValue != value) {
//				countPair += 1;
//			}
//			if (countPair == 2) {
//				pair2 = true;
//			}
//		}
//	}
//
//	if (pair1 && pair2) return true;
//	else return false;
//}

/* This method return the composition of a pair combinaison. */
CombinaisonComposition Combinaison::Pair(vector<Card> cards) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;
	
	vector<Card> cardList;

	vector<string> valueList = Deck::ValueList();
	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countTrips = 0;
		cardList.clear();
		for (vector<Card>::iterator itCard = cards.begin(); itCard != cards.end(); itCard++) {
			if ((*itCard).GetNumber() == *itString) {
				countTrips += 1;
				cardList.push_back(*itCard);
			}
			if (countTrips == 2) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::pair;
				combinaisonComposition.onCards_ = cardList;
				break;
			}
		}
		if (countTrips == 2) break;
	}

	return combinaisonComposition;
}

/* Return the highest card of the deck. It is not considered as combinaison. */
CombinaisonComposition Combinaison::HighCard(vector<Card> hand) {
	CombinaisonComposition combinaisonComposition;

	combinaisonComposition.isCombinaison_ = false;
	combinaisonComposition.combinaison_ = combinaisonTypes::highCard;

	if (ConvertCardValueToNumber(hand[0].GetNumber()) > ConvertCardValueToNumber(hand[1].GetNumber())) 
		combinaisonComposition.onCards_ = { hand[0] };
	else 
		combinaisonComposition.onCards_ = { hand[1] };

	return combinaisonComposition;
}