/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#ifndef SRCFILES_H
#define SRCFILES_H

#include "nfile.hpp"

class SrcFileList;

class SrcFile {
private:
	SrcFile *stack_next;
	SrcFile *list_next;
	NFile *nfile;
	unsigned line;

	friend class SrcFileList;

public:
	SrcFile(char const *name);
	~SrcFile();

	char const *GetName();
	unsigned GetLine() { return line; }
	NFile *GetNfile() { return nfile; }

	void AdvanceLine();
	void Rewind();
};

class SrcFileList {
private:
	SrcFile *first, *last;	// as a list
	SrcFile *top;		// as a stack

public:
	SrcFileList();

	SrcFile *Find(char const *name);
	void Add(char const *name);
	void Pop();
	void AdvanceLine();
	NFile *GetNFile();
};

#endif
