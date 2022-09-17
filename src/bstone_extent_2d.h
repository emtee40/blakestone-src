/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com)
SPDX-License-Identifier: GPL-2.0-or-later
*/


//
// The size of a rectangular region.
//


#ifndef BSTONE_EXTENT_2D_INCLUDED
#define BSTONE_EXTENT_2D_INCLUDED


namespace bstone
{


// ==========================================================================
// Extent2d
//

struct Extent2d
{
	int width_;
	int height_;
}; // Extent2d

//
// Extent2d
// ==========================================================================


bool operator==(
	const Extent2d& lhs,
	const Extent2d& rhs) noexcept;

bool operator!=(
	const Extent2d& lhs,
	const Extent2d& rhs) noexcept;


} // bstone


#endif // !BSTONE_EXTENT_2D_INCLUDED
