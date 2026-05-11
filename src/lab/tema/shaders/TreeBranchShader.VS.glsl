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

uniform int Level;
uniform float Length;
uniform vec3 HeliPosition;

// Output
out vec2 texture_coord;
out float selection_color;


void main()
{
    texture_coord = v_texture_coord;
    vec3 world_position = vec3(WorldPositionAndScale.x, WorldPositionAndScale.y, WorldPositionAndScale.z);

    vec4 position = vec4(v_position, 1.0);
    vec4 w_position = Model * position;

    float tree_height = (Length * ((Level+1)*0.7)) * Length;
    texture_coord.x = (1/(2*3.14)) * atan(v_position.z/v_position.x);
    texture_coord.y = v_position.y / tree_height;

    vec4 height = texture2D(texture_unit_1, w_position.xz / 256);
    w_position.y += height.x * 10 + 0.5f;

    float vec_length = length(HeliPosition.xz - w_position.xz);
    w_position.y -= vec_length * vec_length * 0.03;

    selection_color = WorldPositionAndScale.w;


    gl_Position = Projection * View * w_position;
}
