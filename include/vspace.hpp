#pragma once

class VSpace {
private:
public:
	unsigned int height;
	VSpace() {};
};

std::ostream& operator<<(std::ostream& lhs, const VSpace& rhs)
{
	
	for(unsigned int i = 0; i < rhs.height; i++) {
		lhs.put('\n');
	}
	
	return lhs;
};