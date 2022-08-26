#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_specular1;//screenTexture;

void main()
{
    FragColor = vec4(1.0f,0.0f,0.0f, 1.0f);
    //    vec3 col = texture(screenTexture, TexCoords).rgb;
    //    FragColor = vec4(col, 1.0);
}
