#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

// Uniforms for light properties
uniform vec3 light_position;
uniform vec3 eye_position;
uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;

// Output value to fragment shader
out vec3 color;


void main()
{
    // TODO(student): Compute world space vectors
    vec3 world_pos;
    vec3 N, V, L, H, R;
    
    world_pos = (Model * vec4(v_position, 1.0)).xyz;
    N = normalize(mat3(Model) * v_normal);
    V = normalize(eye_position - world_pos);
    L = normalize(light_position - world_pos);
    H = normalize(L + V);
    R = reflect(-L , N);

    // TODO(student): Define ambient light component
    vec3 ambient_light = vec3(0.45) * object_color;
    

    // TODO(student): Compute diffuse light component
    vec3 diffuse_light = vec3(0);
    diffuse_light = object_color * vec3(0, 0, 1) * material_kd * max(dot(N, L), 0.0);


    // TODO(student): Compute specular light component
    vec3 specular_light = vec3(0);

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Gouraud (1971) shading method. There is also the Phong (1975) shading
    // method, which we'll use in the future. Don't mix them up!
    if (diffuse_light.x > 0 || diffuse_light.y > 0 || diffuse_light.z > 0)
    {
        // specular_light = material_ks * pow(max(dot(V, R), 0.0), material_shininess);
        specular_light =  object_color * vec3(0, 0, 1) * material_ks * pow(max(dot(N, H), 0.0), material_shininess);
    }

    // TODO(student): Compute light
    float dist = distance(light_position, world_pos);
    float attenuation = 1.0 / pow(dist, 2);

    // TODO(student): Send color light output to fragment shader
    color =  (ambient_light + (diffuse_light + specular_light) * attenuation);

    gl_Position = Projection * View * Model * vec4(v_position, 1.0);
}
