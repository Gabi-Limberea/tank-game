#version 330

// Input
in vec3 world_pos;
in vec3 world_normal;

// Uniforms for light properties
uniform float health_in;

uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform float material_ka;
uniform float material_ke;
uniform int material_shininess;

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec3 color;
    vec3 V, L, H, R;

    V = normalize(eye_position - world_pos);
    L = normalize(light_position - world_pos);
    H = normalize(L + V);
    R = reflect(-L, world_normal);

    vec3 ambient_light = material_ka * object_color;
    vec3 emissive_light = material_ke * object_color;

    vec3 diffuse_light = vec3(0);
    diffuse_light = object_color  * material_kd * max(dot(world_normal, L), 0.0);

    vec3 specular_light = vec3(0);
    if (diffuse_light.x > 0 || diffuse_light.y > 0 || diffuse_light.z > 0)
    {
        specular_light = object_color * material_ks * pow(max(dot(V, R), 0.0), material_shininess);
    }

    float dist = distance(light_position, world_pos);
    float attenuation = 1.0 / pow(dist, 2);

    color = (ambient_light + emissive_light + diffuse_light + specular_light);

    if (health_in != -1.0) {
        color = color * vec3(1, 1, health_in / 100);
    }

    out_color = vec4(color, 1.0);
}
