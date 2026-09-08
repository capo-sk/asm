/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include <cstring>
#include "srcfile.hpp"

// class SrcFile

SrcFile::SrcFile(char const *name) {
	stack_next = NULL;
	list_next = NULL;
	nfile = new NFile(name, "r");
	line = 1;
}

void SrcFile::AdvanceLine() {
	line++;
}

char const *SrcFile::GetName() {
	return nfile->GetName();
}

void SrcFile::Rewind() {
	nfile->rewind();
	line = 1;
}

// class SrcFileList

SrcFileList::SrcFileList() {
	first = last = top = NULL;
}

void SrcFileList::Add(char const *name) {
	SrcFile *entry = new SrcFile(name);

	// entry is new top of the stack
	entry->stack_next = top;
	top = entry;

	// add entry to the end of the list
	if (first == NULL)
		first = entry;
	else
		first->list_next = entry;
	last = entry;
}

SrcFile *SrcFileList::Find(char const *name) {
	SrcFile *p;

	for (p = first; p != NULL; p = p->list_next)
		if (strcmp(name, p->GetName()) == 0)
			return p;

	return NULL;
}

void SrcFileList::Pop() {
	if (top != NULL)
		top = top->stack_next;
}

void SrcFileList::AdvanceLine() {
	if (top != NULL)
		top->AdvanceLine();
}

NFile *SrcFileList::GetNFile() {
	if (top != NULL)
		return top->nfile;
	else
		return NULL;
}
