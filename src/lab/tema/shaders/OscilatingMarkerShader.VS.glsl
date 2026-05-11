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

uniform float Time;

uniform vec3 HeliPosition;




void main()
{
    vec4 new_position = vec4(v_position, 1);
    new_position.y += sin(Time) * 0.5;
    
    vec4 w_position = Model * new_position;

    vec4 height = texture2D(texture_unit_1, w_position.xz / 256);
    w_position.y += height.x * 10 + 0.1f;

    float vec_length = length(HeliPosition.xz - w_position.xz);
    w_position.y -= vec_length * vec_length * 0.03;


    gl_Position = Projection * View * w_position;
}
