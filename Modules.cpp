#include <iostream>
#include "Modules.h"

string itos(int value, int len)
{
	string s = to_string(value);
	while (s.length() < len)
	{
		s = "0" + s;
	}
	return s;
}

string truncate(string& s, char c)
{
	int i = s.find_first_of(c);
	if (i >= 0)
	{
		return s.substr(0, i);
	}
	return s;
}

void strTok(string s, string dst[])
{
	int i = s.find_first_of(' ');
	int j;
	dst[0] = "-1";
	dst[0] = s.substr(0, i);
	dst[1] = s.substr(i + 1, s.length());
}

string lowercase(string s)
{
	string s2;
	for (int i = 0; i < s.length(); i++)
	{
		if (s[i] >= 'A' && s[i] <= 'Z')
		{
			s2 += s[i] + ('a' - 'A');
		}
		else {
			s2 += s[i];
		}
	}
	return s2;
}