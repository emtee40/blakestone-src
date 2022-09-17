/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com)
SPDX-License-Identifier: GPL-2.0-or-later
*/


//
// Missing wall image (64x64).
//


#ifndef BSTONE_MISSING_WALL_64X64_IMAGE_INCLUDED
#define BSTONE_MISSING_WALL_64X64_IMAGE_INCLUDED


#include <array>


namespace bstone
{


using MissingWallImage = std::array<unsigned char, 4 * 64 * 64>;


const MissingWallImage& get_missing_wall_image();


} // bstone


#endif // !BSTONE_MISSING_WALL_64X64_IMAGE_INCLUDED
