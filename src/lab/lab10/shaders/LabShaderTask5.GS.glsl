#version 430

// Input and output topologies
layout(triangles) in;
layout(line_strip, max_vertices = 170) out;

// Input
layout(location = 1) in vec2 v_texture_coord[];
layout(location = 0) in vec3 normal[];

// Uniform properties
uniform mat4 View;
uniform mat4 Projection;
// TODO(student): Declare other uniforms here


// Output
layout(location = 0) out vec3 color;

void main()
{
	vec3 normal = normalize(cross(gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz));
	normal /= 10;

	for(int i=0; i<1; i++)
	{
		vec4 position = vec4(gl_in[i].gl_Position.x, gl_in[i].gl_Position.y, gl_in[i].gl_Position.z, 1);
		gl_Position = Projection * View * position;
        color = vec3(0,0,1);
        EmitVertex();


		position = vec4(gl_in[i].gl_Position.x + normal.x, gl_in[i].gl_Position.y + normal.y, gl_in[i].gl_Position.z + normal.z, 1);
	    color = vec3(0,0,1);
	    gl_Position = Projection * View * position;
        EmitVertex();
     }
        
     EndPrimitive();
	 
}
