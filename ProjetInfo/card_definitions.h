#pragma once
#include<string>
#include<vector>

using namespace std;

/* Function returns a random number*/
int RandomNumber(int i);

/* Class that represent a playing card*/
class Card {
private:
	string value_; // Card value
	string suit_;  // Card suit
public:
	Card(string = "", string = "");
	string GetNumber() const { return value_; }; // Method to get the card value
	string GetSuit() const { return suit_; }; // Method to get the card suit
	void PrintCard(); // Method to print a card
	bool operator==(Card); // Overdefinition of == operator
	bool operator!=(Card c) { return !(*this == c); }; // Overdefinition of != operator
	static int ConvertCardValueToNumber(string); // Convert the card value in parameter to the correspondant number
};

/* Class gather all the 52 card in a same deck (object). */
class Deck {
protected:
	vector<Card> cardList_; // The list of all the cards
public:
	Deck(vector<Card> = {}); // Build the deck
	Deck(vector<Card>::iterator, vector<Card>::iterator); // Deck class build with iterator.
	Deck(const Deck&); // Copy constructor
	vector<Card>* GetCardList() { return &cardList_; }; // Return the collection of cards of the deck.
	Card operator[](int i) { return cardList_[i]; }; // Overdefiniton of the [] operator.
	vector<Card>::iterator Begin() { return cardList_.begin(); }; // Return the first iterator of the collection of cards.
	vector<Card>::iterator End() { return cardList_.end(); }; //Return the last iterator of the collection of cards.
	static Deck ToDeck(vector<Card>); // Converts a collection of cards into a deck.
	void PrintDeck(); // Print the deck.
	static vector<string> SuitList();  // Return all suits a card can have.
	static vector<string> ValueList(); // Return all values a card can have.
	void SortCardListByValue(); // Sort a vector of cards from the lowest value (1) to the highest (King).
	void SortCardListBySuit(); // Gather same suit cards in a vector.
	int Count(string); // Count all the cards from a collection that contains the key.
	bool IsStraight(); // Return true if the vector of cards is straight.
	void ExtractCards(string); // Extract a subvector all cards contain the string key.
	void EraseCards(string); // Delete cards containning the key from the collection.
	void EraseCards(Deck); // Delete cards from the vector
};

class BeginDeck : public Deck {
private:
	bool isShaked_; // Is the deck shaked or not ?
public:
	BeginDeck();
	vector<Card> DrawCard(int = 1); // Allow to pick a/many card(s) from the deck (max 5 cards).
	void ShakeDeck(); // Shake the deck.
};

/* Enumeration of all the poker combinaisons. The combinaisons are sorted by strength. */
enum combinaisonTypes {
	highCard = 0, // Hauteur
	pair = 1, // Paire
	twoPairs = 2, // Double paire
	trips = 3, // Brelan
	straight = 4, // Suite
	flush = 5, // Couleur
	fullhouse = 6, // Full
	quads = 7, // Carré
	straightFlush = 8, // Quinte flush
	royalFlush = 9 // Quinte flush royale
};

/* Struct used to identified a combinaison. */
struct CombinaisonComposition {
	combinaisonTypes combinaison_; // Type of the combinaison
	vector<Card> onCards_; // Cards involved in the combinaison
	bool isCombinaison_; // Is there a specific combinaison ?
};

/* Class used to create combinaison throught cards. This class is link to the combinaisonTypes enum. */
class Combinaison {
private: 
	CombinaisonComposition combinaisonComposition_; // All the informations about the combinaison.
	Card highestCard; // The highest card of his hand used to evoid tie.
public:
	Combinaison(Deck, Deck);
	static CombinaisonComposition RoyalFlush(Deck);		// Is the cards create a RoyalFlush combinaison ?
	static CombinaisonComposition StraightFlush(Deck);	// Is the cards create a Straight Flush combinaison ?
	static CombinaisonComposition Quads(Deck);			// Is the cards create a Quads combinaison ?
	static CombinaisonComposition Fullhouse(Deck);		// Is the cards create a Fullhouse combinaison ?
	static CombinaisonComposition Flush(Deck);			// Is the cards create a Flush combinaison ?
	static CombinaisonComposition Straight(Deck);		// Is the cards create a Straight combinaison ?
	static CombinaisonComposition Trips(Deck);			// Is the cards create a Trips combinaison ?
	static CombinaisonComposition TwoPairs(Deck);		// Is the cards create a TwoPairs combinaison ?
	static CombinaisonComposition Pair(Deck);			// Is the cards create a Pair combinaison ?
	static CombinaisonComposition HighCard(Deck);		// The highest card of the deck if any combinaison is created
};



