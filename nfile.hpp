/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef NFILE_HPP
#define NFILE_HPP

#include <fstream>
#include <string>

class NFile: public std::fstream {
private:
	std::string name;

public:
	NFile(std::string const &name, std::ios_base::openmode mode = std::ios_base::in);
	NFile(int which);
	//NFile();
	~NFile();
	std::string const &GetName() { return name; };
	void rewind();
	void printf(char const *fmt, ...);
};

#endif
