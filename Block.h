#ifndef BLOCK_DATA
#define BLOCK_DATA

#include <string>
using namespace std;

class Block
{
	string data;

public:
	Block(string="");
	/* reads a ranged number of bytes */
	string readBytes(int = 0, int = 128);
	/* writes a single byte at specified index */
	void writeBytes(char, int = 0);
	/* writes a stream of bytes starting from an index */
	void writeBytes(string, int = 0);
	/* operator overload to allow controlled access to string */
	char operator[](int);
};

#endif