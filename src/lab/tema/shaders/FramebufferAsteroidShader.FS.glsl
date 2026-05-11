#version 330

// Input
in vec2 texture_coord;
in vec3 text_positions;
in vec4 vertex_position;

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
layout(location = 1) out vec4 out_coordinate;

void main()
{
    vec4 color_1 = texture2D(texture_unit_1, texture_coord);
    vec4 color_2 = texture2D(texture_unit_2, texture_coord);
    vec4 color_3 = texture2D(texture_unit_3, texture_coord);
    vec4 color_4 = texture2D(texture_unit_4, texture_coord);
 
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
       
    out_color = color_out;
    out_coordinate = vec4(vertex_position.x, vertex_position.y, vertex_position.z, 0);
}
