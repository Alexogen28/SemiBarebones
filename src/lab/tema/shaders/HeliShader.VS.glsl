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

// Output
//out vec2 texture_coord;


void main()
{
    //vec2 height_texture_coords = vec2(WorldPositionAndScale.x / 30, WorldPositionAndScale.z / 30);
    //vec4 height = texture2D(texture_unit_2, height_texture_coords);

    //vec3 new_position = v_position;
    //new_position.y += height.x * WorldPositionAndScale.w;

    //texture_coord = v_texture_coord;

    vec4 position = vec4(v_position, 1.0);
    vec4 w_position = Model * position;

    vec4 height = texture2D(texture_unit_1, WorldPositionAndScale.xz / 255);
    w_position.y += height.x * 10 + 2;

    gl_Position = Projection * View * w_position;
}
