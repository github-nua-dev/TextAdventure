#include "Map.h"
#include <string>
#include <iostream>
// The constructor for the Map class
Map::Map(Game* InGame)
{
	TextAdv = InGame;
	ParseMap(LoadMap("Map.csv"));
	TextAdv->GameMap = this;
	InitObstacles();
}

// This function loads the csv file with the mpa details into the game
vector<Room> Map::LoadMap(string FilePath)
{
	// Load the map and check if it exists
	ifstream MapFile("Map.csv");
	if (!MapFile) return vector<Room> {Room()};

	// Create a line of data and a vector to store the correlating name, description and exits
	string MapLine;
	vector<Room> MapData;

	// Loop through the CSV file and populate the map vector (ignoring the initial blankspace), by filling a Room struct and add it onto the end
	bool bIsWhitespace = true;
	Room NewRoom = Room();
	int Index = -1;

	// QUESTION M2: Why are we using a while loop here instead of a do-while?
	while (getline(MapFile, MapLine, ','))
	{
		if (AddMapLineToRoom(MapLine, Index, NewRoom, MapData)) return MapData;
	}
	// return the map vector
	return MapData;
}
// Sets the appropriate Map line to the generating room
bool Map::AddMapLineToRoom(string& Line, int& Index, Room& CurrentRoom, vector<Room>& MapData)
{
	// Remove blankspace characters before file parsing
	if (Index == -1)
	{
		Line.erase(0, 3);
		Index = 0;
	}

	// Check the line for a newline character, and split it if it exists
	string NewLine = "\n";
	TextAdv->RemoveSubstrings(Line, NewLine);
	CurrentRoom = CreateRoom(CurrentRoom, Line, Index);
	// Check for the end of the Room
	CheckRoomCreationEnd(MapData, CurrentRoom, NewLine, Index);

	// Check for the end of the map and return if it's full or not
	size_t Rows = size(GameMap);
	size_t Columns = size(GameMap[0]);
	return int(MapData.size()) == Rows * Columns;
}
// Checks if the map data has reached the end of the Room or file
bool Map::CheckRoomCreationEnd(vector<Room>& MapData, Room& InRoom, string InString, int& Index)
{
	Index++;

	// Is the leftover after removing a substring a substantial string?
	// QUESTION M3: Why are these two values the checks in this if statement?
	if (InString != "\n" && InString != "")
	{
		// Add the room to the map vector
		MapData.push_back(InRoom);

		// Create a new room with the substring's new room name
		Index = 0;
		InRoom = Room();
		InRoom = CreateRoom(InRoom, InString, Index);
		Index++;
		return true;
	}

	// Have we reached the end of a Room?
	if (Index > 2)
	{
		// Add the room to the map vector and initialise a new room
		MapData.push_back(InRoom);
		Index = 0;
		InRoom = Room();
		return true;
	}

	return false;
}
// Enters data into a Room struct at the appropriate variable
Room Map::CreateRoom(Room& InRoom, string InString, int Index)
{
	// Switch on Index to populate the room
	switch (Index)
	{
	case 0:
		InRoom.RoomName = InString;
		return InRoom;
	case 1:
		InRoom.Description = InString;
		return InRoom;
	case 2:
		InRoom.Exits = InString;
		return InRoom;
	default: return InRoom;
	}
}

// This function reads the map vector and converts it into a 2D array to be traversed by the player
void Map::ParseMap(vector<Room> Map)
{
	// First we get the amount of rows and columns there are in our map
	size_t Rows = size(GameMap);
	size_t Columns = size(GameMap[0]);

	// Then we create the room for the appropriate grid space by looping through the columns and rows
	for (size_t MapY = 0; MapY < Columns; MapY++)
	{
		for (size_t MapX = 0; MapX < Rows; MapX++)
		{
			size_t Index = MapX * Columns + MapY;
			GameMap[MapX][MapY] = Map.at(Index);
		}
	}
}

// This function returns the name and description of the room, ready to be output to the player
Room Map::GetRoom(int RoomIndex)
{
	// Get the row and column of the required room from the size of the arrays
	size_t Column = RoomIndex % size(GameMap[0]);
	size_t Row = (RoomIndex - Column) / size(GameMap);
	return GameMap[Row][Column];
}
string Map::EnterRoom(int RoomIndex)
{
	// Read the appropriate room from the map
	Room EnteredRoom = GetRoom(RoomIndex);
	return EnteredRoom.ReadRoom();
}
string Room::ReadRoom()
{
	return RoomName + "\n" + Description + "\n";
}

// This function initialises the obstacles in the world
void Map::InitObstacles()
{
	// QUESTION M4: The obstacles are recreated in-line here. Can you think of a way to load them from a seprate file?
	Obstacles.push_back(Obstacle("chest", 4, "key", false, "This is Room 5, a chest lays open on the ground, inside a little goblin gives you a thumbs up"));
}
// This function notes the obstacle as done, and replaces the world map description
bool Map::OvercomeObstacle(string ObstacleName)
{
	// Loop through the obstacle array
	for (Obstacle& TestObstacle : Obstacles)
	{
		// If we find one with the name, solution and room passed in, and make sure it's not already been resolved...
		if (TestObstacle.ObstacleName == ObstacleName && TextAdv->PlayerInventory->HasItem(TestObstacle.Solution) && TestObstacle.RoomIndex == TextAdv->CurrentRoom && !TestObstacle.Resolved)
		{
			// ... resolve the obstacle and replace the room description
			TestObstacle.Resolved = true;
			TextAdv->PlayerInventory->ConsumeItem(TestObstacle.Solution);

			// Get the row and column of the required room from the size of the arrays
			size_t Column = TextAdv->CurrentRoom % size(GameMap[0]);
			size_t Row = (TextAdv->CurrentRoom - Column) / size(GameMap);
			GameMap[Row][Column].Description = TestObstacle.LatterDescription;

			// Check if the player has won the game
			// QUESTION M4: This seems inefficient, checking for a specific item every time, is there a way we can make this automatic?
			if (ObstacleName == "chest")
				TextAdv->bHasWon = true;
			return true;
		}
	}
	return false;
}