#include <iostream>
#include <fstream>
#include <string>
#include "FileSystem.h"
#include "Modules.h"
#define END_OF_BLOCK "$"


FileSystem::FileSystem(string filename): blockSize(128), totalBlocks(80), disk(filename)
{
	ifstream test(filename);	// verify disk status
	if (test.peek() == -1)
	{
		// disk is empty or does not exist
		resetDiskState();
	}
	path.push_back(0);
	pathName.push_back("root");
	test.close();
}

string FileSystem::getDirectoryName()
{
	return pathName.back();
}

string FileSystem::getPath()
{
	string s = "<";
	for (int i = 0; i < pathName.size(); i++)
	{
		s += pathName[i] + "\\";
	}
	s += "> ";
	return s;
}

int FileSystem::allocateBlock()
{
	Block blk1, blk2;
	disk.readBlock(0, &blk1);	// read block zero
	int freeBlock = stoi(blk1.readBytes(124, 128));	// read next free block from last 4 bytes of block zero
	if (freeBlock != 0)
	{
		disk.readBlock(freeBlock, &blk2);	// read freeBlock
		string temp = blk2.readBytes(124, 128);	// read last four bytes of freeBlock
		blk1.writeBytes(temp, 124);	// set next of freeBlock as head
		disk.writeBlock(0, blk1);	// write block back to disk
	}
	return freeBlock != 0 ? freeBlock : -1;
}

void FileSystem::deallocateBlock(int n)
{
	Block blk1, blk2;
	disk.readBlock(0, &blk1);
	string temp = blk1.readBytes(124, 128);
	blk1.writeBytes(itos(n, 4), 124);
	disk.readBlock(n, &blk2);
	blk2.writeBytes(temp, 124);
	disk.writeBlock(0, blk1);
	disk.writeBlock(n, blk2);
}

void FileSystem::resetDiskState()
{
	Block s;
	disk.toggleRepairMode();	// change disk to repair mode
	for (int i = 0; i < totalBlocks; i++)
	{
		if (i < totalBlocks - 1)
		{
			s.writeBytes(itos(i + 1, 4), 124);
		} else {
			s.writeBytes(itos(0, 4), 124);
		}
		disk.writeBlock(i, s);
		s.writeBytes(END_OF_BLOCK);
	}
	disk.toggleRepairMode();	// revert disk to normal mode
}

int FileSystem::createObject(string filename, int startBlock, int type)
{
	Block blk;
	disk.readBlock(path.back(), &blk);
	int foundType = 0;
	int startByte = -1;
	int exists = findObject(filename, startByte);
	if (exists == -1)
	{
		startByte = findFreeSlot(blk);
		filename.resize(11, '$');
		blk.writeBytes(itos(type, 0) + filename + itos(startBlock, 4), startByte);
		disk.writeBlock(path.back(), blk);
	}
	else {
		startByte = -1;
	}
	return startByte;
}

string FileSystem::readBlock(int blockNum)
{
	Block blk;
	disk.readBlock(blockNum, &blk);
	return blk.readBytes();
}


int FileSystem::deleteObject(string filename)
{
	Block blk;
	disk.readBlock(path.back(), &blk);
	int type;
	int startByte = findObject(filename, type);
	if (startByte != -1)
	{

		blk.writeBytes("0", startByte);
		disk.writeBlock(path.back(), blk);
	}
	return startByte;
}

int FileSystem::findObject(string filename, int &type)
{
	Block blk;
	disk.readBlock(path.back(), &blk);
	string name;
	int fileBlock = -1;
	for (int i = 0; i < 128 && fileBlock == -1; i += 16)
	{
		name = blk.readBytes(i + 1, i + 12);
		if (truncate(name, '$') == filename)
		{
			fileBlock = i;
			type = stoi(blk.readBytes(i, i + 1));
			fileBlock = type == 0 ? -1 : fileBlock;
		}
	}
	return fileBlock;
}

int FileSystem::changeDirectory(string dir)
{
	int type;
	int folder = findObject(dir, type);
	Block blk;
	disk.readBlock(path.back(), &blk);
	if (dir == "..")
	{
		if (path.size() > 1)
		{
			path.pop_back();
			pathName.pop_back();
		}
		folder = 1;
	}
	else if (folder != -1)
	{
		if (type == 1)
		{
			path.push_back(stoi(blk.readBytes(folder + 12, folder + 16)));
			pathName.push_back(dir);
		} else {
			folder = -1;
		}
	}
	return folder;
}

vector<string> FileSystem::getDirectoryList()
{
	Block blk;
	vector<string> dir;
	disk.readBlock(path.back(), &blk);
	string name, type;
	for (int i = 0; i < 128; i += 16)
	{
		type = (blk.readBytes(i, i + 1));
		if (type != " ")
		{
			name = blk.readBytes(i + 1, i + 12);
			if (type == "1")
			{
				dir.push_back(truncate(name, '$'));
			}
		}
	}
	return dir;
}
vector<string> FileSystem::getFileList()
{
	Block blk;
	vector<string> files;
	disk.readBlock(path.back(), &blk);
	string name, type;
	for (int i = 0; i < 128; i += 16)
	{
		type = (blk.readBytes(i, i + 1));
		if (type != " ")
		{
			name = blk.readBytes(i + 1, i + 12);
			if (type == "2")
			{
				files.push_back(truncate(name, '$'));
			}
		}
	}
	return files;
}

int FileSystem::findFreeSlot(Block blk)
{
	int index = -1;
	for (int i = 0; i < 128 && index == -1; i += 16)
	{
		string s = blk.readBytes(i, i + 1);
		if (s == "0" || s == " " || s == "$")
		{
			index = i;
		}
	}
	return index;
}

int FileSystem::allocateIndexBlock()
{
	int index = allocateBlock();
	if (index != -1)
	{
		Block blk;
		disk.readBlock(index, &blk);
		blk.writeBytes("$");
		disk.writeBlock(index, blk);
	}
	return index;
}

int FileSystem::getBlockList(string filename, int *&arr)
{
	int type = 0;
	int startByte = findObject(filename, type);

	if (startByte == -1)
		return -1;
	if (type != 2)
		return -1;

	Block blk;
	disk.readBlock(path.back(), &blk);
	int indexBlock = stoi(blk.readBytes(startByte + 12, startByte + 16));
	disk.readBlock(indexBlock, &blk);
	int i;
	bool eob = false;
	for (i = 0; i < 128 && !eob; i += 4)
	{
		if (blk[i] == '$')
		{
			eob = true;
			i -= 4;
		}
	}
	int n_size = i / 4;
	arr = new int[n_size];
	int k = 0;
	for (int j = 0; j < i; j += 4)
	{
		arr[k] = stoi(blk.readBytes(j, j + 4));
		k++;
	}
	return n_size;
}

void FileSystem::writeIndexBlock(int blockNum, int arr[], int size)
{
	Block blk;
	int i;
	int j = 0;
	for (i = 0; i < size; i++)
	{
		blk.writeBytes(itos(arr[i], 4), j);
		j += 4;
	}
	blk.writeBytes("$", j);
	disk.writeBlock(blockNum, blk);
}

int FileSystem::getBlockIndex(string s)
{
	int type = 0;
	int start = findObject(s, type);
	if (start != -1)
	{
		Block blk;
		disk.readBlock(path.back(), &blk);
		start = stoi(blk.readBytes(start + 12, start + 16));
	}
	return start;
}

void FileSystem::writeBlock(int blkNum, string s)
{
	s.resize(128, '$');
	Block blk;
	disk.readBlock(blkNum, &blk);
	blk.writeBytes(s);
	disk.writeBlock(blkNum, blk);
}