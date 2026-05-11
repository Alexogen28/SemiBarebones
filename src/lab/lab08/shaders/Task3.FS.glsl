#version 330

// Input
in vec2 texture_coord;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
// TODO(student): Declare various other uniforms
uniform sampler2D texture_unit_1;
uniform sampler2D texture_unit_2;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.
    vec4 color_1 = texture2D(texture_unit_1, texture_coord);
    vec4 color_2 = texture2D(texture_unit_2, texture_coord);
    vec4 color = mix(color_1, color_2, 0.5);


 
    if(color.a < 0.5) {
        discard;
    }
    // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel

    out_color = color;
}
