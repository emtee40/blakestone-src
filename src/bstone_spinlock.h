/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 1992-2013 Apogee Entertainment, LLC
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com)
SPDX-License-Identifier: GPL-2.0-or-later
*/

#ifndef BSTONE_SPINLOCK_INCLUDED
#define BSTONE_SPINLOCK_INCLUDED

#include <atomic>

namespace bstone
{

class Spinlock
{
public:
	Spinlock() noexcept;
	Spinlock(Spinlock&& rhs) noexcept;
	void operator=(Spinlock&& rhs) noexcept;
	~Spinlock();

	bool try_lock() noexcept;
	void lock() noexcept;
	void unlock() noexcept;

private:
	using Flag = std::atomic_flag;

	Flag flag_{};
}; // Spinlock

} // bstone

#endif // !BSTONE_SPINLOCK_INCLUDED
