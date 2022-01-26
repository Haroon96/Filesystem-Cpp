#ifndef FILE_SYSTEM
#define FILE_SYSTEM

#include "VirtualDisk.h"
#include <vector>
using namespace std;

class FileSystem
{
	VirtualDisk disk;
	/* vector to keep track of directory blocks that make up the path */
	vector<int> path;
	/* vector to keep track of the directory names */
	vector<string> pathName;
	const int blockSize;
	const int totalBlocks;

public:
	FileSystem(string);
	/* returns current directory name */
	string getDirectoryName();
	/* returns current directory path */
	string getPath();
	/* allocates a block and returns its index */
	int allocateBlock();
	/* finds a filename and returns its index and type */
	int findObject(string, int&);
	/* finds a free slot inside the current block */
	int findFreeSlot(Block);
	/* allocates an index block */
	int allocateIndexBlock();
	/* gets the list of blocks for a file */
	int getBlockList(string, int*&);
	/* deletes a file/folder on block, doesn't deallocate blocks */
	int deleteObject(string);
	int changeDirectory(string);
	/* creates a file/folder on block, doesn't allocate blocks */
	int createObject(string, int, int);
	/* reads the contents of a block from disk and returns it as string */
	string readBlock(int);
	/* builds a new disk from scratch */
	void resetDiskState();
	/* deallocates a block */
	void deallocateBlock(int);
	/* gets the list of files in a directory */
	vector<string> getFileList();
	/* gets the list of subdirectories in a directory */
	vector<string> getDirectoryList();
	/* writes an array of blocks to the index block of a file */
	void writeIndexBlock(int, int[], int);
	/* gets the index of the block allocated for a file/folder */
	int getBlockIndex(string);
	/* writes a new block number for a file */
	void writeBlock(int, string);
};

#endif