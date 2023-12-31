/*
BStone: Unofficial source port of Blake Stone: Aliens of Gold and Blake Stone: Planet Strike
Copyright (c) 2013-2023 Boris I. Bendovsky (bibendovsky@hotmail.com) and Contributors
SPDX-License-Identifier: MIT
*/

// OpenGL 3D Renderer: Vertex Input Manager

#include "bstone_exception.h"
#include "bstone_single_memory_pool.h"

#include "bstone_r3r_tests.h"

#include "bstone_gl_r3r_context.h"
#include "bstone_gl_r3r_vertex_input.h"
#include "bstone_gl_r3r_vertex_input_mgr.h"

namespace bstone {

class GlR3rVertexInputMgrImpl final : public GlR3rVertexInputMgr
{
public:
	GlR3rVertexInputMgrImpl(GlR3rContext& context);
	~GlR3rVertexInputMgrImpl() override;

	static void* operator new(std::size_t size);
	static void operator delete(void* ptr);

	GlR3rContext& get_context() const noexcept override;
	R3rVertexInputUPtr create(const R3rCreateVertexInputParam& param) override;
	void set(R3rVertexInput& vertex_input) override;
	void bind_default_vao() override;

private:
	GlR3rContext& context_;
	GlR3rVertexInputUPtr default_vertex_input_{};

private:
	void initialize_default_vertex_input();
};

// ==========================================================================

using GlR3rVertexInputMgrImplPool = SingleMemoryPool<GlR3rVertexInputMgrImpl>;
GlR3rVertexInputMgrImplPool gl_r3r_vertex_input_mgr_impl_pool{};

// ==========================================================================

GlR3rVertexInputMgrImpl::GlR3rVertexInputMgrImpl(GlR3rContext& context)
BSTONE_BEGIN_CTOR_TRY
	:
	context_{context},
	default_vertex_input_{}
{
	initialize_default_vertex_input();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

GlR3rVertexInputMgrImpl::~GlR3rVertexInputMgrImpl() = default;

void* GlR3rVertexInputMgrImpl::operator new(std::size_t size)
BSTONE_BEGIN_FUNC_TRY
	return gl_r3r_vertex_input_mgr_impl_pool.allocate(size);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void GlR3rVertexInputMgrImpl::operator delete(void* ptr)
BSTONE_BEGIN_FUNC_TRY
	gl_r3r_vertex_input_mgr_impl_pool.deallocate(ptr);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

GlR3rContext& GlR3rVertexInputMgrImpl::get_context() const noexcept
{
	return context_;
}

R3rVertexInputUPtr GlR3rVertexInputMgrImpl::create(const R3rCreateVertexInputParam& param)
BSTONE_BEGIN_FUNC_TRY
	return make_gl_r3r_vertex_input(*this, param);
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void GlR3rVertexInputMgrImpl::set(R3rVertexInput& vertex_input)
BSTONE_BEGIN_FUNC_TRY
	static_cast<GlR3rVertexInput&>(vertex_input).bind();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void GlR3rVertexInputMgrImpl::bind_default_vao()
BSTONE_BEGIN_FUNC_TRY
	if (default_vertex_input_ != nullptr)
	{
		default_vertex_input_->bind_vao();
	}
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

void GlR3rVertexInputMgrImpl::initialize_default_vertex_input()
BSTONE_BEGIN_FUNC_TRY
	const auto param = R3rCreateVertexInputParam{};
	default_vertex_input_ = make_gl_r3r_vertex_input(*this, param);;
	bind_default_vao();
BSTONE_END_FUNC_CATCH_ALL_THROW_NESTED

// ==========================================================================

GlR3rVertexInputMgrUPtr make_gl_r3r_vertex_input_mgr(GlR3rContext& context)
{
	return std::make_unique<GlR3rVertexInputMgrImpl>(context);
}

} // bstone
