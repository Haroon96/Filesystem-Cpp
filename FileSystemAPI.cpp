#include <iostream>
#include "FileSystemAPI.h"
#include "Modules.h"

FileSystemAPI::FileSystemAPI(string s) :fs(s)
{
}

int FileSystemAPI::createFile(string s)
{
	int index = fs.allocateIndexBlock();
	if (index == -1)
	{
		cerr << "Couldn't create a new block." << endl;
	}
	else if (index > 0)
	{
		if (fs.createObject(s, index, 2) == -1)
		{
			fs.deallocateBlock(index);
			cerr << "Couldn't create the file." << endl;
			index = -1;
		}
	}
	return index;
}

void FileSystemAPI::deleteFolder(string s)
{
	if (fs.changeDirectory(s) == -1)
	{
		cerr << "No such directory exists." << endl;
		return;
	}

	vector<string> directories = fs.getDirectoryList();
	vector<string> files = fs.getFileList();
	for (int i = 0; i < files.size(); i++)
	{
		deleteFile(files[i]);
	}
	for (int i = 0; i < directories.size(); i++)
	{
		deleteFolder(directories[i]);
	}
	fs.changeDirectory("..");
	fs.deallocateBlock(fs.getBlockIndex(s));
	fs.deleteObject(s);
}

int FileSystemAPI::deleteFile(string filename)
{
	string data;
	int type = 0;
	int index = fs.findObject(filename, type);
	if (index != -1)
	{
		if (type == 2)
		{
			int *oldBlocks;
			int count = fs.getBlockList(filename, oldBlocks);
			fs.deallocateBlock(fs.getBlockIndex(filename));
			for (int i = 0; i < count; i++)
			{
				fs.deallocateBlock(oldBlocks[i]);
			}
			fs.deleteObject(filename);
		}
	}
	else {
		cerr << filename << " does not exist." << endl;
	}
	
return		index;
}

int FileSystemAPI::changeDirectory(string s)
{
	int success = fs.changeDirectory(s);
	if (success == -1)
	{
		cerr << "No such directory exists." << endl;
	}
	return success;
}

int FileSystemAPI::editFile(string filename)
{
	string data;
	int type = 0;
	int index = fs.findObject(filename, type);
	if (index != -1)
	{
		if (type == 2)
		{
			deleteFile(filename);
			cout << "Modifying " << filename << endl;
			cout << "Press enter when done." << endl;
			getline(cin, data);
			int blkCount = (data.length() / 128) + 1;
			int *blks = new int[blkCount];
			bool fail = false;
			int j = 0;
			for (int i = 0; i < blkCount && !fail; i++)
			{
				blks[i] = fs.allocateBlock();
				j++;
				if (blks[i] == -1)
					fail = true;
			}
			index = createFile(filename);
			if (fail || index == -1)
			{
				cerr << "Not enough blocks." << endl;
				for (int i = 0; i < j; i++)
				{
					fs.deallocateBlock(blks[i]);
				}
			}
			else
			{
				string s;
				for (int i = 0; i < blkCount; i++)
				{
					s.clear();
					for (int j = i * 128; j < i * 128 + 128 && j < data.length(); j++)
					{
						s += data[j];
					}
					fs.writeBlock(blks[i], s);
				}
				fs.writeIndexBlock(index, blks, blkCount);
			}
		}
		else
		{
			cerr << filename << " is a folder." << endl;
		}
	}
	else {
		cerr << "File not found." << endl;
	}
	return index;
}

int FileSystemAPI::createFolder(string s)
{
	int allocatedBlk = fs.allocateBlock();
	if (allocatedBlk == -1)
	{
		cerr << "Out of blocks." << endl;
	}
	if (fs.createObject(s, allocatedBlk, 1) < 0)
	{
		fs.deallocateBlock(allocatedBlk);
		cerr << "Something went wrong." << endl;
		allocatedBlk = -1;
	}
	return allocatedBlk;
}

void FileSystemAPI::listDirectoryContents()
{
	vector<string> directories = fs.getDirectoryList();
	vector<string> files = fs.getFileList();
	for (int i = 0; i < directories.size(); i++)
	{
		cout << "\t" << "<DIR>" << "\t\t" << directories[i] << endl;
	}
	for (int i = 0; i < files.size(); i++)
	{
		cout << "\t\t\t" << files[i] << endl;
	}
}

string FileSystemAPI::getPath()
{
	return fs.getPath();
}

int FileSystemAPI::readFile(string filename)
{

	int *blks, blkCount;
	blkCount = fs.getBlockList(filename, blks);
	if (blkCount < 0)
	{
		cerr << "Something went wrong." << endl;
	}
	else
	{
		for (int i = 0; i < blkCount; i++)
		{
			if (i < blkCount - 1)
				cout << fs.readBlock(blks[i]);
			else
			{
				string s = fs.readBlock(blks[i]);
				cout << truncate(s, '$');
			}
		}
		cout << endl;
	}
	return blkCount;
}
