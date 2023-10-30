#shader vertex
#version 330 core
layout (location = 0) in vec3 inPosition;

void main()
{
    gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
}

#shader fragment
#version 330 core
out vec4 fragColor;

void main()
{
    fragColor = vec4(1.0);
}