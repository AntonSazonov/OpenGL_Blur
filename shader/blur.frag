#version 330 core

#define MAX_RADIUS	64

//layout (binding = 0)
uniform sampler2D	u_tex;

uniform	int			u_radius;
uniform float		u_kernel[MAX_RADIUS];

uniform	ivec2		u_viewport;
uniform	vec2		u_direction;

in vec4				gl_FragCoord;

layout (location = 0) out vec4 o_color;

void main() {
	vec2 uv = gl_FragCoord.xy / u_viewport;
	vec2 dir = u_direction / u_viewport;
	vec2 l = uv - dir;
	vec2 r = uv + dir;
	vec3 res = u_kernel[0] * texture( u_tex, uv ).rgb;
	for ( int i = 1; i < u_radius; ++i ) {
		res += u_kernel[i] * texture( u_tex, l ).rgb;
		res += u_kernel[i] * texture( u_tex, r ).rgb;
		l -= dir;
		r += dir;
	}
	o_color = vec4( res, 1 );
}
