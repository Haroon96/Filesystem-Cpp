#include <iostream>
#include "FileSystemAPI.h"
#include "Modules.h"
using namespace std;

void runCommand(FileSystemAPI&, string*);
void printHelp();

int main()
{
	cout << "||===============================================================||" << endl;
	cout << "||================     Type 'help' for help.     ================||" << endl;
	cout << "||===============================================================||" << endl << endl;
	FileSystemAPI fs ("foo.txt");
	string input;
	string cmd[2];
	while (cmd[0] != "exit")
	{
		cout << fs.getPath();
		getline(cin, input);
		strTok(input, cmd);
		runCommand(fs, cmd);
	}
	return 0;
}

void runCommand(FileSystemAPI &fs, string *cmd)
{
	string command = lowercase(cmd[0]);
	if (command == "dir")
	{
		fs.listDirectoryContents();
	}
	else if (command == "cd")
	{
		fs.changeDirectory(cmd[1]);
	}
	else if (command == "touch")
	{
		fs.createFile(cmd[1]);
	}
	else if (command == "md")
	{
		fs.createFolder(cmd[1]);
	}
	else if (command == "del")
	{
		fs.deleteFile(cmd[1]);
	}
	else if (command == "rd")
	{
		fs.deleteFolder(cmd[1]);
	}
	else if (command == "edit")
	{
		fs.editFile(cmd[1]);
	}
	else if (command == "read")
	{
		fs.readFile(cmd[1]);
	}
	else if (command == "exit")
	{
		cout << "Exiting program." << endl;
	}
	else if (command == "help")
	{
		printHelp();
	}
	else
	{
		cout << "Unrecognized command." << endl;
	}
}

void printHelp()
{
	cout << "DIR" << "\t\t" << "Lists the contents of the current directory" << endl;
	cout << "CD" << "\t\t" << "Change directory" << endl;
	cout << "MD" << "\t\t" << "Creates a new folder" << endl;
	cout << "TOUCH" << "\t\t" << "Creates a new file" << endl;
	cout << "DEL" << "\t\t" << "Creates a new file" << endl;
	cout << "RD" << "\t\t" << "Removes an existing directory" << endl;
	cout << "EDIT" << "\t\t" << "Edits a file" << endl;
	cout << "READ" << "\t\t" << "Display contents of a file" << endl;
	cout << "EXIT" << "\t\t" << "Exit the program" << endl;
}
