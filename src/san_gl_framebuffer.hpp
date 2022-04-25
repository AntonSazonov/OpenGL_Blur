#pragma once

namespace san::gl {

class framebuffer {
	bool		m_is_valid	= false;
	GLuint		m_tex		= 0;
	GLuint		m_rbo		= 0;
	GLuint		m_fbo		= 0;
	glm::ivec2	m_size;

public:
	framebuffer( const glm::ivec2 & size ) : m_size( size ) {

#ifdef SAN_GL_33
		/// 3.3+
		glGenTextures( 1, &m_tex );
		glBindTexture( GL_TEXTURE_2D, m_tex );
		glObjectLabel( GL_TEXTURE, m_tex, 5, "m_tex" );

		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0/*border*/, GL_RGB, GL_UNSIGNED_BYTE, nullptr ); // since 2.0
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glGenRenderbuffers( 1, &m_rbo );
		glBindRenderbuffer( GL_RENDERBUFFER, m_rbo );
		glObjectLabel( GL_RENDERBUFFER, m_rbo, 5, "m_rbo" );

		glRenderbufferStorage( GL_RENDERBUFFER, GL_STENCIL_INDEX8, m_size.x, m_size.y );

		glGenFramebuffers( 1, &m_fbo );
		glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
		glObjectLabel( GL_FRAMEBUFFER, m_fbo, 5, "m_fbo" );

		glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0 );
		glFramebufferRenderbuffer( GL_FRAMEBUFFER, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo );

		GLenum ret = glCheckFramebufferStatus( GL_FRAMEBUFFER ); // GL_FRAMEBUFFER is equivalent to GL_DRAW_FRAMEBUFFER
#else
		/// 4.5+
		glCreateTextures( GL_TEXTURE_2D, 1, &m_tex );
		glTextureStorage2D( m_tex, 1, GL_RGBA8, m_size.x, m_size.y );
		glTextureParameteri( m_tex, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
		glTextureParameteri( m_tex, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
		glTextureParameteri( m_tex, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
		glTextureParameteri( m_tex, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

		glCreateRenderbuffers( 1, &m_rbo );
		glBindRenderbuffer( GL_RENDERBUFFER, m_rbo );
		glRenderbufferStorage( GL_RENDERBUFFER, GL_STENCIL_INDEX8, m_size.x, m_size.y );

		glCreateFramebuffers( 1, &m_fbo );
		glNamedFramebufferTexture( m_fbo, GL_COLOR_ATTACHMENT0, m_tex, 0/*level*/ );
		glNamedFramebufferRenderbuffer( m_fbo, GL_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo );

		GLenum ret = glCheckNamedFramebufferStatus( m_fbo, GL_DRAW_FRAMEBUFFER );
#endif
		m_is_valid = ret == GL_FRAMEBUFFER_COMPLETE;
	}

	virtual ~framebuffer() {
		if ( m_fbo ) glDeleteFramebuffers( 1, &m_fbo );
		if ( m_rbo ) glDeleteRenderbuffers( 1, & m_rbo );
		if ( m_tex ) glDeleteTextures( 1, &m_tex );
	}

	explicit operator bool () const { return m_is_valid; }

	auto get_size() -> glm::ivec2 { return m_size; }
	auto get_tex() -> GLuint { return m_tex; }
	auto get_rbo() -> GLuint { return m_rbo; }
	auto get_fbo() -> GLuint { return m_fbo; }

	void update( uint8_t * p_data ) {
		glBindTexture( GL_TEXTURE_2D, m_tex );
		glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, m_size.x, m_size.y, 0/*border*/, GL_RGB, GL_UNSIGNED_BYTE, p_data ); // since 2.0
	}

	void bind_as_fbo() {
		glBindFramebuffer( GL_FRAMEBUFFER, m_fbo );
	}

	void bind_as_tex( int tex_unit ) {
		glActiveTexture( GL_TEXTURE0 + tex_unit ); // Texture unit
		glBindTexture( GL_TEXTURE_2D, m_tex );
	}
}; // class framebuffer

} // namespace san::gl
