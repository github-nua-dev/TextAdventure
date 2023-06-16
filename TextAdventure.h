#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Map.h"
#include "Interface.h"
#include "Inventory.h"
using namespace std;
class Inventory;

// This class is the leading force behind all the logic in the game.
// It contains the map, game controls and commands, and the file-manipulation
class Game
{
public:
	Game();
	~Game() {};

	// This is the title of your game - make it cool!
	string GameName = "Text Adventure";

	// A flag for when the player has won the game
	bool bHasWon = false;

	// This is a reference to the map that holds the game world
	friend class Map;
	Map* GameMap = nullptr;

	// This is a reference to the interface between the player and the game
	friend class Interface;
	Interface* PlayerInterface = nullptr;
	// QUESTION G1: What does 'friend class' mean?

	// This is a reference to the player's inventory
	Inventory* PlayerInventory = nullptr;

	// This is the index of the map that we start at
	int StartRoom = 4;
	// And this is the index of the room we're currently in
	int CurrentRoom = 4;

	// INPUTS
	// This function allows the player to move accross the game map, returning the response
	string Move(string Direction);
	// This function allows the player to take an item, returning the reaction
	string Take(string Object);
	// This function opens or unlocks an obstacle, returning the success/failure
	string Open(string Object);
	// This function reads the player's inventory for a specific item, or prints the whole thing if no string is passed in
	string Inv(string Item = " ");

private:
	// Removes a substring from a given string, and returns the before and after
	void RemoveSubstrings(string& InString, string& Substring);
	// QUESTION G2: The '&' used here means we can change the values in-place, but how does that happen?
};