#version 330

// Input
// TODO(student): Get vertex attributes from each location

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_tex_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

uniform float Time;

// Output
// TODO(student): Output values to fragment shader
out vec3 color;

void main()
{
    // TODO(student): Send output to fragment shader
    color = v_color * sin(Time);

    // TODO(student): Compute gl_Position
    vec4 vector = (Model * vec4(v_position, 1));
    vector.y -= sin(Time);

    gl_Position = Projection * View * vector;
}