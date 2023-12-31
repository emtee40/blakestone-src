/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

#include "bstone_exception.h"
#include "bstone_sys_window.h"

namespace bstone {
namespace sys {

const char* Window::get_title()
BSTONE_BEGIN_FUNC_TRY
	return do_get_title();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Window::set_title(const char* title)
BSTONE_BEGIN_FUNC_TRY
	do_set_title(title);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

WindowPosition Window::get_position()
BSTONE_BEGIN_FUNC_TRY
	return do_get_position();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Window::set_position(WindowPosition position)
BSTONE_BEGIN_FUNC_TRY
	do_set_position(position);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Window::center()
BSTONE_BEGIN_FUNC_TRY
	do_set_position(WindowPosition{window_position_centered, window_position_centered});
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

WindowSize Window::get_size()
BSTONE_BEGIN_FUNC_TRY
	return do_get_size();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Window::set_size(WindowSize size)
BSTONE_BEGIN_FUNC_TRY
	do_set_size(size);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Window::show(bool is_visible)
BSTONE_BEGIN_FUNC_TRY
	do_show(is_visible);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

bool Window::is_fake_fullscreen()
BSTONE_BEGIN_FUNC_TRY
	return do_is_fake_fullscreen();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Window::set_fake_fullscreen(bool is_fake_fullscreen)
BSTONE_BEGIN_FUNC_TRY
	do_set_fake_fullscreen(is_fake_fullscreen);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

GlContextUPtr Window::make_gl_context()
BSTONE_BEGIN_FUNC_TRY
	return do_make_gl_context();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

WindowSize Window::gl_get_drawable_size()
BSTONE_BEGIN_FUNC_TRY
	return do_gl_get_drawable_size();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Window::gl_swap_buffers()
BSTONE_BEGIN_FUNC_TRY
	do_gl_swap_buffers();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

RendererUPtr Window::make_renderer(const RendererInitParam& param)
BSTONE_BEGIN_FUNC_TRY
	return do_make_renderer(param);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

} // namespace sys
} // namespace bstone
