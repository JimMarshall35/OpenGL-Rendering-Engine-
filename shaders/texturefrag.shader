#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;

in vec2 TexCoords;
uniform sampler2D ourTexture;

void main()
{
	// properties
    //vec3 norm = normalize(Normal);
    
    FragColor = texture(ourTexture, TexCoords);
}
