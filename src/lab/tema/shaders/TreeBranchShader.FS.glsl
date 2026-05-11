#version 330

in vec2 texture_coord;
in float selection_color;

uniform sampler2D texture_unit_1;
uniform sampler2D texture_unit_2;

uniform sampler2D texture_unit_3;

uniform int Level;

// Output
layout(location = 0) out vec4 out_color;
layout(location = 1) out vec4 out_selection;

void main()
{
    vec4 color_1 = texture2D(texture_unit_1, texture_coord);
    vec4 color_2 = texture2D(texture_unit_2, texture_coord);
    vec4 color_3 = texture2D(texture_unit_3, texture_coord);

    vec4 color = vec4(1);

    if(Level < 4)
    {
        color = color_2;
    }else if(Level > 4)
    {
        color = color_3;
    }else color = mix(color_2, color_3, 0.5);

    out_color = color;
    out_selection = vec4(0,0,0,selection_color);
}
