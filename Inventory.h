#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "TextAdventure.h"
using namespace std;

// This struct holds all the details for an inventory item
struct Item
{
public:
	// This is the name that will be displayed when the item is described
	string Name;

	// This is the room index that the item can be found in
	int Location;

	// This is a check to see if the player has collected the item yet
	bool bCollected;

	// This is a check to see if the item has been used and can't be used again
	bool bConsumed;

	// And this string is the replacement for the room description once the item is collected
	string LatterDescription;

	// This constructor function populates the struct with the input variables
	Item(string InName, int InLocation, bool bInCollected, bool bInConsumed, string InLatterDescription)
	{
		Name = InName;
		Location = InLocation;
		bCollected = bInCollected;
		bConsumed = bInConsumed;
		LatterDescription = InLatterDescription;
	}
};

// This class holds all the functions and details of items in the player's inventory
class Inventory
{
public:
	Inventory(Game* InGame);
	~Inventory() {};

	// This is the parent Game class
	Game* TextAdv = nullptr;

	// This is the list of items possble for the player to have in their inventory
	vector<Item> InventoryList;

	// This function collects the item from the room if possible
	bool CollectItem(string ItemName, int RoomIndex);

	// This function consumes an item if possible
	void ConsumeItem(string ItemName);

	// This function checks if the player has an item
	bool HasItem(string ItemName);

	// This function returns all the items the player has collected
	string PrintInventory();
};
