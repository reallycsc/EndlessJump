#pragma once

#include "CommonHeader.h"

// string functions
inline const string eraseStringNonNumber(const string &str)
{
	string tmpStr = str;
	int index = tmpStr.size() - 1;
	while (index != -1)
	{
		if (tmpStr[index] < '0' || tmpStr[index] > '9')
		{
			tmpStr.erase(index, 1);
			index--;
		}
		else
		{
			break;
		}
	}
	return tmpStr;
}

// color functions
inline int getRandomColorIdx(int start, int count, int current)
{
	return (start + current * COLOR_RING24_COUNT / count) % COLOR_RING24_COUNT;
}

inline int getContrayColorIdx(int idx)
{
	return (idx + 12) % COLOR_RING24_COUNT;
}

inline Color3B getAddColorDiff(const Color3B &start, int diff_r, int diff_g, int diff_b)
{
	return Color3B((start.r + diff_r) % 255, (start.g + diff_g) % 255, (start.b + diff_b) % 255);
}