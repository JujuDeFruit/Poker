#include "card_definitions.h"
#include<iostream>
#include<string>
#include <io.h>
#include <fcntl.h>

/*Each card is unique and defined by a number and a suit*/
Card::Card(string number, string suit)
{
	value_ = number;
	suit_ = suit;
}


void Card::PrintCard()
{
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
	wcout << value + L"\t" + symbol << endl;
	
	/* Reset the flux mode to print standard text */
	_setmode(_fileno(stdout), _O_TEXT);
}

/* Method that permit to compare two cards. This method will be usefull to create Combinaison. */
bool Card::operator==(Card card) {
	if (value_ == card.GetNumber() && suit_ == card.GetSuit()) return true;
	else return false;
}