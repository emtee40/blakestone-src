/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com)
SPDX-License-Identifier: GPL-2.0-or-later
*/


//
// A wrapper for std::ostringstream to produce a formatted string.
//


#ifndef BSTONE_FORMAT_STRING_INCLUDED
#define BSTONE_FORMAT_STRING_INCLUDED


#include <iomanip>
#include <sstream>


namespace bstone
{


// A wrapper for std::ostringstream to produce a formatted string.
class FormatString final
{
public:
	FormatString();

	operator std::string();

	std::string to_string() const;


	template<typename T>
	bstone::FormatString& operator<<(
		const T& value)
	{
		stream_ << value;
		return *this;
	}


private:
	std::ostringstream stream_;
}; // FormatString


} // bstone


#endif // !BSTONE_FORMAT_STRING_INCLUDED
