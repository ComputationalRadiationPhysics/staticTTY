/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2016  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 * 
 */

#pragma once

#include <iostream>
#include <cstdlib>
#include <thread>
#include <chrono>
#include <vector>
#include <sstream>
#include <string>
#include <algorithm>
#include <sys/ioctl.h>
#include <unistd.h>

#include "RunRef.hpp"
#include "hspace.hpp"
#include "vspace.hpp"
#include "ProgressBar.hpp"

class StaticTTY
{
private:
	bool exit;
	std::chrono::milliseconds interval;
	
	std::thread runThread;
	std::vector<std::shared_ptr<RuntimeRefBase>> refs;
	
	unsigned int cols, rows;
	
	friend std::ostream& operator<<(std::ostream& lhs, const StaticTTY& rhs);

	void run();
	void parseSpacer();
	winsize getSizeOfTerminal() const;
	bool isResized();
public:
	
	const char* endl = "\n";

	StaticTTY();
	
	template <class Type>
	StaticTTY& operator<<(const Type& rhs);
	
	template <class Type>
	StaticTTY& operator<<(Type& rhs);
	
	StaticTTY& operator<<(const char rhs[]);
	
	void block();
	void finish();
};

std::ostream& operator<<(std::ostream& lhs, const StaticTTY& rhs)
{
	//std::cout << lines << " " << columns << std::endl;
	lhs << "\033[" << 0 << ";" << 0 << "f";
	for(const auto s : rhs.refs) {
		lhs << s->toString();
	}

	return lhs;
}

StaticTTY::StaticTTY() :
	exit(false),
	interval(1000/25), //25fps
	runThread([this] { this->run(); })
{
}

void StaticTTY::run()
{
	while(!exit) {
		std::this_thread::sleep_for(interval);
		parseSpacer();
		std::cout << *this << std::flush;
	}
}

void StaticTTY::parseSpacer()
{
	//Reset terminal is needed, if windows is resized or spacings are altered
	if(isResized()) std::cout << "\033[2J" << std::endl;
	
	std::stringstream outputBuff;
	unsigned int numOfVSpacers = 0;
	for(const auto s : refs) {
		if(s->getSpacer() != RuntimeRefBase::Spacer::None) {
			if(s->getSpacer() == RuntimeRefBase::Spacer::HSpacer) {
				dynamic_cast<ConstRuntimeRef<HSpace>*>(s.get())->getRef().width = 0;
			}
			if(s->getSpacer() == RuntimeRefBase::Spacer::VSpacer) {
				dynamic_cast<ConstRuntimeRef<VSpace>*>(s.get())->getRef().height = 0;
				numOfVSpacers++;
			}
		} else {
			outputBuff << s->toString();
		}
	}
	
	std::vector<unsigned int> colLengths;
	std::string line;
	
	while(std::getline(outputBuff, line)) {
		colLengths.push_back(line.size());
	}
	
	int lineNumber = 0;
	std::vector<std::reference_wrapper<unsigned int>> allSpacersOnLine;

	for(std::shared_ptr<RuntimeRefBase> baseRef : refs) {
		switch(baseRef->getSpacer()) {
			case RuntimeRefBase::Spacer::None:
			{
				std::string refStr = baseRef->toString();
				auto begin = refStr.begin();
				do {
					auto linebreak = std::find(begin, refStr.end(), '\n');
					if(linebreak != refStr.end()) {
						for(std::reference_wrapper<unsigned int> wn : allSpacersOnLine) {
							if(cols > colLengths.at(lineNumber)) {
								wn.get() = (cols - colLengths.at(lineNumber)) / allSpacersOnLine.size();
							} else {
								wn.get() = 0;
							}
						}
						
						if(
							allSpacersOnLine.size() > 0 and 
							(cols - colLengths.at(lineNumber)) % allSpacersOnLine.size() > 0
						) {
							allSpacersOnLine.front()++;
						}
						
						allSpacersOnLine.clear();
						lineNumber++;	
						linebreak++;
					}
					begin = linebreak;
				} while(begin != refStr.end());
			}
				break;
			case RuntimeRefBase::Spacer::HSpacer:
			{
				unsigned int& width = dynamic_cast<ConstRuntimeRef<HSpace>*>(baseRef.get())->getRef().width;
				allSpacersOnLine.push_back(width);
				break;
			}
			case RuntimeRefBase::Spacer::VSpacer:
				if(rows > colLengths.size()) {
					unsigned int& height = dynamic_cast<ConstRuntimeRef<VSpace>*>(baseRef.get())->getRef().height;
					height = (rows - colLengths.size()) / numOfVSpacers;
					if(height > 0) height -= 1; //leave room for the cursor on the last line;
				} else {
					dynamic_cast<ConstRuntimeRef<VSpace>*>(baseRef.get())->getRef().height = 0;
				}
				//-1 because the lowest line must be empty to position cursor
				break;
		}
		//lhs << s->toString();
	}
	
}


winsize StaticTTY::getSizeOfTerminal() const
{
	winsize win;
	int err = ioctl (STDIN_FILENO, TIOCGWINSZ, reinterpret_cast<char*>(&win));
	if(err == 0) {
		return win;
	} else {
		return winsize();
	}
}


bool StaticTTY::isResized()
{
	const winsize ws = getSizeOfTerminal();
	if(ws.ws_col != cols || ws.ws_row != rows) {
		cols = ws.ws_col;
		rows = ws.ws_row;
		return true;
	}
	return false;
}

template <class Type>
StaticTTY& StaticTTY::operator<<(const Type& rhs) {
	refs.push_back(std::make_shared<ConstRuntimeRef<Type>>(rhs));
	return *this;
}

template <class Type>
StaticTTY& StaticTTY::operator<<(Type& rhs) {
	refs.push_back(std::make_shared<RuntimeRef<Type>>(rhs));	
	return *this;
}

//	template <class Type>
StaticTTY& StaticTTY::operator<<(const char rhs[]) {
	const std::string ss(rhs);
	refs.push_back(std::make_shared<ConstRuntimeRef<decltype(ss)>>(ss));
	return *this;
}

void StaticTTY::block()
{
	runThread.join();
}

void StaticTTY::finish()
{
	exit = true;
	block();
}
