#version 460 core
layout (location = 0) in vec3 aPos;

out float heightPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	heightPos = FragPos.y;
}