#pragma once

#include <sstream>

class HSpace {
	char fillChar;
	
public:
	unsigned int width;

	friend std::ostream& operator<<(std::ostream& lhs, const HSpace& rhs);
	HSpace() :
		fillChar(' '),
		width(1)
	{}
	
	HSpace(char fillChar) :
		fillChar(fillChar),
		width(1)
	{}
};

std::ostream& operator<<(std::ostream& lhs, const HSpace& rhs)
{
	
	for(unsigned int i = 0; i < rhs.width; i++) {
		lhs.put(rhs.fillChar);
	}
	
	return lhs;
};