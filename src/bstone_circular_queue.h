/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/


#ifndef BSTONE_CIRCULAR_QUEUE_INCLUDED
#define BSTONE_CIRCULAR_QUEUE_INCLUDED


#include <cassert>

#include <memory>
#include <new>
#include <type_traits>

#include "bstone_exception.h"
#include "bstone_int.h"
#include "bstone_utility.h"


namespace bstone
{


template<
	typename TElement
>
class CircularQueueIterator
{
public:
	using Element = TElement;


	CircularQueueIterator() noexcept = default;

	CircularQueueIterator(
		Element* elements,
		IntP capacity,
		IntP size,
		IntP index) noexcept
		:
		elements_{elements},
		capacity_{capacity},
		size_{size},
		index_{index}
	{
		assert(capacity_ >= 0);
		assert(size_ >= 0);
		assert(index_ >= 0 && index < capacity_);
	}

	Element* operator->() const noexcept
	{
		assert(elements_);
		assert(size_ > 0);

		return elements_ + index_;
	}

	Element& operator*() const noexcept
	{
		assert(elements_);
		assert(size_ > 0);

		return elements_[index_];
	}

	CircularQueueIterator& operator++() noexcept
	{
		assert(elements_);
		assert(capacity_ > 0);
		assert(size_ > 0);

		size_ -= 1;

		if (size_ > 0)
		{
			index_ -= 1;

			if (index_ < 0)
			{
				index_ = capacity_ - 1;
			}
		}
		else
		{
			elements_ = nullptr;
			capacity_ = 0;
			index_ = 0;
		}

		return *this;
	}

	static bool are_equal(
		const CircularQueueIterator& lhs,
		const CircularQueueIterator& rhs) noexcept
	{
		return
			lhs.elements_ == rhs.elements_ &&
			lhs.capacity_ == rhs.capacity_ &&
			lhs.size_ == rhs.size_ &&
			lhs.index_ == rhs.index_;
	}


private:
	Element* elements_{};
	IntP capacity_{};
	IntP size_{};
	IntP index_{};
}; // CircularQueueIterator

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

template<
	typename TElement
>
inline bool operator==(
	const CircularQueueIterator<TElement>& lhs,
	const CircularQueueIterator<TElement>& rhs) noexcept
{
	return CircularQueueIterator<TElement>::are_equal(lhs, rhs);
}

template<
	typename TElement
>
inline bool operator!=(
	const CircularQueueIterator<TElement>& lhs,
	const CircularQueueIterator<TElement>& rhs) noexcept
{
	return !(lhs == rhs);
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

template<
	typename TElement
>
class CircularQueue
{
public:
	using Element = TElement;
	using CIterator = CircularQueueIterator<const Element>;


	CircularQueue() noexcept = default;

	CircularQueue(
		CircularQueue&& rhs) noexcept
		:
		storage_{std::move(rhs.storage_)}
	{
		bstone::swop(capacity_, rhs.capacity_);
		bstone::swop(size_, rhs.size_);
		bstone::swop(front_index_, rhs.front_index_);
		bstone::swop(back_index_, rhs.back_index_);
	}

	CircularQueue& operator=(
		const CircularQueue& rhs) = delete;

	~CircularQueue()
	{
		clear();
	}


	IntP get_capacity() const noexcept
	{
		return capacity_;
	}

	IntP get_size() const noexcept
	{
		return size_;
	}

	bool is_empty() const noexcept
	{
		return get_size() == 0;
	}

	bool is_full() const noexcept
	{
		return get_size() == get_capacity();
	}


	const Element& get_front() const & noexcept
	{
		return *get_front_ptr();
	}

	Element& get_front() & noexcept
	{
		return *get_front_ptr();
	}

	Element&& get_front() && noexcept
	{
		return std::move(*get_front_ptr());
	}


	void clear() noexcept
	{
		clear(ElementTag{});
	}

	void set_capacity(
		IntP capacity)
	{
		assert(capacity >= 0);

		if (!is_empty())
		{
			BSTONE_THROW_STATIC_SOURCE("Non-empty queue.");
		}

		if (get_capacity() == capacity)
		{
			return;
		}

		if (capacity > 0)
		{
			storage_.reset(new char[sizeof(Element) * capacity]);
		}
		else
		{
			storage_ = nullptr;
		}

		capacity_ = capacity;
		front_index_ = 0;
		reset_back_index();
	}


	template<
		typename ...TArgs
	>
	auto emplace(
		TArgs&& ...args)
	{
		if (get_size() == get_capacity())
		{
			BSTONE_THROW_STATIC_SOURCE("No free elements.");
		}

		const auto next_back_index = increment_index(back_index_);
		const auto element_ptr = get_ptr(next_back_index);

		new (element_ptr) Element(std::forward<TArgs>(args)...);

		size_ += 1;
		back_index_ = next_back_index;

		return *element_ptr;
	}

	template<
		typename UElement = Element
	>
	auto push(
		UElement&& element)
	{
		return emplace(std::forward<UElement>(element));
	}

	void pop() noexcept
	{
		assert(!is_empty());

		destruct_front_element(ElementTag{});

		size_ -= 1;
		front_index_ = increment_index(front_index_);
	}

	CIterator begin() const noexcept
	{
		if (is_empty())
		{
			return end();
		}
		else
		{
			return CIterator{
				reinterpret_cast<const Element*>(storage_.get()),
				capacity_,
				size_,
				front_index_
			};
		}
	}

	CIterator end() const noexcept
	{
		return CIterator{};
	}

	static void swap(
		CircularQueue& lhs,
		CircularQueue& rhs) noexcept
	{
		assert(std::addressof(lhs) != std::addressof(rhs));

		bstone::swop(lhs.storage_, rhs.storage_);

		bstone::swop(lhs.capacity_, rhs.capacity_);
		bstone::swop(lhs.size_, rhs.size_);
		bstone::swop(lhs.front_index_, rhs.front_index_);
		bstone::swop(lhs.back_index_, rhs.back_index_);
	}


private:
	struct TrivialTag{};
	struct NonTrivialTag{};

	using ElementTag = std::conditional_t<
		std::is_trivial<Element>::value,
		TrivialTag,
		NonTrivialTag
	>;

	using Storage = std::unique_ptr<char[]>;


	Storage storage_{};

	IntP capacity_{};
	IntP size_{};
	IntP front_index_{};
	IntP back_index_{};


	Element* get_ptr(
		IntP index) noexcept
	{
		assert(index >= 0 && index < capacity_);

		return reinterpret_cast<Element*>(storage_.get()) + index;
	}

	Element* get_front_ptr() noexcept
	{
		return get_ptr(front_index_);
	}


	void reset_back_index()
	{
		back_index_ = capacity_ - 1;
	}

	IntP increment_index(
		IntP index) noexcept
	{
		return (index + 1) % capacity_;
	}

	void destruct_front_element(
		TrivialTag) noexcept
	{
	}

	void destruct_front_element(
		NonTrivialTag) noexcept
	{
		(*get_ptr(front_index_)).~Element();
	}

	void clear(
		TrivialTag) noexcept
	{
		size_ = 0;
		front_index_ = 0;
		reset_back_index();
	}

	void clear(
		NonTrivialTag) noexcept
	{
		while (!is_empty())
		{
			pop();
		}
	}
}; // CircularQueue

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


// <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

template<
	typename TElement
>
inline void swap(
	CircularQueue<TElement>& lhs,
	CircularQueue<TElement>& rhs) noexcept
{
	CircularQueue<TElement>::swap(lhs, rhs);
}

// >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


} // bstone


#endif // !BSTONE_CIRCULAR_QUEUE_INCLUDED
