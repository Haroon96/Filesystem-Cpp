#include <iostream>
#include "Block.h"
#include "Modules.h"

Block::Block(string s)
{
	s.resize(128, ' ');
	data = s;
}

string Block::readBytes(int start, int end)
{
	string s;
	for (int i = start; i < end && i < 128; i++)
	{
		s += data[i];
	}
	return s;
}

void Block::writeBytes(char c, int index)
{
	data[index] = c;
}

void Block::writeBytes(string s, int start)
{
	int j = 0;
	for (int i = start; i < s.length() + start && i < 128; i++)
	{
		data[i] = s[j];
		j++;
	}
}

char Block::operator[](int index)
{
	if (index >= 0 && index < 128)
	{
		return data[index];
	}
	return -1;
}
