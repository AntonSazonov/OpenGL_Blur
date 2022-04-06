#pragma once

namespace san::gl {

class vao {
	GLuint	m_vao;

public:
	vao() {
		glGenVertexArrays( 1, &m_vao );
		glBindVertexArray( m_vao );
		glObjectLabel( GL_VERTEX_ARRAY, m_vao, 5, "m_vao" );
	}

	virtual ~vao() { glDeleteVertexArrays( 1, &m_vao ); }
	void bind() { glBindVertexArray( m_vao ); }
}; // class vao

// Dummy VAO object for rendering quads on screen/framebuffer.
struct vao_quad : vao {
	void draw() {
		bind();
		glDrawArrays( GL_TRIANGLE_STRIP, 0 /*first*/, 4 /*count*/ );
	}
};

} // namespace san::gl
