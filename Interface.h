#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;
class Interface;
class Game;

// This struct contains the layout for acceptable commands and their appropriate function calls
struct CommandMapping
{
public:
	// This line creates a keyword to refer to a function pointer. Now we can use 'FunctionPtr' instead of that nasty function pointer declaration
	// QUESTION INT1: What is a function pointer, and why do we use it here?
	using FunctionPtr = string(Game::*)(string);

	// This function pointer refers to the function the player will be directed to
	FunctionPtr Function;

	// This array of strings is the list of corresponding inputs the player may give to call the function
	vector<string> Inputs;

	// This constructor function populates the struct with the input variables
	CommandMapping(FunctionPtr InFunction, vector<string> InInputs)
	{
		Function = InFunction;
		Inputs = InInputs;
	}
};

// This class manages the conversation between the game and the player
class Interface
{
public:
	Interface(Game* InGame);
	~Interface() {};

	// The parent game this class belongs to
	Game* TextAdv = nullptr;

	// These are the mappings for the player inputs
	vector<CommandMapping> InputMappings;

	// This function populates the CommandMapping array
	void PopulateCommandArray();

	// This functions asks the player for their next move
	void PromptPlayer();
	// This string is the player's prompt to act
	string Prompt = "\nWhat would you like to do?\n";
	// This function interprets the player's input into a command for the game, and returns the console text for the output
	string ParseInput(string PlayerCommand);
	// this is the string that is returned if the player passed in an invalid command
	string InvalidCommand = "That didn't quite make sense, why don't you try again?";
};

