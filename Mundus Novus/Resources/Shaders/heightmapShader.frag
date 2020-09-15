#version 460 core
out vec4 FragColor;
  
in float heightPos;

uniform vec2 heightParams; //x - min height, y - max height

void main()
{
    float normalisedHeight = ((heightPos - heightParams.x) / (heightParams.y - heightParams.x));
    FragColor = vec4(normalisedHeight, 0.0, 0.0, 1.0);
}