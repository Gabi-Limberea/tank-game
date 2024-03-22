#version 330

// Input
// TODO(student): Get color value from vertex shader
in vec3 color;
in float health;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec3 new_color = color;

    if (health != -1.0) {
//        new_color = ((health - 100.0) * vec3(1)) * new_color;
        new_color = color * vec3(1, 1, health / 100);
    }

    out_color = vec4(new_color, 1.0);
}
