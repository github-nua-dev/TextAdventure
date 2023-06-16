#include "Interface.h"
#include "TextAdventure.h"

// The constructor for the Interface class
Interface::Interface(Game* InGame)
{
	TextAdv = InGame;
	TextAdv->PlayerInterface = this;
	PopulateCommandArray();
}

// This function populates the CommandMapping array
void Interface::PopulateCommandArray()
{
	// Here we have three commands: move, take, open, and inventory. These commands each have multiple inputs that will call them
	InputMappings.push_back(CommandMapping(&Game::Move, { "move", "go", "travel" }));
	InputMappings.push_back(CommandMapping(&Game::Take, { "take", "pickup", "collect" }));
	InputMappings.push_back(CommandMapping(&Game::Open, { "open", "unlock" }));
	InputMappings.push_back(CommandMapping(&Game::Inv,  { "inventory", "bag" }));
}

// This functions asks the player for their next move
void Interface::PromptPlayer()
{
	// Ask the player to act
	cout << Prompt << endl;

	// Take an input, and send it for parsing
	string Command = "";
	getline(cin, Command);
	string Response = ParseInput(Command);
	cout << endl << Response;
}

// This function interprets the player's input into a command for the game, and returns the console text for the output
string Interface::ParseInput(string PlayerCommand)
{
	// First we lower case the command...
	string Input = PlayerCommand;

	// QUESTION INT2: The 'A ? B : C' used here is a specific operator, what is this and what does it do?
	for (char& CharIndex : Input) CharIndex = islower(CharIndex) ? CharIndex : tolower(CharIndex);
	
	// ...and separate the command from the object/direction
	string Command = Input;
	string Object = " ";
	TextAdv->RemoveSubstrings(Command, Object);

	// Then we check to see if the command exists in the command mapping
	for (CommandMapping& CommandMap : InputMappings)
	{
		if (count(CommandMap.Inputs.begin(), CommandMap.Inputs.end(), Command) > 0)
		{
			// If it does, we call it with the object/direction
			// QUESTION INT3: What is that asterisk doing there?
			return (TextAdv->*CommandMap.Function)(Object);
		}
	}

	// If it doesn't, we return the invalid command string
	return InvalidCommand;
}