/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com)
SPDX-License-Identifier: GPL-2.0-or-later
*/


//
// A text writer for a stream.
//


#ifndef BSTONE_TEXT_WRITER_INCLUDED
#define BSTONE_TEXT_WRITER_INCLUDED


#include <string>

#include "bstone_stream.h"


namespace bstone
{


// A binary reader for a block of memory.
class TextWriter final
{
public:
	TextWriter();

	TextWriter(
		Stream* stream);

	TextWriter(
		TextWriter&& rhs);


	// Opens the writer.
	bool open(
		Stream* stream);

	// Closes the writer.
	void close();

	// Returns true if the writer is initialized or
	// false otherwise.
	bool is_initialized() const;

	// Writes a string.
	//
	// Returns true on success or false otherwise.
	bool write(
		const std::string& string);


private:
	Stream* stream_;
}; // TextWriter


} // bstone


#endif // !BSTONE_TEXT_WRITER_INCLUDED
