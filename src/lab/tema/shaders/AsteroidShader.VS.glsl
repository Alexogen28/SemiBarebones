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
uniform sampler2D texture_unit_3;
uniform sampler2D texture_unit_4;

// Output
out vec2 texture_coord;
out vec3 text_positions;


void main()
{
    vec3 position = v_position;
    texture_coord = v_texture_coord;

    vec4 color = texture2D(texture_unit_2, texture_coord);
    position.y = color.x * 10;
    text_positions.y = position.y;

    float vec_length = length(WorldPositionAndScale.xz - position.xz);
    position.y = position.y - vec_length * vec_length * 0.03;

    gl_Position = Projection * View * Model * vec4(position, 1.0);
}
