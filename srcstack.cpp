/* 6502 Assembler

   Copyright 2026 William Brioschi aka CAPo/Spinning Kids

   See LICENSE file
*/

#include "srcstack.hpp"
#include "srcfile.hpp"

// class SrcFileList

SrcStack::SrcStack(string const &fname)
: SrcText(fname)
{
	new_source_once(new SrcFile(fname));
}

void SrcStack::new_source(SrcText *source)
{
	sources.push(source);
}

void SrcStack::new_source_once(SrcText *source)
{
	std::string name = source->get_name();

	if (!is_present(name)) {
		if (names.size() == 0)
			first = name;
		names.insert(name);

		new_source(source);
	}
}

bool SrcStack::is_present(string const &name) const
{
	return names.count(name) > 0;
}

bool SrcStack::pop()
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

std::string SrcStack::get_location() const
{
	return sources.top()->get_location();
}

SrcText &SrcStack::get_current()
{
	return *sources.top();
}

bool SrcStack::getline(std::string &buffer)
{
	return get_current().getline(buffer);
}

void SrcStack::rewind()
{
	get_current().rewind();
}
