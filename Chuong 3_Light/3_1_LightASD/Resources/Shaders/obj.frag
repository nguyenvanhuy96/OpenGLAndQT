#version 330 core
out vec4 FragColor;


struct Material{
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;
 float shininess;
};
struct Light {
 vec3 ambient;
 vec3 diffuse;
 vec3 specular;

 vec3 lightColor;
 vec3 lightPos;
};
uniform Material material;
uniform Light light;
uniform vec3 viewPos;


in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;

void main(){
    //FragColor = vec4(1.0f,0.0f,0.0f, 1.0f);

    // Cường độ sáng
    vec3 ambient= light.ambient * material.ambient ;
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.lightPos-FragPos);
    float diff = max(dot(norm,lightDir),0.0f);
    vec3 diffuse =light.diffuse*(material.diffuse * diff);
    //specular
    vec3 viewDir= normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
    vec3 specular = light.specular*(spec *material.specular);

    // combine
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
};
