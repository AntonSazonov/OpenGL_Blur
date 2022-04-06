#pragma once

// Supported contects are: 3.3 core or 4.5 core
#ifndef SAN_GL_33
 #define SAN_GL_45
#endif

#include <glad/gl.h>

#define GLFW_INCLUDE_NONE
#include <glfw/glfw3.h>		// include after glad

#include <glm/glm.hpp>

#include <cstring>

namespace san::gl {

#define RETURN_IF( expr )	if ( (expr) ) { fprintf( stderr, "Error: %s\n", #expr ); return; }

class window {
	bool			m_is_valid	= false;
	GLFWwindow *	m_window	= nullptr;

public:
	window( const glm::ivec2 & size, const char * title = "" ) {

		glfwSetErrorCallback( []( int err, const char * desc ) {
				fprintf( stderr, "GLFW error %3d: %s\n", err, desc );
			} );

		RETURN_IF( glfwInit() != GLFW_TRUE );

		glfwWindowHint( GLFW_VISIBLE, GLFW_FALSE );
		glfwWindowHint( GLFW_RESIZABLE, GLFW_FALSE );

		glfwWindowHint( GLFW_CLIENT_API, GLFW_OPENGL_API );
		glfwWindowHint( GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API );

		glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
		glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE/*GLFW_TRUE*/ );
		//glfwWindowHint( GLFW_CONTEXT_NO_ERROR, GLFW_TRUE );

#ifdef SAN_GL_DEBUG
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE );
#else
		glfwWindowHint( GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE );
#endif

#ifdef SAN_GL_33
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
#else
		glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 4 );
		glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 5 );
#endif

		glfwWindowHint( GLFW_DOUBLEBUFFER, GLFW_TRUE );

		glfwWindowHint( GLFW_SAMPLES, 0 );
		glfwWindowHint( GLFW_DEPTH_BITS, 0 );
		glfwWindowHint( GLFW_STENCIL_BITS, 8 );

		RETURN_IF( !(m_window = glfwCreateWindow( size.x, size.y, title, nullptr, nullptr )) );
		glfwSetWindowUserPointer( m_window, this );

//		int w, h;
//		glfwGetFramebufferSize( m_window, &w, &h );
//		printf( "w = %d, h = %d\n", w, h );

		glfwSetWindowSizeCallback( m_window, []( GLFWwindow * p_wnd, int width, int height ) {
				auto p_this = static_cast<decltype( this )>( glfwGetWindowUserPointer( p_wnd ) );
				if ( p_this ) p_this->on_resize( glm::ivec2( width, height ) );
			} );

		glfwSetKeyCallback( m_window, []( GLFWwindow * p_wnd, int key, int scancode, int action, int mods ) {
				auto p_this = static_cast<decltype( this )>( glfwGetWindowUserPointer( p_wnd ) );
				if ( p_this ) p_this->on_key( key, scancode, action, mods );
			} );


		/// center window
		{
			GLFWmonitor * p_monitor = glfwGetPrimaryMonitor();
			const GLFWvidmode * p_mode = glfwGetVideoMode( p_monitor );
			int w, h;
			glfwGetWindowSize( m_window, &w, &h );
			glfwSetWindowPos( m_window,
				p_mode->width  / 2 - w / 2,
				p_mode->height / 2 - h / 2 );
		}

		glfwMakeContextCurrent( m_window );

		int version = gladLoadGL( glfwGetProcAddress );
		//int version = gladLoaderLoadGL();
		if ( !version ) {
			printf( "Failed to initialize OpenGL context\n" );
			return;
		}
		printf( "Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR( version ), GLAD_VERSION_MINOR( version ) );

		if ( GLAD_GL_KHR_parallel_shader_compile ) {
			printf( "GL_KHR_parallel_shader_compile supported.\n" );
			glMaxShaderCompilerThreadsKHR( 2 ); // 0xFFFFFFFF - implementation-specific maximum
		}

#ifdef SAN_GL_DEBUG
		if ( GLAD_GL_KHR_debug ) {
			printf( "GL_KHR_debug supported.\n" );

			// 3.3 core does not have glDebugMessageControl, but we have GL_KHR_debug extension
			/// *** DEBUG ***
			// GL_DEBUG_OUTPUT - Faster version but not useful for breakpoints
			// GL_DEBUG_OUTPUT_SYNCHRONUS - Callback is in sync with errors, so a breakpoint
			//    can be placed on the callback in order to get a stacktrace for the GL error.
			//glEnable( GL_DEBUG_OUTPUT_SYNCHRONOUS );
			glDebugMessageControl( GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE );
			glDebugMessageCallback( []( GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar * p_message, const void * p_user /*user param.*/ ) {
				const char * p_source;
				switch ( source ) {
					case GL_DEBUG_SOURCE_API			: p_source = "API";				break;
					case GL_DEBUG_SOURCE_WINDOW_SYSTEM	: p_source = "Window System";	break;
					case GL_DEBUG_SOURCE_SHADER_COMPILER: p_source = "Shader Compiler";	break;
					case GL_DEBUG_SOURCE_THIRD_PARTY	: p_source = "Third Party";		break;
					case GL_DEBUG_SOURCE_APPLICATION	: p_source = "Application";		break;
					case GL_DEBUG_SOURCE_OTHER			: p_source = "Other";			break;
					default								: p_source = "Unknown";			break;
				}

				static int errs = 0;
				const char * p_type;
				switch ( type ) {
					case GL_DEBUG_TYPE_ERROR				: p_type = "Error"; ++errs;	break;
					case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR	: p_type = "Deprecated";	break;
					case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR	: p_type = "Undefined";		break;
					case GL_DEBUG_TYPE_PORTABILITY			: p_type = "Portability";	break;
					case GL_DEBUG_TYPE_PERFORMANCE			: p_type = "Performance";	break;
					case GL_DEBUG_TYPE_OTHER				: p_type = "Other";			break;
					case GL_DEBUG_TYPE_MARKER				: p_type = "Marker";		break;
					case GL_DEBUG_TYPE_PUSH_GROUP			: p_type = "Push Group";	break;
					case GL_DEBUG_TYPE_POP_GROUP			: p_type = "Pop Group";		break;
					default									: p_type = "Unknown";		break;
				}

				const char * p_severity;
				switch ( severity ) {
					case GL_DEBUG_SEVERITY_HIGH			: p_severity = "High";			break;
					case GL_DEBUG_SEVERITY_MEDIUM		: p_severity = "Medium";		break;
					case GL_DEBUG_SEVERITY_LOW			: p_severity = "Low";			break;
					case GL_DEBUG_SEVERITY_NOTIFICATION	: p_severity = "Notification";	break;
					default								: p_severity = "Unknown";		break;
				}

				//fprintf( stderr, "GL Debug: %15s, %11s, %12s, \"%s\"\n", p_source, p_type, p_severity, p_message );
				fprintf( stderr, "[%s, %s, %s]: %s\n", p_source, p_type, p_severity, p_message );

				if ( errs > 3 ) {
					auto p_this = static_cast<decltype( this )>( const_cast<void *>( p_user ) );
					if ( p_this ) p_this->close();
				}

			}, this );
		}
#endif

		glDisable( GL_MULTISAMPLE );
		glDisable( GL_CULL_FACE );
		glDisable( GL_DEPTH_TEST );
		//glDepthFunc( GL_LEQUAL );

#if 0
		int n_exts = 0;
		glGetIntegerv( GL_NUM_EXTENSIONS, &n_exts );
		printf( "n_exts: %d\n", n_exts );
		printf( "p_exts:\n" );
		for ( int i = 0; i < n_exts; i++ ) {
			printf( " %s\n", glGetStringi( GL_EXTENSIONS, i ) );
		}
		return;
#endif

#if 1
		if ( glfwExtensionSupported( "WGL_EXT_swap_control_tear" ) == GLFW_TRUE ||
			 glfwExtensionSupported( "GLX_EXT_swap_control_tear" ) == GLFW_TRUE )
		{
			printf( "Extension EXT_swap_control_tear is supported.\n" );
			glfwSwapInterval( -1 );
		} else {
			glfwSwapInterval( 1 );
		}
#endif

		m_is_valid = true;
	}

	virtual ~window() {
    	if ( m_window ) glfwDestroyWindow( m_window );
		glfwTerminate();
		m_is_valid = false;
	}

	explicit operator bool () const { return m_is_valid; }

	virtual void on_frame( double /*time*/, const glm::ivec2 & /*framebuffer*/, const glm::ivec2 & /*mouse*/ ) = 0;

	GLFWwindow * get_window() { return m_window; }

	void close() {
		if ( m_window ) glfwSetWindowShouldClose( m_window, GLFW_TRUE );
	}

	void run( bool wait = false ) {
		if ( operator bool () ) {
			if ( !glfwWindowShouldClose( m_window ) ) { // check for early exit
				glfwShowWindow( m_window ); // calls resize_callback
				while ( !glfwWindowShouldClose( m_window ) ) {

					int win_width, win_height;
					glfwGetWindowSize( m_window, &win_width, &win_height );

					int fb_width, fb_height;
					glfwGetFramebufferSize( m_window, &fb_width, &fb_height );

					double mx, my;
					glfwGetCursorPos( m_window, &mx, &my );

					this->on_frame( glfwGetTime(), glm::ivec2( fb_width, fb_height ), glm::ivec2( win_width, win_height ), glm::ivec2( mx, my ) );
					glfwSwapBuffers( m_window );
					if ( wait ) {
						glfwWaitEvents();
					} else {
						glfwPollEvents();
					}
				}
			}
		}
	}
}; // class window

} // namespace san::gl
