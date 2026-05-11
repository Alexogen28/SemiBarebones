#version 330

// Input
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture_coord;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec4 WorldPositionAndScale;

uniform sampler2D texture_unit_1;
uniform sampler2D texture_unit_2;

// Output
out vec2 texture_coord;


void main()
{
    vec4 position = vec4(v_position, 1.0);
    vec4 w_position = Model * position;

    vec4 height = texture2D(texture_unit_1, WorldPositionAndScale.xz / 255);
    w_position.y += height.x * 10 + 2;

    gl_Position = Projection * View * w_position;
}
