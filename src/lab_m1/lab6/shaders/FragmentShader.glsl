#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_normal;
in vec3 frag_coord;
in vec3 frag_color;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_normal;
layout(location = 2) out vec4 out_coord;


void main()
{
    // TODO(student): Write pixel out color
    out_color = vec4(frag_color, 1);
    out_normal = vec4(frag_normal, 1);
    out_coord = vec4(frag_coord, 1);
}
