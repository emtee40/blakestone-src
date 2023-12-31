/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

#include <climits>
#include "SDL_rect.h"
#include "SDL_render.h"
#include "bstone_exception.h"
#include "bstone_single_memory_pool.h"
#include "bstone_sys_sdl_detail.h"
#include "bstone_sys_sdl_exception.h"
#include "bstone_sys_sdl_texture.h"
#include "bstone_sys_sdl_renderer.h"

namespace bstone {
namespace sys {

namespace {

static_assert(
	sizeof(SDL_Rect) == sizeof(R2RectI) &&
		offsetof(SDL_Rect, x) == offsetof(R2RectI, x) &&
		offsetof(SDL_Rect, y) == offsetof(R2RectI, y) &&
		offsetof(SDL_Rect, w) == offsetof(R2RectI, width) &&
		offsetof(SDL_Rect, h) == offsetof(R2RectI, height),
	"Unsupported R2RectI type.");

static_assert(
	sizeof(SDL_Rect) == sizeof(RendererViewport) &&
		offsetof(SDL_Rect, x) == offsetof(RendererViewport, x) &&
		offsetof(SDL_Rect, y) == offsetof(RendererViewport, y) &&
		offsetof(SDL_Rect, w) == offsetof(RendererViewport, width) &&
		offsetof(SDL_Rect, h) == offsetof(RendererViewport, height),
	"Unsupported RendererViewport type.");

// ==========================================================================

struct SdlRendererDeleter
{
	void operator()(SDL_Renderer* sdl_renderer)
	{
		SDL_DestroyRenderer(sdl_renderer);
	}
};

using SdlRendererUPtr = std::unique_ptr<SDL_Renderer, SdlRendererDeleter>;

// ==========================================================================

class SdlRenderer final : public Renderer
{
public:
	SdlRenderer(Logger& logger, SDL_Window& sdl_window, const RendererInitParam& param);
	SdlRenderer(const SdlRenderer&) = delete;
	SdlRenderer& operator=(const SdlRenderer&) = delete;
	~SdlRenderer() override;

	static void* operator new(std::size_t size);
	static void operator delete(void* ptr);

private:
	Logger& logger_;
	SdlRendererUPtr sdl_renderer_{};

private:
	const char* do_get_name() const override;

	void do_set_viewport(const RendererViewport* viewport) override;

	void do_clear() override;
	void do_set_draw_color(Color color) override;
	void do_fill(Span<const R2RectI> rects) override;
	void do_present() override;

	void do_read_pixels(const R2RectI* rect, PixelFormat pixel_format, void* pixels, int pitch) override;

	TextureUPtr do_make_texture(const TextureInitParam& param) override;

private:
	static SDL_PixelFormatEnum map_pixel_format(PixelFormat pixel_format);

	void log_flag(const char* flag, std::string& message);
	void log_flags(Uint32 flags, std::string& message);
	void log_texture_formats(const SDL_RendererInfo& info, std::string& message);
	void log_info();
};

// ==========================================================================

using SdlRendererPool = SingleMemoryPool<SdlRenderer>;
SdlRendererPool sdl_renderer_pool{};

// ==========================================================================

SdlRenderer::SdlRenderer(Logger& logger, SDL_Window& sdl_window, const RendererInitParam& param)
BSTONE_BEGIN_CTOR_TRY
	:
	logger_{logger}
{
	logger_.log_information("<<< Start up SDL renderer.");

	const auto sdl_flags = Uint32{param.is_vsync ? SDL_RENDERER_PRESENTVSYNC : 0U};
	sdl_renderer_ = SdlRendererUPtr{sdl_ensure_result(SDL_CreateRenderer(&sdl_window, -1, sdl_flags))};
	log_info();

	logger_.log_information(">>> SDL renderer started up.");
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

SdlRenderer::~SdlRenderer()
{
	logger_.log_information("<<< Shut down SDL renderer.");
}

void* SdlRenderer::operator new(std::size_t size)
BSTONE_BEGIN_FUNC_TRY
	return sdl_renderer_pool.allocate(size);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlRenderer::operator delete(void* ptr)
BSTONE_BEGIN_FUNC_TRY
	sdl_renderer_pool.deallocate(ptr);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

const char* SdlRenderer::do_get_name() const
BSTONE_BEGIN_FUNC_TRY
	auto sdl_renderer_info = SDL_RendererInfo{};
	sdl_ensure_result(SDL_GetRendererInfo(sdl_renderer_.get(), &sdl_renderer_info));
	return sdl_renderer_info.name;
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlRenderer::do_set_viewport(const RendererViewport* viewport)
{
	sdl_ensure_result(SDL_RenderSetViewport(
		sdl_renderer_.get(),
		reinterpret_cast<const SDL_Rect*>(viewport)));
}

void SdlRenderer::do_clear()
BSTONE_BEGIN_FUNC_TRY
	sdl_ensure_result(SDL_RenderClear(sdl_renderer_.get()));
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlRenderer::do_set_draw_color(Color color)
BSTONE_BEGIN_FUNC_TRY
	sdl_ensure_result(SDL_SetRenderDrawColor(sdl_renderer_.get(), color.r, color.g, color.b, color.a));
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlRenderer::do_fill(Span<const R2RectI> rects)
BSTONE_BEGIN_FUNC_TRY
	if (rects.get_size() > INT_MAX)
	{
		BSTONE_THROW_STATIC_SOURCE("Too many rectangles.");
	}

	sdl_ensure_result(SDL_RenderFillRects(
		sdl_renderer_.get(),
		reinterpret_cast<const SDL_Rect*>(rects.get_data()),
		static_cast<int>(rects.get_size())));
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlRenderer::do_present()
BSTONE_BEGIN_FUNC_TRY
	SDL_RenderPresent(sdl_renderer_.get());
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlRenderer::do_read_pixels(const R2RectI* rect, PixelFormat pixel_format, void* pixels, int pitch)
BSTONE_BEGIN_FUNC_TRY
	const auto sdl_pixel_format = map_pixel_format(pixel_format);

	sdl_ensure_result(SDL_RenderReadPixels(
		sdl_renderer_.get(),
		reinterpret_cast<const SDL_Rect*>(rect),
		sdl_pixel_format,
		pixels,
		pitch));
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

TextureUPtr SdlRenderer::do_make_texture(const TextureInitParam& param)
BSTONE_BEGIN_FUNC_TRY
	return make_sdl_texture(logger_, *sdl_renderer_, param);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

SDL_PixelFormatEnum SdlRenderer::map_pixel_format(PixelFormat pixel_format)
BSTONE_BEGIN_FUNC_TRY
	switch (pixel_format)
	{
		case PixelFormat::r8g8b8: return SDL_PIXELFORMAT_RGB24;
		default: BSTONE_THROW_STATIC_SOURCE("Unknown pixel format.");
	}
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlRenderer::log_flag(const char* name, std::string& message)
{
	message += "    ";
	message += name;
	detail::sdl_log_eol(message);
}

void SdlRenderer::log_flags(Uint32 flags, std::string& message)
{
	message += "  Flags:";
	detail::sdl_log_eol(message);

	if ((flags & SDL_RENDERER_SOFTWARE) != 0)
	{
		log_flag("software", message);
	}

	if ((flags & SDL_RENDERER_ACCELERATED) != 0)
	{
		log_flag("accelerated", message);
	}

	if ((flags & SDL_RENDERER_PRESENTVSYNC) != 0)
	{
		log_flag("vsync", message);
	}

	if ((flags & SDL_RENDERER_TARGETTEXTURE) != 0)
	{
		log_flag("target texture", message);
	}
}

void SdlRenderer::log_texture_formats(const SDL_RendererInfo& info, std::string& message)
{
	if (info.num_texture_formats == 0)
	{
		message += "  No texture formats.";
		detail::sdl_log_eol(message);
		return;
	}

	message += "  Texture formats:";
	detail::sdl_log_eol(message);

	for (auto i = decltype(info.num_texture_formats){}; i < info.num_texture_formats; ++i)
	{
		message += "    ";
		detail::sdl_log_xint(i + 1, message);
		message += ". \"";
		message += SDL_GetPixelFormatName(info.texture_formats[i]);
		message += '"';
		detail::sdl_log_eol(message);
	}
}

void SdlRenderer::log_info()
{
	auto message = std::string{};
	message.reserve(1024);

	auto sdl_info = SDL_RendererInfo{};
	sdl_ensure_result(SDL_GetRendererInfo(sdl_renderer_.get(), &sdl_info));

	message += "Effective parameters: \"";
	detail::sdl_log_eol(message);

	message += "  Name: \"";
	message += sdl_info.name;
	message += '"';
	detail::sdl_log_eol(message);

	message += "  Max texture width: ";
	detail::sdl_log_xint(sdl_info.max_texture_width, message);
	detail::sdl_log_eol(message);

	message += "  Max texture height: ";
	detail::sdl_log_xint(sdl_info.max_texture_height, message);
	detail::sdl_log_eol(message);

	log_flags(sdl_info.flags, message);
	log_texture_formats(sdl_info, message);

	logger_.log_information(message);
}

} // namespace

// ==========================================================================

RendererUPtr make_sdl_renderer(Logger& logger, SDL_Window& sdl_window, const RendererInitParam& param)
BSTONE_BEGIN_FUNC_TRY
	return std::make_unique<SdlRenderer>(logger, sdl_window, param);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

} // namespace sys
} // namespace bstone
