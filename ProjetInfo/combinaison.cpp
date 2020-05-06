#pragma region Includes

#include"card_definitions.h"
#include<iostream>

#pragma endregion

using namespace std;

#pragma region Const

const int MAX_CARDS_COMBINAISON = 5;
const int CARDS_HAND = 2;
const int CARDS_RIVER = 5;

#pragma endregion

#pragma region Constructor

/* Return the highest combinaison possible with  player's hand and the river. */
Combinaison::Combinaison(Deck hand, Deck river) {

	if (hand.GetCardList()->size() == CARDS_RIVER && river.GetCardList()->size() == CARDS_HAND)
	{
		Deck intermediate(hand);
		hand = river;
		river = intermediate;
	}

	/* Concat hand and river to create one same deck. First it is the hand, and then it is the river. */
	Deck deck(hand);
	deck.GetCardList()->insert(deck.End(), river.Begin(), river.End());

	if (RoyalFlush(deck).isCombinaison_) {
		combinaisonComposition_ = RoyalFlush(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (StraightFlush(deck).isCombinaison_) {
		combinaisonComposition_ = StraightFlush(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Quads(deck).isCombinaison_) {
		combinaisonComposition_ = Quads(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Fullhouse(deck).isCombinaison_) {
		combinaisonComposition_ = Fullhouse(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Flush(deck).isCombinaison_) {
		combinaisonComposition_ = Flush(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Straight(deck).isCombinaison_) {
		combinaisonComposition_ = Straight(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Trips(deck).isCombinaison_ && !Trips(river).isCombinaison_) {
		combinaisonComposition_ = Trips(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (TwoPairs(deck).isCombinaison_) {
		combinaisonComposition_ = TwoPairs(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Pair(deck).isCombinaison_ && !Pair(river).isCombinaison_) {
		combinaisonComposition_ = Pair(deck);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else {
		combinaisonComposition_ = HighCard(hand);
	}
	//combinaisonComposition_.cards_.PrintDeck();
	//system("pause");
}

#pragma endregion

#pragma region Check combinaison

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
			combinaisonComposition.cards_ = Deck::ToDeck({ Card("10", *itSL) , Card("Jack", *itSL) , Card("Queen", *itSL) , Card("King", *itSL) , Card("1", *itSL) });
			break;
		}
	}

	CombinaisonComposition riverCombinaison;
	if (deck.GetCardList()->size() == (CARDS_HAND + CARDS_RIVER)) {
		Deck river(deck.Begin() + 2, deck.End());
		riverCombinaison = RoyalFlush(river);
	}

	if (riverCombinaison.isCombinaison_ && combinaisonComposition.isCombinaison_ && riverCombinaison.cards_ == combinaisonComposition.cards_) {
		CombinaisonComposition nullCombinaison;
		nullCombinaison.isCombinaison_ = false;
		combinaisonComposition = nullCombinaison;
	}

	return combinaisonComposition;
}

/* This method return the composition of a straight flush combinaison. */
/* ===> Add straight for As. */
CombinaisonComposition Combinaison::StraightFlush(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

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
																							// Then if the extracted list is bigger than five, 
																							// the program has at least three straight to compare 
																							// and has to choose the stronger one (with the highest card).
		for (unsigned int i = diffBetweenSizeAndNumberOfCards; i >= 0; i--) {
			Deck list(extractedList.begin() + i, extractedList.end() + (i - diffBetweenSizeAndNumberOfCards));
			if (list.IsStraight()) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::straightFlush;
				combinaisonComposition.cards_ = *list.GetCardList();
				break;
			}
		}
	}

	CombinaisonComposition riverCombinaison;
	if (deck.GetCardList()->size() == (CARDS_HAND + CARDS_RIVER)) {
		Deck river(deck.Begin() + 2, deck.End());
		river.SortCardListByValue();
		riverCombinaison = StraightFlush(river);
	}

	if (riverCombinaison.isCombinaison_ && combinaisonComposition.isCombinaison_ && riverCombinaison.cards_ == combinaisonComposition.cards_) {
		CombinaisonComposition nullCombinaison;
		nullCombinaison.isCombinaison_ = false;
		combinaisonComposition = nullCombinaison;
	}

	return combinaisonComposition;
}

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
			if ((*itCard).GetValue() == *itString) {
				countQuads += 1;
				quads.push_back(*itCard);
			}
			if (countQuads == 4)
			{
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::quads;
				combinaisonComposition.cards_ = quads;
				break;
			}
		}

		CombinaisonComposition riverCombinaison;
		if (deck.GetCardList()->size() == (CARDS_HAND + CARDS_RIVER)) {
			Deck river(deck.Begin() + 2, deck.End());
			riverCombinaison = Quads(river);
		}

		if (riverCombinaison.isCombinaison_ && combinaisonComposition.isCombinaison_ && riverCombinaison.cards_ == combinaisonComposition.cards_) {
			CombinaisonComposition nullCombinaison;
			nullCombinaison.isCombinaison_ = false;
			combinaisonComposition = nullCombinaison;
		}
	}
	return combinaisonComposition;
}

/* This method return the composition of a fullhouse combinaison. */
CombinaisonComposition Combinaison::Fullhouse(Deck deck) {

	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	/* There is 1 trip max, else it is not a Fullhouse. */
	CombinaisonComposition trips = Trips(deck);

	if (trips.isCombinaison_) {
		/* Get a pair. */
		Deck deckListWithoutTrips(deck);
		deckListWithoutTrips.EraseCards(trips.cards_.GetCardList()->front().GetValue());
		CombinaisonComposition pair = Pair(deckListWithoutTrips);

		/* Get the highest pair as possible. If there is a trips, there are maximum 2 pairs. */
		deckListWithoutTrips.EraseCards(pair.cards_);
		CombinaisonComposition pair2 = Pair(deckListWithoutTrips);
		if (pair.isCombinaison_ && pair2.isCombinaison_ && pair.cards_.GetCardList()->front() < pair2.cards_.GetCardList()->front()) pair = pair2;

		if (pair.isCombinaison_) {
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::fullhouse;
			trips.cards_.GetCardList()->insert(trips.cards_.End(), pair.cards_.Begin(), pair.cards_.End());
			combinaisonComposition.cards_ = trips.cards_;
		}
	}

	CombinaisonComposition riverCombinaison;
	if (deck.GetCardList()->size() == (CARDS_HAND + CARDS_RIVER)) {
		Deck river(deck.Begin() + 2, deck.End());
		riverCombinaison = Fullhouse(river);
	}

	if (riverCombinaison.isCombinaison_ && combinaisonComposition.isCombinaison_ && riverCombinaison.cards_ == combinaisonComposition.cards_) {
		CombinaisonComposition nullCombinaison;
		nullCombinaison.isCombinaison_ = false;
		combinaisonComposition = nullCombinaison;
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
			combinaisonComposition.cards_ = *extractedCards.GetCardList();
		}
	}

	/* If there are more than 5 cards in the flush choose these ones in the player hand. */
	if (combinaisonComposition.isCombinaison_ && combinaisonComposition.cards_.GetCardList()->size() > MAX_CARDS_COMBINAISON) {
		Deck river(deck.Begin() + 2, deck.End()); //
		river.SortCardListByValue();
		int cardsToErase = combinaisonComposition.cards_.GetCardList()->size() - MAX_CARDS_COMBINAISON; // 6 cards, in the flush, we need to remove one card that belong to the river and to the composition.
		int counter = 0;
		for (vector<Card>::iterator it = river.Begin(); it != river.End(); it++) {
			if ((*it).GetSuit() == combinaisonComposition.cards_.GetCardList()->front().GetSuit()) {
				Deck combinaisonDeck(combinaisonComposition.cards_);
				combinaisonDeck.EraseCards(Deck::ToDeck({ *it }));
				combinaisonComposition.cards_ = *combinaisonDeck.GetCardList();
				counter += 1;
			}
			if (counter == cardsToErase) break;
		}
	}

	CombinaisonComposition riverCombinaison;
	if (deck.GetCardList()->size() == (CARDS_HAND + CARDS_RIVER)) {
		Deck river(deck.Begin() + 2, deck.End());
		riverCombinaison = Flush(river);
	}

	if (riverCombinaison.isCombinaison_ && combinaisonComposition.isCombinaison_ && riverCombinaison.cards_ == combinaisonComposition.cards_) {
		CombinaisonComposition nullCombinaison;
		nullCombinaison.isCombinaison_ = false;
		combinaisonComposition = nullCombinaison;
	}

	return combinaisonComposition;
}

/* This method return the composition of a flush combinaison. */
CombinaisonComposition Combinaison::Straight(Deck deck) {
	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = false;

	Deck river;
	if (deck.GetCardList()->size() == (CARDS_HAND + CARDS_RIVER)) {
		river = Deck(deck.Begin() + 2, deck.End());
	}

	Deck cardList(deck);
	cardList.RemoveSameValueCards();
	cardList.SortCardListByValue();

	int diffBetweenSizeAndNumberOfCards = cardList.GetCardList()->size() - MAX_CARDS_COMBINAISON;		// The number of cards to compare is 5.
																										// Then if the list is bigger than five, 
																										// the program has at least three straights to compare 

																										// and has to choose the stronger one (with the highest card).
	if (cardList.GetCardList()->size() >= 5) {
		for (int i = diffBetweenSizeAndNumberOfCards; i >= 0; i--) {
			Deck list(cardList.GetCardList()->begin() + i, cardList.GetCardList()->end() + (i - diffBetweenSizeAndNumberOfCards));
			if (list.IsStraight() && !Straight(river).isCombinaison_) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::straightFlush;
				combinaisonComposition.cards_ = *list.GetCardList();
				break;
			}
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
			if ((*itCard).GetValue() == *itString) {
				countTrips += 1;
				cardList.push_back(*itCard);
			}
			if (countTrips == 3) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::trips;
				combinaisonComposition.cards_ = cardList;
				break;
			}
		}
		if (countTrips == 3) break;
	}

	return combinaisonComposition;
}

/* This method return the composition of a two pairs combinaison. */
CombinaisonComposition Combinaison::TwoPairs(Deck deck) {

	/* if the input deck has 7 cards, it means it is player's hand + river. Then extract the river.*/
	Deck river;
	if (deck.GetCardList()->size() == CARDS_HAND + CARDS_RIVER) river = Deck(deck.Begin() + 2, deck.End());

	/* Pair number in the deck. */
	int pairNumber = deck.CountCombinaison(combinaisonTypes::pair);

	/* If the river as the same pair number than the hand more the river, then river contains all pairs or if the number of pair isn't at least 2. */
	if (river.CountCombinaison(combinaisonTypes::pair) == pairNumber || pairNumber < 2) {
		CombinaisonComposition nullCombinaison;
		return nullCombinaison;
	}

	/* Now, it is sure there are double pairs. */
	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.isCombinaison_ = true;
	combinaisonComposition.combinaison_ = combinaisonTypes::twoPairs;

	Deck pairList, d(deck);

	/* Create a vector containning just pairs. */
	for (int i = 0; i < pairNumber; i++) {
		CombinaisonComposition pair = Pair(d);
		pairList = pairList + pair.cards_;
		d.EraseCards(pair.cards_);
	}

	/* Sort pairs by value. */
	pairList.SortCardListByValue();

	/* If there are 3 pairs, then the program has to choose the strongest ones not contained by the river. */
	if (pairNumber == 3) {
		/* Restore the hand. */
		Deck hand(deck.Begin(), deck.Begin() + 2);

		/* The combinaison has always the strongest pair. Now choose the second one. */
		combinaisonComposition.cards_ = { pairList.End() - 2, pairList.End() };
		/* If the hand has a card of the middle pair or the hand has not cards belonging to two weekest pairs. */
		if (hand.Count(pairList[2]) || !(hand.Count(pairList[0]) || hand.Count(pairList[2]))) {
			combinaisonComposition.cards_.GetCardList()->insert(combinaisonComposition.cards_.Begin(), pairList.Begin() + 2, pairList.Begin() + 4);
		}
		/* If the hand has a card belonging to the weekest pair. */
		else {
			combinaisonComposition.cards_.GetCardList()->insert(combinaisonComposition.cards_.Begin(), pairList.Begin(), pairList.Begin() + 2);
		}
	}
	else {
		combinaisonComposition.cards_ = pairList;
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
			if ((*itCard).GetValue() == *itString) {
				countPair += 1;
				cardList.push_back(*itCard);
			}
			if (countPair == 2) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::pair;
				combinaisonComposition.cards_ = cardList;
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

	if (Card::ConvertCardValueToNumber(deck[0].GetValue()) > Card::ConvertCardValueToNumber(deck[1].GetValue()))
		combinaisonComposition.cards_ = Deck::ToDeck({ deck[0] });
	else
		combinaisonComposition.cards_ = Deck::ToDeck({ deck[1] });

	return combinaisonComposition;
}

#pragma endregion

#pragma region Compare combinaisons

bool Combinaison::operator==(Combinaison combinaison) {
	bool isEqual = false;

	combinaisonComposition_.cards_.SortCardListByValue();
	combinaison.combinaisonComposition_.cards_.SortCardListByValue();

	vector<string> valuesOfThis;
	for each (Card card in *combinaisonComposition_.cards_.GetCardList()) { valuesOfThis.push_back(card.GetValue()); }

	vector<string> valuesCombinaison;
	for each(Card card in *combinaison.combinaisonComposition_.cards_.GetCardList()) { valuesCombinaison.push_back(card.GetValue()); }

	if (combinaisonComposition_.combinaison_ == combinaison.combinaisonComposition_.combinaison_
		&&	valuesOfThis == valuesCombinaison
		&&	Card::ConvertCardValueToNumber(highestCard_.GetValue()) == Card::ConvertCardValueToNumber(combinaison.highestCard_.GetValue())) {
		cout << "Tie game !" << endl;
		isEqual = true;
	}
	return isEqual;
}

/* Overdefinition of < operator to fit with Combinaison class. Returns a bool3States enum on the ground that to return TRUE, FALSE or NULL_STATE (for a tie game). */
bool3States Combinaison::operator<(Combinaison combinaison) {

	bool3States isInferior;

	combinaisonComposition_.cards_.SortCardListByValue();
	combinaison.combinaisonComposition_.cards_.SortCardListByValue();

	combinaisonTypes myCombinaison = combinaisonComposition_.combinaison_;
	combinaisonTypes combinaisonCombinaison = combinaison.combinaisonComposition_.combinaison_;

	/* If the two combinaisons are equals, then returns NULL_STATE : it is a tie game. */
	if (*this == combinaison) return NULL_STATE;
	/* If the two combinaisons are not equals. */
	else if (myCombinaison == combinaisonCombinaison) {

		/* Let's get all the card values of the two combinaisons. */
		vector<string> valuesOfThis;
		for each (Card card in *combinaisonComposition_.cards_.GetCardList()) { valuesOfThis.push_back(card.GetValue()); }

		vector<string> valuesCombinaison;
		for each(Card card in *combinaison.combinaisonComposition_.cards_.GetCardList()) { valuesCombinaison.push_back(card.GetValue()); }

		/* If the two vector<string> containning all the values are equal. For more details check : https://en.cppreference.com/w/cpp/container/vector/operator_cmp */
		if (valuesOfThis == valuesCombinaison) {
			/* Then to separate the winner to the loser, evaluate the highest card. The highest card win the round. */
			if (Card::ConvertCardValueToNumber(highestCard_.GetValue()) < Card::ConvertCardValueToNumber(combinaison.highestCard_.GetValue())) {
				isInferior = TRUE;
			}
			else {
				isInferior = FALSE;
			}
		}
		/* If values are not equals. */
		else if (valuesOfThis < valuesCombinaison) {
			isInferior = TRUE;
		}
		else {
			isInferior = FALSE;
		}
	}
	/* If combinaison of the current item is inferior to the combinaison in parameter then return FALSE. */
	else if (myCombinaison < combinaisonCombinaison) {
		isInferior = TRUE;
	}
	/* Otherwise return FALSE. */
	else {
		isInferior = FALSE;
	}

	return isInferior;
}

/* Overdefinition of > operator to fit with Combinaison class. Returns a bool3States enum on the ground that to return TRUE, FALSE or NULL_STATE (for a tie game). */
bool3States Combinaison::operator>(Combinaison combinaison) {
	if (*this == combinaison) return NULL_STATE;
	else if (*this < combinaison) return FALSE;
	else return TRUE;
}

#pragma endregion
