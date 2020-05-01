#pragma once
#include<string>
#include<vector>

using namespace std;

class Card
{
private:
	string value_;
	string suit_;
public:
	Card(string, string);
	string GetNumber() const { return value_; };
	string GetSuit() const { return suit_; };
	void PrintCard();
};

class Deck
{
private:
	vector<Card> cardList_;
public:
	Deck();
	static vector<string> SuitList();
	static vector<string> ValueList();
	void PrintDeck();
};