#shader vertex
#version 330 core
layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;
out vec4 FragColor;

void main()
{
    gl_Position = vec4(inPosition.x, inPosition.y, inPosition.z, 1.0);
    FragColor = vec4(inColor.x, inColor.y, inColor.z, 1.0);
}

#shader fragment
#version 330 core
in vec4 FragColor;
out vec4 fragColor;

void main()
{
    fragColor = vec4(FragColor.x, FragColor.y, FragColor.z, 1.0);
}