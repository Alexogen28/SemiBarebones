#version 330

// Input
in vec2 texture_coord;
in vec3 text_positions;

// Uniform properties
uniform sampler2D texture_1;
uniform sampler2D texture_2;
uniform sampler2D texture_3;
uniform sampler2D texture_4;
// TODO(student): Declare various other uniforms
uniform sampler2D texture_unit_1;
uniform sampler2D texture_unit_2;
uniform sampler2D texture_unit_3;
uniform sampler2D texture_unit_4;

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.
    
    vec4 color_1 = texture2D(texture_unit_1, texture_coord);
    vec4 color_2 = texture2D(texture_unit_2, texture_coord);
    vec4 color_3 = texture2D(texture_unit_3, texture_coord);
    vec4 color_4 = texture2D(texture_unit_4, texture_coord);
 
    //vec4 color_out = mix(color_1, color_2.rrrr, 0.5);
    vec4 color_out;


    if(text_positions.y < 1.3 ){
        color_out = color_3;
    } 
    else if (text_positions.y > 1.3 &&  text_positions.y < 2) {
        color_out =  mix(color_3, color_1, (text_positions.y - 1.3)*1.5 );
    }
    else if (text_positions.y > 1.85 &&  text_positions.y < 2) {
        color_out =  mix(color_1, color_3, 0.35);
    }
    else if (text_positions.y > 2 && text_positions.y < 5.7) {
          color_out = color_1;
    } 
    else if (text_positions.y > 5.7 && text_positions.y < 5.85){
         color_out =  mix(color_1, color_4, 0.35);
    }
    else if (text_positions.y > 5.85 && text_positions.y < 6){
         color_out =  mix(color_1, color_4, 0.70);
    }
    else {
         color_out = color_4;
    }
       

     // TODO(student): Use the "discard" directive to terminate execution
    // based on the value of the alpha channel
    if(color_1.a < 0.5) {
        discard;
    }
  

    out_color = color_out;
}
