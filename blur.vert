#version 330 core

void main() {
	vec2 unit = ivec2( gl_VertexID & 1, gl_VertexID >> 1 ); // map [0-4] to [0;0]-[1,1]
	gl_Position = vec4( unit * 2 - 1, 0, 1 ); // map [0;1] to [-1;-1 - 1;1]
}
