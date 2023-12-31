/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

#include "bstone_exception.h"
#include "bstone_sys_renderer.h"

namespace bstone {
namespace sys {

const char* Renderer::get_name() const
BSTONE_BEGIN_FUNC_TRY
	return do_get_name();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Renderer::set_viewport()
BSTONE_BEGIN_FUNC_TRY
	do_set_viewport(nullptr);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Renderer::set_viewport(const RendererViewport& viewport)
BSTONE_BEGIN_FUNC_TRY
	do_set_viewport(&viewport);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Renderer::clear()
BSTONE_BEGIN_FUNC_TRY
	do_clear();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Renderer::set_draw_color(Color color)
BSTONE_BEGIN_FUNC_TRY
	do_set_draw_color(color);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Renderer::fill(Span<const R2RectI> rects)
BSTONE_BEGIN_FUNC_TRY
	do_fill(rects);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Renderer::present()
BSTONE_BEGIN_FUNC_TRY
	do_present();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void Renderer::read_pixels(PixelFormat pixel_format, void* pixels, int pitch)
BSTONE_BEGIN_FUNC_TRY
	do_read_pixels(nullptr, pixel_format, pixels, pitch);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

TextureUPtr Renderer::make_texture(const TextureInitParam& param)
BSTONE_BEGIN_FUNC_TRY
	return do_make_texture(param);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

} // namespace sys
} // namespace bstone
