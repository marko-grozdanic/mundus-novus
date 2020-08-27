#version 460 core
out vec4 FragColor;
  
in vec2 TexCoord;
in float heightPos;

uniform sampler2D texture1;

void main()
{
    float colourShade = abs(heightPos) / 10;
    FragColor = vec4(colourShade, 0.0, 0.0, 1.0);
    //FragColor = texture(texture1, TexCoord);
}