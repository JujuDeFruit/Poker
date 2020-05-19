#pragma region Includes

#include "card_definitions.h"
#include<iostream>
#include<string>
#include <io.h>
#include <fcntl.h>

#pragma endregion

#pragma region Constructor

/*Each card is unique and defined by a number and a suit*/
Card::Card(string number, string suit)
{
	value_ = number;
	suit_ = suit;
}

#pragma endregion

#pragma region Methods

/* Print a card with the correspondant symbol. */
void Card::PrintCard()
{
	cout << " |";

	/* Set the flux mode to print Unicode-16 text */
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
	wcout << value + L"\t" + symbol;
	
	/* Reset the flux mode to print standard text */
	_setmode(_fileno(stdout), _O_TEXT);

	cout << "| ";
}

/* Static method that permit to cast cards' value (string) to its int value (from 1 to 13 [King]). */
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

/* Method that permit to compare two cards. This method will be usefull to create Combinaison. */
bool Card::operator==(const Card& card) const {
	if (value_ == card.GetValue() && suit_ == card.GetSuit()) return true;
	else return false;
}

/* Method to compare cards. */
bool Card::operator<(const Card& card) const {
	int valueThis = Card::ConvertCardValueToNumber(value_);
	int valueCard = Card::ConvertCardValueToNumber(card.value_);
	return valueThis < valueCard;
}

/* Overdefinition of assignement operator. */
void Card::operator=(const Card& card) {
	value_ = card.GetValue();
	suit_ = card.GetSuit();
}

#pragma endregion