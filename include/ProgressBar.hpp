#pragma once

#include <iostream>

template <class ArithmeticType>
class ProgressBar {
private:
	unsigned int width;
	ArithmeticType min, max;
	ArithmeticType& value;
public:
	ProgressBar(unsigned int width, const ArithmeticType& min, const ArithmeticType& max, ArithmeticType& value) :
		width(width),
		min(min),
		max(max),
		value(value)
	{}
	
	template <class T>
	friend std::ostream& operator<<(std::ostream& lhs, const ProgressBar<T>& pb);
};

template <class ArithmeticType>
std::ostream& operator<<(std::ostream& lhs, const ProgressBar<ArithmeticType>& pb)
{
	lhs << "[";
	if(pb.value < pb.min) {
		//Value too small for pb
		for(unsigned int i = 0; i < pb.width; i++) lhs << " ";
	} else if(pb.value > pb.max) {
		//Value too big for pb
		for(unsigned int i = 0; i < pb.width; i++) lhs << "=";
	} else {
		unsigned int chars = pb.width * (pb.value - pb.min) / (pb.max - pb.min);
		for(unsigned int i = 0; i < chars; i++) {
			lhs << "=";
		}
		if(chars < pb.width) lhs << ">";
		for(unsigned int i = chars + 1; i < pb.width; i++) {
			lhs << " ";
		}
	}
	lhs << "]";
	return lhs;
}