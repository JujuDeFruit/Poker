#include "card_definitions.h"
#include<iostream>
#include<string>
#include <io.h>
#include <fcntl.h>

#pragma region Constructor

/*
 * Constructor of the crd class. Each card is unique and defined by a number and a suit.
 * @param number : number of the card (string).
 * @param suit : suit of the card (string).
 */
Card::Card(string number, string suit)
{
	value_ = number;
	suit_ = suit;
}

#pragma endregion

#pragma region Methods

/* 
 * Print a card with the correspondant symbol. 
 */
void Card::PrintCard()
{
	cout << "|";

	/* Set the stream mode to print Unicode-16 text */
	_setmode(_fileno(stdout), _O_U16TEXT);

	wchar_t* symbol = nullptr;

	/* We are trying to get unicode code to print the symbole of each suit.*/
	if (suit_ == "spade")
	{
		symbol = L"\u2660"; // Unicode code for spades
	}
	else if (suit_ == "heart")
	{
		symbol = L"\u2665";
	}
	else if (suit_ == "diamond")
	{
		symbol = L"\u2666";
	}
	else if (suit_ == "club")
	{
		symbol = L"\u2663";
	}
	
	wstring value(value_.begin(), value_.end()); // Converting the string value of the card into wchar_t * value to print it with the symbol.
	const wchar_t* val = value.c_str(); // Converting to wchar_t* type

	/* Print Unicode-16 text*/
	wcout << value + L" " + symbol;
	
	/* Reset the standard mode to print standard text */
	_setmode(_fileno(stdout), _O_TEXT);

	cout << "| ";
}

/* 
 * Static method that permit to cast cards' value (string) to its int value (from 1 to 13 [King]). 
 * @param cardValue : string card value to convert to card value (int).
 */
int Card::ConvertCardValueToNumber(string cardValue) {

	int val = 0;
	if (cardValue == "1") {
		val = 14;
	}
	else if (cardValue == "King") {
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

#pragma endregion

#pragma region Overdefinitions

/* 
 * Compare two cards. Overdefinition of == operator to fit to the card class. 
 * @param card : card to compare to the current card.
 */
bool Card::operator==(const Card& card) const {
	if (value_ == card.GetValue() && suit_ == card.GetSuit()) return true;
	else return false;
}

/*
 * Compare two cards. Overdefinition of < operator to fit to the card class.
 * @param card : card to compare to the current card.
 */
bool Card::operator<(const Card& card) const {
	int valueThis = Card::ConvertCardValueToNumber(value_);
	int valueCard = Card::ConvertCardValueToNumber(card.value_);
	return valueThis < valueCard;
}

/*
 * Affectation of a card. Overdefinition of = operator to fit to the card class.
 * @param card : card to affect to the current card.
 */
void Card::operator=(const Card& card) {
	value_ = card.GetValue();
	suit_ = card.GetSuit();
}

#pragma endregion