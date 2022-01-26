#include <iostream>
#include <string>
#include "VirtualDisk.h"

VirtualDisk::VirtualDisk(string filename)
{
	disk.open(filename, ios::in | ios::out);
	/*
	Opening a file with these flags prevents
	truncation and allows both R/W operations.
	However, it does not allow writing more bytes
	than those that already exist in the file.
	*/
	repairMode = false;
	diskname = filename;
}

VirtualDisk::~VirtualDisk()
{
	disk.close();
}

bool VirtualDisk::toggleRepairMode()
{
	if (repairMode == true)
	{
		// disk is in repair mode
		// switch to normal mode
		disk.close();
		disk.open(diskname, ios::in | ios::out);
		repairMode = false;
	}
	else {
		// disk is in normal mode
		// switch to repair mode
		disk.close();
		disk.open(diskname, ios::out);
		repairMode = true;
	}
	return repairMode;
}

void VirtualDisk::writeBlock(int blockNo, Block data)
{
	disk.clear();
	disk.seekp(blockNo * 128);
	for (int i = 0; i < 128; i++)
	{
		disk.put(data[i]);	// write bytes to the disk from block
	}
}

void VirtualDisk::readBlock(int blockNo, Block *blk)
{
	disk.clear();
	disk.seekg(blockNo * 128);
	for (int i = 0; i < 128; i++)
	{
		blk->writeBytes(disk.get(), i);	// write bytes to the block holder from disk
	}
}