#version 330 core
out vec4 FragColor;
  
in vec3 Normal;
in vec3 FragPos;


void main()
{
	// properties
    //vec3 norm = normalize(Normal);
    
    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f); 
}
