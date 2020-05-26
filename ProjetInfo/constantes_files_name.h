#pragma once
#include <string>

/* 
 * Namespace containing all files name.
 */
namespace ConstFiles {

	/* File used to synchronize players. */
	const std::string INITFILE = "__init__.txt";

	/* File storing current deck. */
	const std::string DECKFILESERVER = "1Deck.txt";
	const std::string DECKFILECLIENT = "0Deck.txt";

	/* File containing the river. */
	const std::string RIVERFILESERVER = "1River.txt";
	const std::string RIVERFILECLIENT = "0River.txt";

	/* File containing the current turn. Player 0 or 1 has to play ? */
	const std::string TURNFILESERVER = "1Turn.txt";
	const std::string TURNFILECLIENT = "0Turn.txt";

	/* File containing player's hand. */
	const std::string HANDFILESERVER = "1Hand.txt";
	const std::string HANDFILECLIENT = "0Hand.txt";

	/* File containing previous action of the player. */
	const std::string ACTIONFILESERVER = "1Action.txt";
	const std::string ACTIONFILECLIENT = "0Action.txt";

	/* File containing money played by the player. */
	const std::string MONEYFILESERVER = "1Money.txt";
	const std::string MONEYFILECLIENT = "0Money.txt";

	/* File containing the winner. */
	const std::string WINNERFILESERVER = "1Winner.txt";
	const std::string WINNERFILECLIENT = "0Winner.txt";

	/* File containing the current pot. */
	const std::string POTFILESERVER = "1Pot.txt";
	const std::string POTFILECLIENT = "0Pot.txt";
}