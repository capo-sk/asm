/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include <cstdio>
#include <cstring>
#include "srcfile.hpp"

// class SrcFile

SrcFile::SrcFile(char const *name) {
	next = NULL;
	nfile = new NFile(name, "r");
	line = 1;
}

SrcFile::~SrcFile() {
	if (nfile) delete nfile;
}

void SrcFile::AdvanceLine() {
	line++;
}

void SrcFile::Rewind() {
	nfile->rewind();
	line = 1;
}

// class FileName

FileName::FileName(char const *a_name) {
	next = NULL;
	name = strdup(a_name);
}

FileName::~FileName() {
	if (name) delete name;
}

// class SrcFileList

SrcFileList::SrcFileList() {
	first = last = NULL;
	top = NULL;
}

void SrcFileList::Add(char const *name) {
	SrcFile *fentry = new SrcFile(name);
	FileName *nentry = new FileName(name);

	printf("File: %s\n", name);

	// entry is new top of the stack
	fentry->next = top;
	top = fentry;

	// add entry to the end of the list
	if (last == NULL)
		first = nentry;
	else
		last->next = nentry;
	last = nentry;
}

bool SrcFileList::isPresent(char const *name) {
	FileName *p;

	for (p = first; p != NULL; p = p->next)
		if (strcmp(name, p->name) == 0)
			return p;

	return NULL;
}

bool SrcFileList::Pop() {
	SrcFile *p;

	if (top != NULL && top->next != NULL) {  // do not pop the initial file
		p = top;
		top = p->next;
		delete p;
		return true;
	} else
		return false;
}

void SrcFileList::Reset() {
	FileName *p, *q;

	while (Pop()) { }

	if (first != NULL) {
		p = first->next;
		while (p != NULL) {
			q = p->next;
			delete p;
			p = q;
		}
		first->next = NULL;
	}
	last = first;
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

char const *SrcFileList::getFilename() {
	if (top != NULL)
		return top->getFilename();
	else
		return NULL;
}

unsigned SrcFileList::getLinenum() {
	if (top != NULL)
		return top->getLinenum();
	else
		return 0;
}

