#pragma once

#include <memory>
#include <glm/gtc/type_ptr.hpp>

namespace san::gl::shader {

class base {
	GLuint	m_id = 0;

protected:
	base( GLenum type ) : m_id( glCreateShader( type ) ) {}
	virtual ~base() { if ( m_id ) glDeleteShader( m_id ); }

public:

	bool compile_from_string( const char * p_src ) {
		GLint status = GL_FALSE;
		if ( m_id ) {
			glShaderSource( m_id, 1, &p_src, nullptr );
			glCompileShader( m_id );
			glGetShaderiv( m_id, GL_COMPILE_STATUS, &status );
			if ( status != GL_TRUE ) {
				int len;
				glGetShaderiv( m_id, GL_INFO_LOG_LENGTH, &len );
				if ( len ) {
					char * p_log = new (std::nothrow) char [len + 1];
					glGetShaderInfoLog( m_id, len, nullptr, p_log );
					fprintf( stderr, "%s\n", p_log );
					delete [] p_log;
				}
			}
		}
		return status == GL_TRUE;
	}

	bool compile_from_file( const char * file_name ) {
		bool result = false;
		if ( m_id ) {
			FILE * fp = fopen( file_name, "rb" );
			if ( fp ) {
				fseek( fp, 0, SEEK_END );
				size_t size = ftell( fp );
				fseek( fp, 0, SEEK_SET );
				char * p_data = new (std::nothrow) char [size + 1];
				if ( p_data ) {
					if ( fread( p_data, size, 1, fp ) != 1 ) {
						fprintf( stderr, "fread(): error\n" );
						return false;
					}
					p_data[size] = '\0';
					result = compile_from_string( p_data );
					delete [] p_data;
				}
				fclose( fp );
			} else {
				fprintf( stderr, "compile_from_shader(): can't compile %s\n", file_name );
			}
		}
		return result;
	}


	friend class prog;
}; // class base

struct vert : base { vert() : base( GL_VERTEX_SHADER   ) {} };
struct frag : base { frag() : base( GL_FRAGMENT_SHADER ) {} };

class vert_default : public vert {
public:
	vert_default() {
		compile_from_string( R"(
#version 450 core
void main() {
	// map [0-4] to [-1,-1 - 1,1]
	gl_Position = vec4( ivec2( gl_VertexID & 1, gl_VertexID >> 1 ) * 2 - 1, 0, 1 );
}
)" );
	}
}; // class vert_default

class prog {
	GLuint	m_id = 0;

public:
	prog() : m_id( glCreateProgram() ) {}
	virtual ~prog() { if ( m_id ) glDeleteProgram( m_id ); }

	void attach( const base & shader ) { glAttachShader( m_id, shader.m_id ); }

	bool link() {
		GLint status = GL_FALSE;
		if ( m_id ) {
			glLinkProgram( m_id );
			glGetProgramiv( m_id, GL_LINK_STATUS, &status );
			if ( status == GL_TRUE ) {
				glValidateProgram( m_id );
				glGetProgramiv( m_id, GL_VALIDATE_STATUS, &status );
			}
		}
		return status == GL_TRUE;
	}

	GLuint get_id() const { return m_id; }

	const prog & bind() const {
		if ( m_id ) glUseProgram( m_id );
		return *this;
	}

#if 1
#define UNIFORM_V glGetUniformLocation( m_id, name ), 1, glm::value_ptr( v )

	/// https://www.opengl.org/sdk/docs/man/html/glUniform.xhtml
	void uniform( const char * name, GLint x )				{ if ( m_id ) glUniform1i( glGetUniformLocation( m_id, name ), x ); }
	void uniform( const char * name, GLfloat x )			{ if ( m_id ) glUniform1f( glGetUniformLocation( m_id, name ), x ); }
	void uniform( const char * name, const glm::uvec2 & v )	{ if ( m_id ) glUniform2uiv( UNIFORM_V ); }
	void uniform( const char * name, const glm::ivec2 & v )	{ if ( m_id ) glUniform2iv( UNIFORM_V ); }
	void uniform( const char * name, const glm::vec2 & v )	{ if ( m_id ) glUniform2fv( UNIFORM_V ); }
	void uniform( const char * name, const glm::vec4 & v )	{ if ( m_id ) glUniform4fv( UNIFORM_V ); }

	void uniform( const char * name, const GLfloat * v, int size )	{ if ( m_id ) glUniform1fv( glGetUniformLocation( m_id, name ), size, v ); }

#undef UNIFORM_V
#endif
}; // class prog

} // namespace san::gl::shader
