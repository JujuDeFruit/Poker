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
	bool operator==(Card);
};

/* Class gather all the 52 card in a same deck (object). */
class Deck {
private:
	vector<Card> cardList_; // The list of all the cards
	bool isShaked_ = false; // Is the deck shaked or not ?
public:
	Deck(); // Build the deck
	static vector<string> SuitList();  // Return all suits a card can have.
	static vector<string> ValueList(); // Return all values a card can have.
	vector<Card> DrawCard(int = 1); // Allow to pick a/many card(s) from the deck (max 5 cards).
	void ShakeDeck(); // Shake the deck.
	void PrintDeck(); // Print the deck.
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
	vector<Card> onCards_; // Cards involved int he combinaison
	bool isCombinaison_; // Is there a specific combinaison ?
};

/* Class used to create combinaison throught cards. This class is link to the combinaisonTypes enum. */
class Combinaison {
private: 
	CombinaisonComposition combinaisonComposition_; // All the informations about the combinaison.
	Card highestCard; // The highest card of his hand used to evoid tie.
public:
	Combinaison(vector<Card>, vector<Card>);
	static int ConvertCardValueToNumber(string);
	static vector<Card> SortCardListByValue(vector<Card>);
	static CombinaisonComposition RoyalFlush(vector<Card>);		// Is the cards create a RoyalFlush combinaison ?
	static CombinaisonComposition StraightFlush(vector<Card>);	// Is the cards create a Straight Flush combinaison ?
	static CombinaisonComposition Quads(vector<Card>);			// Is the cards create a Quads combinaison ?
	static CombinaisonComposition Fullhouse(vector<Card>);		// Is the cards create a Fullhouse combinaison ?
	static CombinaisonComposition Flush(vector<Card>);			// Is the cards create a Flush combinaison ?
	static CombinaisonComposition Straight(vector<Card>);		// Is the cards create a Straight combinaison ?
	static CombinaisonComposition Trips(vector<Card>);			// Is the cards create a Trips combinaison ?
	static CombinaisonComposition TwoPairs(vector<Card>);		// Is the cards create a TwoPairs combinaison ?
	static CombinaisonComposition Pair(vector<Card>);			// Is the cards create a Pair combinaison ?
	static CombinaisonComposition HighCard(vector<Card>);		// The highest card of the deck if any combinaison is created
};



