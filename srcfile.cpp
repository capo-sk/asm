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

void SrcFile::rewind()
{
	nfile->rewind();
	SrcText::rewind();
}

bool SrcFile::getline(char *buffer, unsigned size)
{
	nfile->getline(buffer, size);
	if (buffer[0] == 0 && nfile->eof())
		return false;
	else {
		advance_line();
		return true;
	}
}

// class SrcFileList

SrcStack::SrcStack(string const &fname)
: SrcText(fname)
{
	new_source_once(new SrcFile(fname));
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

void SrcStack::new_source(SrcText *source)
{
	sources.push(source);
}

void SrcStack::new_source_once(SrcText *source)
{
	string name = source->get_name();

	if (!is_present(name)) {
		if (names.size() == 0)
			first = name;
		names.insert(name);

		new_source(source);
	}
}

bool SrcStack::is_present(std::string const &name)
{
	return names.count(name) > 0;
}

bool SrcStack::Pop()
{
	if (sources.size() > 1) {
		sources.pop();
		return true;
	} else
		return false;
}

void SrcStack::reset()
{
	names.clear();
	names.insert(first);

	while (sources.size() > 1) {
		sources.pop();
	}

	rewind();
}

std::string SrcStack::get_location()
{
	return sources.top()->get_location();
}

SrcText &SrcStack::get_current()
{
	return *sources.top();
}

bool SrcStack::getline(char *buffer, unsigned max)
{
	return get_current().getline(buffer, max);
}

void SrcStack::rewind()
{
	get_current().rewind();
}
