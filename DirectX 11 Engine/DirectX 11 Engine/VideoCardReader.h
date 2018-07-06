#pragma once
#include <vector>

/*
This class is used to get information about the user's video card.
*/

struct DisplayMode
{
	int numerator;
	int denominator;
};

class VideoCardReader
{
public:
	bool Initialize();
	int GetVideoCardMemory();
	std::vector<DisplayMode> GetDisplayModes();
private:
	int videoCardMemory;
	std::vector<DisplayMode> displayModes;
}