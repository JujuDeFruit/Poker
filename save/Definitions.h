#pragma once
#include<string>
#include<vector>

using namespace std;

class Card
{
private:
	int number_;
	string suit_;
public:
	Card(int, string);
	int GetNumber() const { return number_; };
	string GetSuit() const { return suit_; };
	static vector<string> SuitList();
	static vector<int> NumberList();
	void PrintCard();
};

class Deck
{
private:
	vector<Card> cardList_;
public:
	Deck();
	void PrintDeck();
};