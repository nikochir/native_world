#include "nw_gfx_pch.hpp"
#include "nw_gfx_buf_shd.h"
#if (defined NW_GAPI)
#	include "../../core/nw_gfx_engine.h"
#	include "../../lib/nw_gfx_lib_buf.h"
#	if (NW_GAPI & NW_GAPI_OGL)
namespace NW
{
	gfx_buf_shd::gfx_buf_shd() : gfx_buf(), m_slot(NW_NULL) { }
	gfx_buf_shd::gfx_buf_shd(layt_tc& layout, size_t count) : gfx_buf_shd() { NW_CHECK(remake(layout, count), "remake error!", return); }
	gfx_buf_shd::gfx_buf_shd(layt_tc& layout, size_t count, ptr_tc data) : gfx_buf_shd() { NW_CHECK(remake(layout, count, data), "remake error!", return); }
	gfx_buf_shd::gfx_buf_shd(gsbuf_tc& copy) : gfx_buf_shd() { operator=(copy); }
	gfx_buf_shd::gfx_buf_shd(gsbuf_t&& copy) : gfx_buf_shd() { operator=(copy); }
	gfx_buf_shd::~gfx_buf_shd() { }
	// --setters
	gfx_buf_shd::gsbuf_t& gfx_buf_shd::set_slot(cv1u slot) { m_slot = slot; return *this; }
	gfx_buf_shd::buf_t& gfx_buf_shd::set_data(size_t key, ptr_tc data, size_t count) {
		gfx_buf::set_data(key, data, count);
		glBindBuffer(GL_UNIFORM_BUFFER, get_handle());
		glBufferSubData(GL_UNIFORM_BUFFER, get_stride() * key, get_stride() * count, get_data(get_stride() * key));
		return *this;
	}
	// --operators
	op_stream_t& gfx_buf_shd::operator<<(op_stream_t& stm) const { gfx_buf::operator<<(stm); return stm; }
	ip_stream_t& gfx_buf_shd::operator>>(ip_stream_t& stm) { gfx_buf::operator>>(stm); return stm; }
	// --==<core_methods>==--
	v1bit gfx_buf_shd::remake()
	{
		NW_CHECK(gfx_buf::remake(), "remake error!", return NW_FALSE);
		
		glBindBuffer(GL_UNIFORM_BUFFER, get_handle());
		//glBufferData(GL_UNIFORM_BUFFER, get_space(), get_data(), has_data() ? GL_STATIC_DRAW : GL_DYNAMIC_DRAW);
		glBufferData(GL_UNIFORM_BUFFER, get_space(), get_data(), GL_DYNAMIC_DRAW);
		
		return NW_TRUTH;
	}
	v1nil gfx_buf_shd::on_draw()
	{
		gfx_buf::on_draw();
		glBindBuffer(GL_UNIFORM_BUFFER, get_handle());
		glBindBufferBase(GL_UNIFORM_BUFFER, get_slot(), get_handle());
	}
	// --==</core_methods>==--
}
#	endif	// GAPI_OGL
#	if (NW_GAPI & NW_GAPI_D3D)
namespace NW
{
	gfx_buf_shd::gfx_buf_shd(gfx_engine& graphics) :
		t_cmp(), gfx_buf(graphics),
		m_slot(NW_NULL)
	{
	}
	gfx_buf_shd::gfx_buf_shd(gfx_engine& graphics, size nof_bytes, cptr buffer, size offset) :
		gfx_buf_shd(graphics)
	{
		if (!remake_bytes(nof_bytes, buffer, offset)) { throw init_error(__FILE__, __LINE__); }
	}
	gfx_buf_shd::~gfx_buf_shd()
	{
	}
	// --setters
	void gfx_buf_shd::set_slot(v1u bind_slot) {
		m_slot = bind_slot;
	}
	void gfx_buf_shd::set_offset(size offset) {
		m_offset = offset;
	}
	void gfx_buf_shd::set_data_bytes(size nof_bytes, cptr buffer, size offset) {
		D3D11_MAPPED_SUBRESOURCE msub_res{ 0 };
		m_gfx->get_ctxh()->Map(m_handle, 0u, D3D11_MAP_WRITE_NO_OVERWRITE, 0u, &msub_res);
		memcpy(static_cast<ubyte*>(msub_res.pData) + m_offset + offset, buffer, nof_bytes);
		m_gfx->get_ctxh()->Unmap(m_handle, 0u);
	}
	// --==<core_methods>==--
	v1bit gfx_buf_shd::remake_bytes(size nof_bytes, cptr buffer, size offset)
	{
		m_size = nof_bytes;
		m_offset = offset;
		m_stride = nof_bytes;
		if (m_handle != NW_NULL) { m_handle->Release(); m_handle = NW_NULL; }
		if (m_size <= 0) { return NW_FALSE; }
		if (m_offset >= m_size || m_elems.size() == 0) { return NW_FALSE; }

		D3D11_BUFFER_DESC buf_desc{ 0 };
		buf_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buf_desc.MiscFlags = 0u;
		buf_desc.StructureByteStride = m_size;
		buf_desc.ByteWidth = m_size;

		if (buffer == NW_NULL) {
			buf_desc.Usage = D3D11_USAGE_DYNAMIC;
			buf_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			m_gfx->get_dvch()->CreateBuffer(&buf_desc, NW_NULL, &m_handle);
		}
		else {
			buf_desc.Usage = D3D11_USAGE_DEFAULT;
			buf_desc.CPUAccessFlags = 0u;
			D3D11_SUBRESOURCE_DATA sub_data{ 0 };
			sub_data.pSysMem = buffer;
			m_gfx->get_dvch()->CreateBuffer(&buf_desc, &sub_data, &m_handle);
		}

		if (m_handle == NW_NULL) { throw init_error(__FILE__, __LINE__); return NW_FALSE; }
		return NW_TRUTH;
	}
	void gfx_buf_shd::on_draw()
	{
		m_gfx->get_ctxh()->VSSetConstantBuffers(m_slot, 1u, &m_handle);
		m_gfx->get_ctxh()->PSSetConstantBuffers(m_slot, 1u, &m_handle);
		m_gfx->get_ctxh()->GSSetConstantBuffers(m_slot, 1u, &m_handle);
		m_gfx->get_ctxh()->HSSetConstantBuffers(m_slot, 1u, &m_handle);
		m_gfx->get_ctxh()->DSSetConstantBuffers(m_slot, 1u, &m_handle);
	}
	// --==</core_methods>==--
}
#	endif	// GAPI_D3D
#endif	// NW_GAPI