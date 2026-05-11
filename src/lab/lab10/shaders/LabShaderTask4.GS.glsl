#version 430

// Input and output topologies
layout(triangles) in;
layout(triangle_strip, max_vertices = 170) out;

// Input
layout(location = 1) in vec2 v_texture_coord[];
layout(location = 0) in vec3 normal[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here

uniform float Time;

// Output
layout(location = 0) out vec2 texture_coord;

void main()
{
	// TODO(student): Emit the vertices of the triangle for which
	// information is received in the input attributes. Set the
	// following information for each vertex:
	//   - The coordinate of the vertex in clip space, transformed
	//     from world space, as received from the vertex shader
	//   - Texture coordinate received from the vertex shader.

	float duration = 4;
	float current_time = mod(Time,duration);
	float explosion_factor;

	if(current_time < duration)
	{
		explosion_factor = current_time / duration;
	}

	vec3 normal = normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));
	
	for(int i=0; i<gl_in.length(); i++)
	{
		vec4 position = vec4(gl_in[i].gl_Position.x + normal.x * explosion_factor, gl_in[i].gl_Position.y + normal.y * explosion_factor, gl_in[i].gl_Position.z + normal.z * explosion_factor, 1);
		gl_Position = Projection * View * position;

        texture_coord = v_texture_coord[i];
        EmitVertex();
     }
        
     EndPrimitive();
	 
}
