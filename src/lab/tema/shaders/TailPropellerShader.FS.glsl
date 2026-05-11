#version 330

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color = glm::vec4(0.5,0.5,0,1);

    out_color = color;
}
