#include "TextAdventure.h"
#include "Map.h"
#include "Interface.h"
#include "Inventory.h"

int main()
{
	Game TextAdv = Game();
	return 0;
}

// The constructor for the Game class
Game::Game()
{
	// Create the managing classes for the game
	GameMap = new Map(this);
	PlayerInterface = new Interface(this);
	PlayerInventory = new Inventory(this);

	// Introduce, prompt, and end the game
	cout << "Welcome to " << GameName << "!\n" << GameMap->EnterRoom(StartRoom);
	while (!bHasWon) PlayerInterface->PromptPlayer();
	cout << "Congratulations, you won the game!\nPress any button to exit.";

	// Let the player end when they're ready
	string goodbye;
	cin >> goodbye;
}

// Removes a substring from a given string, and returns the before and after
void Game::RemoveSubstrings(string& InString, string& Substring)
{
	// First we find out if the substring exists, and if it doesn't, we return out of the function
	size_t Index = InString.find(Substring);
	if (Index == -1) return;

	// We then separate the prefix and suffix from the instring, and set the InString to the prefix, and the Substring to the suffix
	string Before = InString.substr(0, InString.find(Substring));
	string After = InString.substr(InString.find(Substring) + 1);
	InString = Before;
	Substring = After;
}

// This function allows the player to move accross the game map, returning if the movement is valid
string Game::Move(string Direction)
{
	// Parse our direction into an upper case character
	char DirInitial = islower(Direction.at(0)) ? toupper(Direction.at(0)) : Direction.at(0);

	// Is the direction even valid?
	string Dirs = "NESW";
	if (count(Dirs.begin(), Dirs.end(), DirInitial) == 0) return "That doesn't seem like a direction.";
	// Check the exit is valid for the room
	string CurrentExits = GameMap->GetRoom(CurrentRoom).Exits;
	if (count(CurrentExits.begin(), CurrentExits.end(), DirInitial) == 0)
	{
		// Return an invalid movement if we haven't moved
		return "You can't move " + Direction + "\n";
	}

	// Get the new room's index
	int DirectionIndex;
	switch (DirInitial)
	{
	case 'N': DirectionIndex = -static_cast<int>(size(GameMap->GameMap[0])); break; // North and south are equal to adding or subtracting the number of columns from the current room number
	case 'E': DirectionIndex = 1; break; // East and west are left and right by one
	case 'S': DirectionIndex = static_cast<int>(size(GameMap->GameMap[0])); break;
	case 'W': DirectionIndex = -1; break;
	default: return "That doesn't seem like a direction";
	}

	// Get the new room's details and return them
	CurrentRoom += DirectionIndex;
	return GameMap->EnterRoom(CurrentRoom);

}
// This function allows the player to take an item, returning whether or not it was taken
string Game::Take(string Object)
{
	if (PlayerInventory->CollectItem(Object, CurrentRoom))
		return "You collected: " + Object + "\n";
	return "You can't do that\n";
}
// This function opens or unlocks an obstacle, returning the success
string Game::Open(string Object)
{
	if (GameMap->OvercomeObstacle(Object))
		return "AH-HA!\n";
	// QUESTION G3: Can you make the program re-print the room description with the new description when it changes?

	return "No luck there\n";
}
// This function reads the player's inventory
string Game::Inv(string Item)
{
	// If nothing was passed in, print the whole inventory
	if (Item == " ") return PlayerInventory->PrintInventory();

	// If the player has the item, acknowledge it
	if (PlayerInventory->HasItem(Item)) return "You have:\n" + Item + "\n";

	// If the player doesn't have the item, tell them they don't
	return "It doesn't look like you have that";
}