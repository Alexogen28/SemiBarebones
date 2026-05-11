#version 330

// Output
layout(location = 0) out vec4 out_color;


void main()
{
    vec4 color = glm::vec4(1,0.4f,0.5f,1);

    out_color = color;
}
