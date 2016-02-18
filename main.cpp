#include <iostream>
#include "include/StaticTTY.h"

int main() {
    StaticTTY tty;
	
	size_t f = 0;
	float percent = 0;

	tty << HSpace('#') << "Title" << HSpace('#') << "\n";
	tty << "Float f = " << f << "\n";
	tty << HSpace('#') << "Even Table" << HSpace('#') << "\n";
	tty << "\n\n\n";
	for(int i = 0; i < 5; i++) {
		for(int j = 0; j < 5; j++) {
			tty << i*5+j;
			if(j < 4) tty << HSpace(' ');
		}
		tty << "\n";
	}
	tty << "\n\n\n";
	tty << HSpace(' ') << ProgressBar<float>(60, 0.0, 100.0, percent) << "   \n";
	
	while(true) {
		f++;
		percent += .5;
		if(percent > 120) percent = -20;
		std::this_thread::sleep_for(std::chrono::milliseconds(40));
	};
	tty.block();
	tty.finish();
	return 0;
}
