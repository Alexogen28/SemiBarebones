#version 330

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Calculate the out_color using the texture() function.

    vec4 color = glm::vec4(0.75,0,0,1);

    out_color = color;
}
