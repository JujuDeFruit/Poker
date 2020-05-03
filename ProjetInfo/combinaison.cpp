#include"card_definitions.h"
#include<iostream>

using namespace std;

const int MAX_CARDS_COMBINAISON = 5;

/* Return the highest combinaison possible with  player's hand and the river. */
Combinaison::Combinaison(Deck hand, Deck river) {
	Deck deck(hand);
	deck.GetCardList()->insert(deck.End(), river.Begin(), river.End());
	//deck.SortCardListByValue();

	if (RoyalFlush(deck).isCombinaison_ && !RoyalFlush(river).isCombinaison_) {
		combinaisonComposition_ = RoyalFlush(deck);
	}
	/*else if (StraightFlush(sortByValue).isCombinaison_ && StraightFlush(sortByValue).onCards_.back() != StraightFlush(river).onCards_.back()) {
		combinaisonComposition_ = StraightFlush(sortByValue);
	}*/
	else if (Quads(deck).isCombinaison_ && !Quads(river).isCombinaison_) {
		combinaisonComposition_ = Quads(deck);
	}
	else if (Fullhouse(deck).isCombinaison_ && !Fullhouse(river).isCombinaison_) {
		combinaisonComposition_ = Fullhouse(deck);
	}
	else if (Flush(deck).isCombinaison_ && !Flush(river).isCombinaison_) {
		combinaisonComposition_ = Flush(deck);
	}
	else if (Trips(deck).isCombinaison_ && !Trips(river).isCombinaison_) {
		combinaisonComposition_ = Trips(deck);
	}
	else if (TwoPairs(deck).isCombinaison_ && !TwoPairs(river).isCombinaison_) {
		combinaisonComposition_ = TwoPairs(deck);
	}
	else if (Pair(deck).isCombinaison_ && !Pair(river).isCombinaison_) {
		combinaisonComposition_ = Pair(deck);
	}
	else {
		combinaisonComposition_ = HighCard(hand);
	}
	for (vector<Card>::iterator it = combinaisonComposition_.onCards_.begin(); it != combinaisonComposition_.onCards_.end(); it++) (*it).PrintCard();
	system("pause");
}

// Check combinaison region

/* This method return the composition of a royale pair combinaison. */
CombinaisonComposition Combinaison::RoyalFlush(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	vector<string> suitList = Deck::SuitList();
	for (vector<string>::iterator itSL = suitList.begin(); itSL != suitList.end(); itSL++)
	{
		if (count(deck.Begin(), deck.End(), Card("10", *itSL))
			&& count(deck.Begin(), deck.End(), Card("Jack", *itSL))
			&& count(deck.Begin(), deck.End(), Card("Queen", *itSL))
			&& count(deck.Begin(), deck.End(), Card("King", *itSL))
			&& count(deck.Begin(), deck.End(), Card("1", *itSL)))
		{
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::straightFlush;
			combinaisonComposition.onCards_ = { Card("10", *itSL) , Card("Jack", *itSL) , Card("Queen", *itSL) , Card("King", *itSL) , Card("1", *itSL) };
			break;
		}
	}
	return combinaisonComposition;
}

/* This method return the composition of a straight flush combinaison. */
/* Méthode pas parfaite. A refaire.*/
CombinaisonComposition Combinaison::StraightFlush(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	Deck river(deck.Begin() + 2, deck.End());
	river.SortCardListByValue();

	Deck cardList(deck); 
	cardList.SortCardListByValue();

	vector<Card> extractedList;

	vector<string> suitList = Deck::SuitList();
	for (vector<string>::iterator it = suitList.begin(); it != suitList.end(); it++) {
		int count = cardList.Count(*it);
		if (count >= MAX_CARDS_COMBINAISON) {
			Deck extractedList(cardList); 
			cardList.ExtractCards(*it);
		}
	}
	if (extractedList.size() >= MAX_CARDS_COMBINAISON) {
		int diffBetweenSizeAndNumberOfCards = extractedList.size() - MAX_CARDS_COMBINAISON; // The number of cards to compare is 5.
																		// Then if the extracted list is bigger than five, the program has at least two straight to compare and has to choose the stronger one  (with the highest card).
		for (unsigned int i = diffBetweenSizeAndNumberOfCards; i >= 0; i--) {
			Deck list(extractedList.begin() + i, extractedList.end() + (i - diffBetweenSizeAndNumberOfCards));
			if (list.IsStraight()) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::straightFlush;
				combinaisonComposition.onCards_ = *list.GetCardList();
				break;
			}
		}
	}

	return combinaisonComposition;
}
/* Méthode pas parfaite. A refaire.*/

/* This method return the composition of a quads combinaison. */
CombinaisonComposition Combinaison::Quads(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	vector<Card> quads;

	vector<string> valueList = Deck::ValueList();
	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countQuads = 0;
		quads.clear();
		for (vector<Card>::iterator itCard = deck.Begin(); itCard != deck.End(); itCard++) {
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

/* This method return the composition of a fullhouse combinaison. */
CombinaisonComposition Combinaison::Fullhouse(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	CombinaisonComposition trips = Trips(deck);

	vector<Card> cardsCombinaison(trips.onCards_);

	if (trips.isCombinaison_) {
		Deck deckListWithoutTrips(deck);
		deckListWithoutTrips.EraseCards(trips.onCards_.front().GetNumber());
		CombinaisonComposition pair = Pair(deckListWithoutTrips);
		if (pair.isCombinaison_) {
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::fullhouse;
			cardsCombinaison.insert(cardsCombinaison.end(), pair.onCards_.begin(), pair.onCards_.end());
			combinaisonComposition.onCards_ = cardsCombinaison;
		}
	}

	return combinaisonComposition;
}

/* This method return the composition of a flush combinaison. */
CombinaisonComposition Combinaison::Flush(Deck deck) {
	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	const vector<string> suitList = Deck::SuitList();
	for (vector<string>::const_iterator it = suitList.begin(); it != suitList.end(); it++) {
		if (deck.Count(*it) >= MAX_CARDS_COMBINAISON) {
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::flush;
			Deck extractedCards(deck);
			extractedCards.ExtractCards(*it);
			extractedCards.SortCardListByValue();
			combinaisonComposition.onCards_ = *extractedCards.GetCardList();
		}
	}

	Deck river(deck.Begin() + 2, deck.End()); //
	// If there are more than 5 cards in the flush choose these ones in the player hand
	if (combinaisonComposition.isCombinaison_ && combinaisonComposition.onCards_.size() > MAX_CARDS_COMBINAISON) {
		river.SortCardListByValue();
		int cardsToErase = combinaisonComposition.onCards_.size() - MAX_CARDS_COMBINAISON; // 6 cards, in the flush, we need to remove one card that belong to the river and to the composition.
		int counter = 0;
		for (vector<Card>::iterator it = river.Begin(); it != river.End(); it++) {
			if ((*it).GetSuit() == combinaisonComposition.onCards_.front().GetSuit()) {
				Deck combinaisonDeck(combinaisonComposition.onCards_);
				combinaisonDeck.EraseCards(Deck::ToDeck({ *it }));
				combinaisonComposition.onCards_ = *combinaisonDeck.GetCardList();
				counter += 1;
			}
			if (counter == cardsToErase) break;
		}
	}

	return combinaisonComposition;
}

/* This method return the composition of a trips combinaison. */
CombinaisonComposition Combinaison::Trips(Deck deck) {
	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	vector<Card> cardList;

	vector<string> valueList = Deck::ValueList();
	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countTrips = 0;
		cardList.clear();
		for (vector<Card>::iterator itCard = deck.Begin(); itCard != deck.End(); itCard++) {
			if ((*itCard).GetNumber() == *itString) {
				countTrips += 1;
				cardList.push_back(*itCard);
			}
			if (countTrips == 3) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::trips;
				combinaisonComposition.onCards_ = cardList;
				break;
			}
		}
		if (countTrips == 3) break;
	}

	return combinaisonComposition;
}

/* This method return the composition of a two pairs combinaison. */
CombinaisonComposition Combinaison::TwoPairs(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	bool pair1 = false;
	string value;
	Deck cardFirstPair, cardSecondPair;

	vector<string> valueList = Deck::ValueList();
	/* Reverse he value list to detect the highest pairs first. */
	reverse(valueList.begin(), valueList.end());

	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countPair = 0;
		cardFirstPair.GetCardList()->clear();
		for (vector<Card>::iterator itCard = deck.Begin(); itCard != deck.End(); itCard++) {
			if ((*itCard).GetNumber() == *itString) {
				countPair += 1;
				cardFirstPair.GetCardList()->push_back(*itCard);
			}
			if (countPair == 2) {
				pair1 = true;
				value = (*itCard).GetNumber();
				break;
			}
		}
		if (pair1) break;
	}

	bool pair2 = false;

	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countPair = 0;
		cardSecondPair.GetCardList()->clear();
		for (vector<Card>::iterator itCard = deck.Begin(); itCard != deck.End(); itCard++) {
			const string cardValue = (*itCard).GetNumber();
			if (cardValue == *itString && cardValue != value) {
				countPair += 1;
				cardSecondPair.GetCardList()->push_back(*itCard);
			}
			if (countPair == 2) {
				pair2 = true;
				break;
			}
		}
		if (pair2) break;
	}

	if (pair1 && pair2) {
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::twoPairs;
		Deck pairs(cardFirstPair);
		pairs.GetCardList()->insert(pairs.End(), cardSecondPair.Begin(), cardSecondPair.End());
		combinaisonComposition.onCards_ = *pairs.GetCardList();
	}

	return combinaisonComposition;
}

/* This method return the composition of a pair combinaison. */
CombinaisonComposition Combinaison::Pair(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	vector<Card> cardList;

	vector<string> valueList = Deck::ValueList();
	for (vector<string>::iterator itString = valueList.begin(); itString != valueList.end(); itString++) {
		int countPair = 0;
		cardList.clear();
		for (vector<Card>::iterator itCard = deck.Begin(); itCard != deck.End(); itCard++) {
			if ((*itCard).GetNumber() == *itString) {
				countPair += 1;
				cardList.push_back(*itCard);
			}
			if (countPair == 2) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::pair;
				combinaisonComposition.onCards_ = cardList;
				break;
			}
		}
		if (countPair == 2) break;
	}

	return combinaisonComposition;
}

/* Return the highest card of the deck. It is not considered as combinaison. */
CombinaisonComposition Combinaison::HighCard(Deck deck) {
	CombinaisonComposition combinaisonComposition;

	combinaisonComposition.isCombinaison_ = false;
	combinaisonComposition.combinaison_ = combinaisonTypes::highCard;

	if (Card::ConvertCardValueToNumber(deck[0].GetNumber()) > Card::ConvertCardValueToNumber(deck[1].GetNumber()))
		combinaisonComposition.onCards_ = { deck[0] };
	else
		combinaisonComposition.onCards_ = { deck[1] };

	return combinaisonComposition;
}