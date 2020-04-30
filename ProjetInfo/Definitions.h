#pragma once
#include<string>

class Carte
{
private:
	int number_;
	std::string suit_;
public:
	Carte(int, std::string);
	int GetNumber() const { return number_; };
	std::string GetSuit() const { return suit_; };
};