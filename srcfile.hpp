/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SRCFILE_HPP
#define SRCFILE_HPP

#include "source.hpp"
#include "nfile.hpp"
#include <unordered_set>
#include <stack>
#include <string>

class SrcFile: public SrcText {
private:
	NFile *nfile;

public:
	SrcFile(std::string const &fname);
	virtual ~SrcFile();

//	virtual std::string getLocation() const;
	virtual void Rewind();

	virtual bool getline(char *buffer, unsigned size);
};

class SrcFileList {
private:
	std::unordered_set<std::string> names;
	std::string first;
	std::stack<SrcText> files;

public:
	SrcFileList();

	bool isPresent(std::string const &name);
	void Add(std::string const &name);
	void Add(SrcText *source);
	bool Pop();
	void Reset();
	void AdvanceLine();
	std::string getLocation();
	//std::string &getFilename();
	//unsigned getLinenum();
	SrcText &getCurrent();
	bool getline(char *buffer, unsigned max);
	//NFile *GetNFile();
};

#endif
