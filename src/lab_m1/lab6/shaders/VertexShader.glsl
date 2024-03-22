#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec3 v_coord;
layout(location = 3) in vec3 color;


// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_normal;
out vec3 frag_coord;
out vec3 frag_color;

void main()
{
    // TODO(student): Send output to fragment shader
    frag_color = vec3(color.x * sin(Time), color.y * sin(Time), color.z * sin(Time));
    frag_normal = v_normal;
    frag_coord = v_coord;

    // TODO(student): Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position.x + sin(Time), v_position.y, v_position.z, 1.0);
}
