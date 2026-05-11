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
    color = v_color + sin(Time);

    // TODO(student): Compute gl_Position

    float totally_random = v_position.x * cos(Time) + v_position.y * 1.12 * sin(Time) + v_position.z * 9.12 * sin(Time);
    float random_x = v_position.x * 784782.12;
    float random_y = v_position.y * 125.12;
    float random_z = v_position.z * 7534.8856;

    //gl_Position = Projection * View * Model * vec4(v_position.x * cos(Time), v_position.y*sin(Time), v_position.z*cos(Time), 1);
    gl_Position = Projection * View * Model * vec4(v_position * cos(Time), 1);
}