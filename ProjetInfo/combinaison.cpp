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

	/* Let's find the highest combinaison as possible. */
	if (RoyalFlush(hand, river).isCombinaison_) {
		combinaisonComposition_ = RoyalFlush(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (StraightFlush(hand, river).isCombinaison_) {
		combinaisonComposition_ = StraightFlush(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Quads(hand, river).isCombinaison_) {
		combinaisonComposition_ = Quads(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Fullhouse(hand, river).isCombinaison_) {
		combinaisonComposition_ = Fullhouse(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Flush(hand, river).isCombinaison_) {
		combinaisonComposition_ = Flush(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Straight(hand, river).isCombinaison_) {
		combinaisonComposition_ = Straight(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Trips(hand, river).isCombinaison_) {
		combinaisonComposition_ = Trips(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (TwoPairs(hand, river).isCombinaison_) {
		combinaisonComposition_ = TwoPairs(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else if (Pair(hand, river).isCombinaison_) {
		combinaisonComposition_ = Pair(hand, river);
		highestCard_ = HighCard(hand).cards_[0];
	}
	else {
		combinaisonComposition_ = HighCard(hand);
	}
	cout << combinaisonComposition_.combinaison_ << endl;
	combinaisonComposition_.cards_.PrintDeck();
	system("pause");
}

#pragma endregion

#pragma region Check combinaison

/* This method return the composition of a royale pair combinaison. */
CombinaisonComposition Combinaison::RoyalFlush(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river);		// Build the full deck

	const vector<string> suitList = Deck::SuitList();
	/* Browse all suit to find a flush. */
	for each (string suit in suitList)
	{
		/* If the hand and the river create a Royal Flush combinaison, and the hand is involved in this one. */
		if (deck.Count(Card("10", suit))
			&& deck.Count(Card("Jack", suit))
			&& deck.Count(Card("Queen", suit))
			&& deck.Count(Card("King", suit))
			&& deck.Count(Card("1", suit))
			&& (hand.Count(Card("10", suit)) || hand.Count(Card("Jack", suit)) || hand.Count(Card("Queen", suit)) || hand.Count(Card("King", suit)) || hand.Count(Card("1", suit)))
			)
		{
			/* Then return the combinaison. */
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::straightFlush;
			combinaisonComposition.cards_ = Deck::ToDeck({ Card("10", suit) , Card("Jack", suit) , Card("Queen", suit) , Card("King", suit) , Card("1", suit) });
			break;
		}
	}

	return combinaisonComposition;
}

/* This method return the composition of a straight flush combinaison. */
CombinaisonComposition Combinaison::StraightFlush(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river);		// Build the full deck.

	/* Boolean used to know if the deck contains a flush. */
	bool fiveCardsWithSameColor = false;

	const vector<string> suitList = Deck::SuitList();
	/* Browse all the suit a card can have. */
	for each(string suit in suitList) {
		/* If the deck contains 5 or more cards with the same suit, then the deck has a flush. */
		if (deck.Count(suit) >= MAX_CARDS_COMBINAISON) {
			/* Extract the cards involved in the flush from the hand and the river. */
			hand.ExtractCards(suit);
			river.ExtractCards(suit);
			/* the deck contains a flush. it is a current evaluated suit flush. */
			fiveCardsWithSameColor = true;
			break;
		}
	}

	/* If there is not a fluh or if the hand is not involved in this one, then return a null combinaison. */
	if (!fiveCardsWithSameColor || !hand.GetCardList()->size()) return combinaisonComposition;

	/* Check if the flush can create a straight. */
	CombinaisonComposition straightCardList = Straight(hand, river);
	/* If the current flush is a straight too, then it is a straight flush. */
	if (straightCardList.isCombinaison_ && hand.GetCardList()->size()) {
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::straightFlush;
		combinaisonComposition.cards_ = straightCardList.cards_;
	}

	return combinaisonComposition;
}

/* This method return the composition of a quads combinaison. */
CombinaisonComposition Combinaison::Quads(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river);		// Build the full deck
	deck.SortCardListByValue();

	/* If the hand and the river do not build a quads or if the river build a quad by itself, then return the null combiniaison. */
	if (!deck.CountCombinaison(combinaisonTypes::quads) || river.CountCombinaison(combinaisonTypes::quads)) return combinaisonComposition;

	/* It is sure, there is a quad. Count the number of same value card in the deck. When the quad is found, then quit the loop with break. */
	for each (Card card in *deck.GetCardList()) {
		string value = card.GetValue();
		/* If there are 4 cards with the same value in the deck and one of the cards at least is in the hand. */
		if (deck.Count(value) == 4 && hand.Count(value)) {
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::quads;
			Deck cards(deck);
			cards.ExtractCards(value);
			combinaisonComposition.cards_ = cards;
			break;
		}
	}

	return combinaisonComposition;
}

/* This method return the composition of a fullhouse combinaison. */
CombinaisonComposition Combinaison::Fullhouse(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river); // Build the full deck
	Deck pairDeck(deck);

	bool isTrip = false, isPair = false;

	/* There is 1 trip max, else it is not a Fullhouse. */
	CombinaisonComposition trips;
	if (deck.CountCombinaison(combinaisonTypes::trips)) {	// If there is a trip.
		trips = Trips(hand, river);							// Then, get the value.
		if (trips.cards_.GetCardList()->size()) {			// If the trip is involved in the hand.
			hand.EraseCards(trips.cards_[0].GetValue());	// Erase cards involved in the trip.
			river.EraseCards(trips.cards_[0].GetValue());
			isTrip = true;
		}
		else return combinaisonComposition;					// Any trip is made from the hand.
	}
	else return combinaisonComposition;
	
	deck = hand.Concat(river); // Rebuild the deck without cards involved in the trip

	CombinaisonComposition pair;
	if (deck.CountCombinaison(combinaisonTypes::pair)) {
		pair = Pair(hand, river);			// If the is a pair, then get it.
		isPair = true;
	}
	else if (deck.CountCombinaison(combinaisonTypes::trips)) {
		pair = Trips(hand, river);			// Else if there is another trip, then get it, and consider it as a pair.
		pair.cards_.EraseCards(hand);
		pair.cards_ = hand + Deck::ToDeck({pair.cards_[0]});
		isPair = true;
	}
	else return combinaisonComposition;

	if (isTrip && isPair) {				// if there is a pair and a trip, it is a fullhouse.
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::fullhouse;
		combinaisonComposition.cards_ = trips.cards_ + pair.cards_;
	}

	return combinaisonComposition;
}

/* This method return the composition of a flush combinaison. */
CombinaisonComposition Combinaison::Flush(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river);

	/* Deck that store all the cards of the flush. */
	Deck sameSuitCards;
	/* Variable that store the suit involved in the flush, if there is. */
	string suit;

	/* Browse the deck to find a card combinaison where there are 5 cards or more that have the same suit. */
	const vector<string> suitList = Deck::SuitList();
	for each(string suitValue in suitList) {
		/* If the suit create a flush. */
		if (deck.Count(suitValue) >= MAX_CARDS_COMBINAISON) {
			sameSuitCards = deck;
			sameSuitCards.ExtractCards(suitValue);
			sameSuitCards.SortCardListByValue();
			suit = suitValue;
			break;
		}
	}

	if (river.Equivalent(sameSuitCards) || sameSuitCards.GetCardList()->empty()) return combinaisonComposition;

	combinaisonComposition.isCombinaison_ = true;
	combinaisonComposition.combinaison_ = combinaisonTypes::flush;

	int size = sameSuitCards.GetCardList()->size();

	/* If the flush is composed at least by 5 cards, hand is involded. */
	if (size == MAX_CARDS_COMBINAISON) {
		combinaisonComposition.cards_ = sameSuitCards;
	}
	/* If the flush is composed by 6 or 7 cards, then get the highest ones which han is involved. */
	else if (size > MAX_CARDS_COMBINAISON) {
		hand.SortCardListByValue();
		/* Get the highest cards. */
		Deck comb(sameSuitCards.End() - 5, sameSuitCards.End());
		/* if the highest cards are the river, then intervert the last card of the hand with the first card of the combinaison. */
		if (river.Equivalent(comb)) {
			comb.GetCardList()->front() = sameSuitCards.GetCardList()->size() == MAX_CARDS_COMBINAISON + 1 ? sameSuitCards[0] : sameSuitCards[1];
		}
		combinaisonComposition.cards_ = comb;
	}

	return combinaisonComposition;

}

/* This method return the composition of a flush combinaison. */
CombinaisonComposition Combinaison::Straight(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river);		// Build the full deck. 

	Deck cardList(deck);
	cardList.RemoveSameValueCards();		// Remove all multiple same value Card to keep the value information.
	cardList.SortCardListByValue();

	if (cardList.Count("1")) cardList.GetCardList()->front() = cardList.GetCardList()->back();	// If the deck has an ace, then this one can be the weakest or the strongest one. It has to be the first and the last element.

	int diffBetweenSizeAndNumberOfCards = cardList.GetCardList()->size() - MAX_CARDS_COMBINAISON;		// The number of cards to compare is 5.
																										// Then if the list is bigger than five, 
																										// the program has at least three straights to compare 
																										// and has to choose the stronger one (with the highest card).
	if (cardList.GetCardList()->size() >= MAX_CARDS_COMBINAISON) {
		/* Let's browse the deck from the end to get the strongest straight as possible. */
		for (int i = diffBetweenSizeAndNumberOfCards; i >= 0; i--) {
			/* Create the 5 cards current deck. */
			Deck list(cardList.GetCardList()->begin() + i, cardList.GetCardList()->end() + (i - diffBetweenSizeAndNumberOfCards));
			/* If the current 5 cards deck id a straight and if the hand is involved in the straight, then the current deck is a combinaison. */
			if (list.IsStraight() && (list.Count(hand.GetCardList()->front()) || list.Count(hand.GetCardList()->back()))) {
				combinaisonComposition.isCombinaison_ = true;
				combinaisonComposition.combinaison_ = combinaisonTypes::straight;
				combinaisonComposition.cards_ = *list.GetCardList();
				break;
			}
		}
	}

	return combinaisonComposition;
}

/* This method return the composition of a trips combinaison. */
CombinaisonComposition Combinaison::Trips(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river);

	if (!deck.CountCombinaison(combinaisonTypes::trips)) return combinaisonComposition;

	/* Sort the hand to have the strongest card at the end. */
	hand.SortCardListByValue();

	/* Browse the hand from the end to get the stongest card first. */
	for (vector<Card>::reverse_iterator card = hand.GetCardList()->rbegin(); card != hand.GetCardList()->rend(); card++) {
		string value = (*card).GetValue();
		/* If there are 3 cards with the same value, then quit the for loop and get the cards involved in the trip before. */
		if (deck.Count(value) == 3) {
			Deck trip(deck);
			trip.ExtractCards(value);
			combinaisonComposition.isCombinaison_ = true;
			combinaisonComposition.combinaison_ = combinaisonTypes::trips;
			combinaisonComposition.cards_ = trip;
			break;
		}
	}

	return combinaisonComposition;
}

/* This method return the composition of a two pairs combinaison. */
CombinaisonComposition Combinaison::TwoPairs(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck twoPairs;
	Deck deck = hand.Concat(river);

	/* Pair number in the deck. */
	int pairNumber = deck.CountCombinaison(combinaisonTypes::pair);

	/* If the river as the same pair number than the hand more the river, then river contains all pairs or if the number of pair isn't at least 2. */
	if (river.CountCombinaison(combinaisonTypes::pair) == pairNumber || pairNumber < 2 || hand[0].GetValue() == hand[1].GetValue()) return combinaisonComposition;

	bool isTwoPairs = true;

	/* If each card of the hand creates a pair, then it is a Two Pairs combinaison. */
	for each(Card card in *hand.GetCardList()) {
		Deck pair(deck);
		pair.ExtractCards(card.GetValue());
		/* If the extracted cards are a pair, then add it to the deck twoPairs, that will remain to the combinaison. */
		if (pair.GetCardList()->size() == 2) twoPairs = twoPairs + pair;
		else {
			isTwoPairs = false;
			break;
		}
	}

	/* If there are two pairs created by the hand. */
	if (isTwoPairs) {
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::twoPairs;
		combinaisonComposition.cards_ = twoPairs;
	}

	return combinaisonComposition;
}

/* This method return the composition of a pair combinaison. */
CombinaisonComposition Combinaison::Pair(Deck hand, Deck river) {
	CombinaisonComposition combinaisonComposition;

	Deck deck = hand.Concat(river);		// Build the full deck.

	/* If deck does not contain at least one pair, return null combinaison. */
	if (!deck.CountCombinaison(combinaisonTypes::pair)) return combinaisonComposition;

	/* Get the cards value of the hand*/
	string hand0Value = hand[0].GetValue();
	string hand1Value;
	if(hand.GetCardList()->size() == 2)
		hand1Value = hand[1].GetValue();
	else hand1Value = "";

	/* If the pair is just build with hand's cards. */
	if (hand0Value == hand1Value) {
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::pair;
		combinaisonComposition.cards_ = hand;
	}
	/* If the pair is build with the first card of the hand, and another card in the river. */
	else if (river.Count(hand0Value)) {
		river.ExtractCards(hand0Value);
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::pair;
		combinaisonComposition.cards_ = Deck::ToDeck({ hand[0] }) + river;
	}
	/* If the pair is build with the second card of the hand, and another card in the river. */
	else if (river.Count(hand1Value)) {
		river.ExtractCards(hand1Value);
		combinaisonComposition.isCombinaison_ = true;
		combinaisonComposition.combinaison_ = combinaisonTypes::pair;
		combinaisonComposition.cards_ = Deck::ToDeck({ hand[1] }) + river;
	}

	return combinaisonComposition;
}

/* Return the highest card of the deck. It is not considered as combinaison. */
CombinaisonComposition Combinaison::HighCard(Deck hand) {
	CombinaisonComposition combinaisonComposition;
	combinaisonComposition.combinaison_ = combinaisonTypes::highCard;

	/* Compare the 2 cards. Return the strongest one. */
	if (Card::ConvertCardValueToNumber(hand[0].GetValue()) > Card::ConvertCardValueToNumber(hand[1].GetValue()))
		combinaisonComposition.cards_ = Deck::ToDeck({ hand[0] });
	else
		combinaisonComposition.cards_ = Deck::ToDeck({ hand[1] });

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
		&& Card::ConvertCardValueToNumber(highestCard_.GetValue()) == Card::ConvertCardValueToNumber(combinaison.highestCard_.GetValue())) {
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