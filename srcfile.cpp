/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "srcfile.hpp"
#include <iostream>

using namespace std;

// class SrcFile

SrcFile::SrcFile(std::string const &name)
: SrcText(name)
{
	nfile = new NFile(name);
}

SrcFile::~SrcFile()
{
	if (nfile) delete nfile;
}

void SrcFile::Rewind()
{
	nfile->rewind();
	SrcText::Rewind();
}

bool SrcFile::getline(char *buffer, unsigned size)
{
	nfile->getline(buffer, size);
	if (buffer[0] == 0 && nfile->eof())
		return false;
	else
		return true;
}

// class SrcFileList

SrcFileList::SrcFileList()
{
}

void SrcFileList::Add(std::string const &name)
{
	SrcFile *next = new SrcFile(name);

	files.push(*next);

	if (names.size() == 0)
		first = name;
	names.insert(name);
}

bool SrcFileList::isPresent(std::string const &name)
{
	return names.count(name) > 0;
}

bool SrcFileList::Pop()
{
	if (files.size() > 1) {
		files.pop();
		return true;
	} else
		return false;
}

void SrcFileList::Reset()
{
	while (files.size() > 1) {
		files.pop();
	}

	names.clear();
	names.insert(first);
}

void SrcFileList::AdvanceLine()
{
	files.top().AdvanceLine();
}

//NFile *SrcFileList::GetNFile() {
//	return files.top().nfile;
//}

std::string SrcFileList::getLocation()
{
	return files.top().getLocation();
}

//std::string const &SrcFileList::getFilename()
//{
//	return files.top().getName();
//}

//unsigned SrcFileList::getLinenum()
//{
//	return files.top().getLine();
//}

SrcFile &SrcFileList::getCurrent()
{
	return files.top();
}

bool SrcFileList::getline(char *buffer, unsigned max)
{
	return getCurrent().getline(buffer, max);
}

