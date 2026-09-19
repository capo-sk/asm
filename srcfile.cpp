/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "srcfile.hpp"

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

#if 0
void SrcFileList::AddFile(std::string const &name)
{
	SrcText *next = new SrcFile(name);

	sources.push(next);

	if (names.size() == 0)
		first = name;
	names.insert(name);
}
#endif

void SrcFileList::Add(SrcText *source)
{
	sources.push(source);
}

void SrcFileList::AddOnce(SrcText *source)
{
	string name = source->getName();

	if (names.count(name) == 0) {
		if (names.size() == 0)
			first = name;
		names.insert(name);

		Add(source);
	}
}

bool SrcFileList::isPresent(std::string const &name)
{
	return names.count(name) > 0;
}

bool SrcFileList::Pop()
{
	if (sources.size() > 1) {
		sources.pop();
		return true;
	} else
		return false;
}

void SrcFileList::Reset()
{
	while (sources.size() > 1) {
		sources.pop();
	}

	names.clear();
	names.insert(first);
}

void SrcFileList::AdvanceLine()
{
	sources.top()->AdvanceLine();
}

std::string SrcFileList::getLocation()
{
	return sources.top()->getLocation();
}

SrcText &SrcFileList::getCurrent()
{
	return *sources.top();
}

bool SrcFileList::getline(char *buffer, unsigned max)
{
	bool retval = getCurrent().getline(buffer, max);
	return retval;
}
