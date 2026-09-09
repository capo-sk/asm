/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SRCFILES_HPP
#define SRCFILES_HPP

#include "nfile.hpp"

class SrcFileList;

class SrcFile {
private:
	SrcFile *next;
	NFile *nfile;
	unsigned line;

	friend class SrcFileList;

public:
	SrcFile(char const *name);
	~SrcFile();

	unsigned getLinenum() { return line; }
	NFile *GetNFile() { return nfile; }
	char const *getFilename() { return nfile->GetName(); }

	void AdvanceLine();
	void Rewind();
};

struct FileName {
private:
	char const *name;
	FileName *next;

	friend class SrcFileList;

public:
	FileName(char const *a_name);
	~FileName();
	char const *getName() { return name; }
};

class SrcFileList {
private:
	FileName *first, *last;	// list of file names
	SrcFile *top;		// stack of files

public:
	SrcFileList();

	bool isPresent(char const *name);
	void Add(char const *name);
	bool Pop();
	void Reset();
	void AdvanceLine();
	char const *getFilename();
	unsigned getLinenum();
	SrcFile *GetCurrent() { return top; }
	NFile *GetNFile();
};

#endif
