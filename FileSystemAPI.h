#ifndef FILE_SYSTEM_API
#define FILE_SYSTEM_API

#include "FileSystem.h"

class FileSystemAPI
{
	FileSystem fs;
public:
	FileSystemAPI(string);
	/* returns the current path */
	string getPath();
	int readFile(string);
	int createFolder(string);
	int createFile(string);
	void deleteFolder(string);
	int deleteFile(string);
	int changeDirectory(string);
	int editFile(string);
	/* lists the contents of current directory */
	void listDirectoryContents();
};

#endif