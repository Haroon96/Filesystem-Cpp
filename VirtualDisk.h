#ifndef V_DISK
#define V_DISK

#include "Block.h"
#include <fstream>
using namespace std;

class VirtualDisk
{
	fstream disk;
	string diskname;
	bool repairMode;

public:
	VirtualDisk(string);
	~VirtualDisk();
	/*	
		disk operates in two modes, repair and normal 
		in normal mode, you can R/W to disk but in repair,
		you can only write to the disk. Repair mode is used
		to build the disk from scratch (empty file)
	*/
	bool toggleRepairMode();
	/* writes a block to the disk */
	void writeBlock(int, Block);
	/* reads a block from disk */
	void readBlock(int, Block*);
};

#endif