#version 330 core

    layout(location = 0) in vec3 position;

    uniform mat4 model;
    uniform mat4 projection;

    out vec4 vColor;

    void main() 
	{
        gl_Position = projection * model * vec4(position, 1.0);
        vColor = vec4(clamp(position, 0.0f, 1.0f), 1.0f);
    }