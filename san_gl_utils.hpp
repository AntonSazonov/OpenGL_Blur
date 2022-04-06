#pragma once

#include "stb_image.h"
#include "stb_image_resize.h"

namespace san::gl {

bool load_texture( GLuint tex_id, const glm::ivec2 & size, const char * filename, int flip_verticaly = 0 ) {
	int w, h, n;
	stbi_set_flip_vertically_on_load( flip_verticaly ); // flip verticaly for OpenGL coordinate system
	uint8_t * p_src = stbi_load( filename, &w, &h, &n, 0 );
	//printf( "load_texture(): n = %d\n", n );
	if ( p_src ) {
		/// resize...
		uint8_t * p_dst = new (std::nothrow) uint8_t [size.x * size.y * n];
		if ( p_dst ) {
			if ( stbir_resize_uint8( p_src, w, h, 0, p_dst, size.x, size.y, 0, n ) ) {
				glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
				//glPixelStorei( GL_UNPACK_ROW_LENGTH, -size.x );
#ifdef SAN_GL_33
				glBindTexture( GL_TEXTURE_2D, tex_id );
				//glActiveTexture( 0 );
				glTexSubImage2D( GL_TEXTURE_2D, 0/*level*/, 0/*x_off*/, 0/*y_off*/, size.x, size.y, GL_RGB, GL_UNSIGNED_BYTE, p_dst ); // since 2.0
#else
				glTextureSubImage2D( tex_id, 0/*level*/, 0/*x_off*/, 0/*y_off*/, size.x, size.y, GL_RGB, GL_UNSIGNED_BYTE, p_dst ); // since 4.5
#endif
			}
			delete [] p_dst;
		}
		stbi_image_free( p_src );
		return true;
	}
	return false;
}

// s:  1.25  2.5   5   10  12.5
// r:    5    10   20  40   50
// s = r / 4
template <unsigned MaxRadius = 64>
class gaussian_blur_kernel {
public:
	static constexpr unsigned MaxSize = MaxRadius * 2 + 1;

private:
	float	m_kernel[MaxSize];
	int		m_radius;
	int		m_size;
	float	m_sigma;
	float	m_sigma_coeff;
	float	m_accum;		// Sum of all kernel's values. Used for normalization.

	void calc() {
		m_accum = 0;
		for ( int i = -m_radius; i <= m_radius; ++i ) {
			float value = exp( -(i * i) / (m_sigma * m_sigma * 2) );
			m_kernel[i + m_radius] = value;
			m_accum += value;
		}
	}

public:
	gaussian_blur_kernel( int radius = 10, float sigma_coeff = 2.5f ) {
		set_radius( radius );
		set_sigma_coeff( sigma_coeff );
	}

	void set_radius( int radius ) {
		m_radius = radius;
		if ( m_radius > MaxRadius ) m_radius = MaxRadius; // clamp to MaxRadius
		m_size = m_radius * 2 + 1;
		m_sigma = m_radius / m_sigma_coeff;
		calc();
	}

	void set_sigma_coeff( float coeff ) {
		m_sigma_coeff = coeff;
		calc();
	}

	void normalize() {
		for ( int i = 0; i < m_size; ++i ) {
			m_kernel[i] /= m_accum;
		}
	}

	int				get_radius() const	{ return m_radius; }
	unsigned		get_size() const	{ return m_size; }
	const float *	get_values() const	{ return m_kernel; }
}; // class gaussian_blur_kernel

} // namespace san::gl
