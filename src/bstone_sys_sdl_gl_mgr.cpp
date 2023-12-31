/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2022 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

#include "SDL_video.h"
#include "bstone_single_memory_pool.h"
#include "bstone_sys_sdl_exception.h"
#include "bstone_sys_sdl_gl_mgr.h"
#include "bstone_sys_sdl_gl_shared_library.h"

namespace bstone {
namespace sys {

namespace {

class SdlGlMgr final : public GlMgr
{
public:
	SdlGlMgr(Logger& logger);
	SdlGlMgr(const SdlGlMgr&) = delete;
	SdlGlMgr& operator=(const SdlGlMgr&) = delete;
	~SdlGlMgr() override;

	static void* operator new(std::size_t size);
	static void operator delete(void* ptr);

private:
	Logger& logger_;

private:
	GlSharedLibraryUPtr do_make_shared_library(const char* path) override;

	bool do_has_extension(const char* extension_name) override;

	int do_get_swap_interval() noexcept override;
	void do_set_swap_interval(int swap_interval) override;
};

// ==========================================================================

using SdlGlMgrPool = SingleMemoryPool<SdlGlMgr>;
SdlGlMgrPool sdl_gl_mgr_pool{};

// ==========================================================================

SdlGlMgr::SdlGlMgr(Logger& logger)
	:
	logger_{logger}
{
	logger_.log_information("Start up SDL OpenGL manager.");
}

SdlGlMgr::~SdlGlMgr()
{
	logger_.log_information("Shut down SDL OpenGL manager.");
}

void* SdlGlMgr::operator new(std::size_t size)
BSTONE_BEGIN_FUNC_TRY
	return sdl_gl_mgr_pool.allocate(size);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void SdlGlMgr::operator delete(void* ptr)
BSTONE_BEGIN_FUNC_TRY
	sdl_gl_mgr_pool.deallocate(ptr);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

GlSharedLibraryUPtr SdlGlMgr::do_make_shared_library(const char* path)
{
	return make_sdl_gl_shared_library(path);
}

bool SdlGlMgr::do_has_extension(const char* extension_name)
{
	return SDL_GL_ExtensionSupported(extension_name) == SDL_TRUE;
}

int SdlGlMgr::do_get_swap_interval() noexcept
{
	return SDL_GL_GetSwapInterval();
}

void SdlGlMgr::do_set_swap_interval(int swap_interval)
{
	sdl_ensure_result(SDL_GL_SetSwapInterval(swap_interval));
}

} // namespace

// ==========================================================================

GlMgrUPtr make_sdl_gl_mgr(Logger& logger)
{
	return std::make_unique<SdlGlMgr>(logger);
}

} // namespace sys
} // namespace bstone
