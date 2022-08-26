#version 330 core
out vec4 FragColor;
struct Material{
    vec3 ambient;
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
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
    vec3 ambientMap= texture(material.texture_diffuse1, TexCoords).xyz;
    vec3  diffuseMap= texture(material.texture_diffuse1, TexCoords).xyz;
    vec3   specularMap= texture(material.texture_specular1, TexCoords).xyz;

    // Cường độ sáng
    vec3 ambient= light.ambient * ambientMap ;
    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.lightPos-FragPos);
    float diff = max(dot(norm,lightDir),0.0f);
    vec3 diffuse =light.diffuse*(diffuseMap * diff);
    //specular
    vec3 viewDir= normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir,norm);
    float spec = pow(max(dot(viewDir,reflectDir),0.0f),material.shininess);
    vec3 specular = light.specular*(spec *specularMap);

    // combine
    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0f);
};
