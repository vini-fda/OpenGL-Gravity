#version 330 core
layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_color;
layout (location = 2) in vec2 vertex_tex_coord;
uniform float time;
//uniform mat4 transform;
out vec3 color;
out vec2 tex_coord;
out vec3 pos;
void main()
{
   pos = vertex_position;
   tex_coord = vertex_tex_coord;
   color = vertex_color;
   gl_Position = vec4(vertex_position, 1.0);
}
