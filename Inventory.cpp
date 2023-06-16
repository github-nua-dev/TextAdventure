#include "Inventory.h"

// The constructor for the Intventory class
Inventory::Inventory(Game* InGame)
{
	// Set the parent reference
	TextAdv = InGame;
	// In here we populate the list of items in our game
	InventoryList.push_back(Item("key", 3, false, false, "This is room 4"));
}

// This function collects the item from the room if possible
bool Inventory::CollectItem(string ItemName, int RoomIndex)
{
	// Loop through the inventory array and see if the item...
	// QUESTION INV1: What is the for loop doing when we use 'Item& InvItem : InventoryList'?
	for (Item& InvItem : InventoryList)
	{
		// ...exists, is in the room, and hasn't already been collected
		if (InvItem.Name == ItemName && InvItem.Location == RoomIndex && !InvItem.bCollected)
		{
			// Collect the item and return a success if it is
			InvItem.bCollected = true;

			// Get the row and column of the required room from the size of the arrays
			// QUESTION INV2: Why is the size() function being divided, should it not return the size of the array?
			size_t Column = TextAdv->CurrentRoom % size(TextAdv->GameMap->GameMap[0]);
			size_t Row = (TextAdv->CurrentRoom - Column) / size(TextAdv->GameMap->GameMap);
			TextAdv->GameMap->GameMap[Row][Column].Description = InvItem.LatterDescription;
			return true;
		}
	}

	// Otherwise return that the take command was invalid
	return false;
}

// This function consumes an item if possible
void Inventory::ConsumeItem(string ItemName)
{
	// Loop through the inventory array and see if the item...
	for (Item& InvItem : InventoryList)
	{
		// ...exists, and has been collected
		if (InvItem.Name == ItemName && InvItem.bCollected)
		{
			// Consume the item and return
			InvItem.bConsumed = true;
			return;
		}
	}
}

// This function checks if the player has an item
bool Inventory::HasItem(string ItemName)
{
	// Loop through the inventory array and see if the item...
	for (Item& InvItem : InventoryList)
	{
		// ...exists, and hasn't been consumed
		if (InvItem.Name == ItemName && InvItem.bCollected && !InvItem.bConsumed)
		{
			// Return the acknowledgment that the player has the item
			return true;
		}
	}

	// Otherwise inform the player that they don't have the item
	return false;
}

// This function prints out all the items the player has collected
string Inventory::PrintInventory()
{
	bool InventoryEmpty = true;

	// QUESTION INV3: We've made a string here even though we won't use it if the inventory is empty. Why is it here, and what's the trade off/alternative?
	string InventoryString = "Your inventory contains:\n";
	// Loop through the inventory array and see if the item...
	for (Item& InvItem : InventoryList)
	{
		// ... has been collected
		if (InvItem.bCollected)
		{
			// Appends the name of the item
			InventoryString += InvItem.Name + "\n";
			InventoryEmpty = false;
		}
	}

	// Return the final output, but announce it's empty if no items were found
	if (InventoryEmpty) return "It looks like your inventory is empty\n";
	return InventoryString;
}