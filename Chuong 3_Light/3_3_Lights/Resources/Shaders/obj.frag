#version 330
out vec4 FragColor;

in vec2 textCoord;
in vec3 normal;
in vec3 modelPos;

struct Material{
    vec3 ambient;
    sampler2D diffuse; // vec3 diffuse;
    sampler2D specular;// vec3 specular;
    float shininess;
};

struct DirLight{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight{
    vec3 lightPos;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float quadratic;
    float linear;
    float constant;
};
// SpotLight= PointLight + DirLight
struct SpotLight{
    vec3 lightPos;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float quadratic;
    float linear;
    float constant;

    float cutOff;
    float outerCutOff;
};
#define NR_POINT_LIGHTS 4

uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform SpotLight spotLight;
uniform vec3 viewPos;
uniform bool useBlinnPhong;
uniform bool useGama;
//uniform bool noTex;

vec3 calDirLight(DirLight light, vec3 norm, vec3 viewDir, vec3 ambientMap, vec3 diffuseMap, vec3 specularMap);
vec3 calPointLight(PointLight light, vec3 norm,vec3 modelPos, vec3 viewDir, vec3 ambientMap, vec3 diffuseMap, vec3 specularMap);
vec3 calSpotLight(SpotLight light, vec3 norm,vec3 modelPos, vec3 viewDir, vec3 ambientMap, vec3 diffuseMap, vec3 specularMap);

void main(void)
{
    vec3 norm = normalize(normal);
    vec3 viewDir= normalize(viewPos - modelPos);
    // Tính toán lại diffuse và specular
    vec3 ambientMap;
    vec3 diffuseMap;
    vec3 specularMap;
    bool noTex= false;
   // if(noTex){
        // nếu material
        // {…
        //  vec3 diffuse;
        //  vec3 specular; ..}
       // ambientMap= material.ambient;
       // diffuseMap= material.diffuse;
       // specularMap= material.specular;
    //}else{
        // nếu material
        // {…
        //  sampler2D diffuse;
        //  sampler2D specular; ..}
        ambientMap= texture(material.diffuse, textCoord).xyz;
        diffuseMap= texture(material.diffuse, textCoord).xyz;
        specularMap= texture(material.specular, textCoord).xyz;
    //}
    vec3 result = vec3(0,0,0);
    // … Tính toán result
    //      if  (useGama){
    //         float gama= 2.2;
    //         result = pow(result, vec3(1.0/gama)) ;
    //      }
    // combine multiphe lights
    // phase 1: directional lighting
     result += calDirLight(dirLight, norm, viewDir,ambientMap, diffuseMap,specularMap);
    // phase 2: point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += calPointLight(pointLights[i], norm, modelPos, viewDir,ambientMap, diffuseMap,specularMap);
//    // phase 3: spot light
      result += calSpotLight(spotLight, norm, modelPos, viewDir, ambientMap, diffuseMap,specularMap);
    FragColor = vec4(result, 1.0f);
}
vec3 calDirLight(DirLight light, vec3 norm, vec3 viewDir, vec3 ambientMap, vec3 diffuseMap, vec3 specularMap  ){
    vec3 lightDir = normalize(light.direction-modelPos); //cách 2: vec3 lightDir = normalize(-dirLight.direction);
    //Diffuse shading
    float diff = max(dot(norm,lightDir),0.0f);
    // combine result
    vec3 ambient= light.ambient * ambientMap;
    vec3 diffuse=light.diffuse*( diffuseMap * diff);
    // specular shading
    vec3 specular= vec3(0,0,0);
    if (diff >0){
        float dotProd =0.0;
        if (useBlinnPhong){
            // calculate using Blinn-Phong model
            vec3 halfwaydir = normalize(lightDir + viewDir);
            dotProd = dot(norm,halfwaydir);
        }else{
            // calculate using Phong model
            vec3 reflectDir = reflect(-lightDir,norm);
            dotProd = dot(viewDir,reflectDir);
        }
        float spec = pow(max(dotProd,0.0f),material.shininess);
        specular = light.specular*(spec * specularMap  );
    }

    return ambient + diffuse + specular;
}
vec3 calPointLight(PointLight light, vec3 norm,vec3 modelPos, vec3 viewDir, vec3 ambientMap, vec3 diffuseMap, vec3 specularMap  ){
    vec3 lightDir = normalize(light.lightPos-modelPos);
    //Diffuse shading
    float diff = max(dot(norm,lightDir),0.0f);
    // combine result
    vec3 ambient= light.ambient * ambientMap;
    vec3 diffuse=light.diffuse*( diffuseMap * diff);

    // specular shading
    vec3 specular= vec3(0,0,0);
    if (diff >0){
        float dotProd =0.0;
        if (useBlinnPhong){
            // calculate using Blinn-Phong model
            vec3 halfwaydir = normalize(lightDir + viewDir);
            dotProd = dot(norm,halfwaydir);
        }else{
            // calculate using Phong model
            vec3 reflectDir = reflect(-lightDir,norm);
            dotProd = dot(viewDir,reflectDir);
        }
        float spec = pow(max(dotProd,0.0f),material.shininess);
        specular = light.specular*(spec * specularMap  );
    }

    // attenuation
    float Distance = length(light.lightPos - modelPos);
    float attenuation = 1.0f/ (light.constant + light.linear*Distance + light.quadratic *Distance);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return ambient + diffuse + specular;
}
vec3 calSpotLight(SpotLight light, vec3 norm,vec3 modelPos, vec3 viewDir, vec3 ambientMap, vec3 diffuseMap, vec3 specularMap ){
    vec3 lightDir = normalize(light.lightPos-modelPos);
    float theta= dot(lightDir, normalize(-light.direction));
    vec3 ambient = light.ambient * diffuseMap;
    if(theta >spotLight.outerCutOff){ // > because using cosines and not degrees
        //Diffuse shading
        float diff = max(dot(norm,lightDir),0.0f);
        // combine result
        vec3 diffuse=light.diffuse*( diffuseMap * diff);
        // specular shading
        vec3 specular= vec3(0,0,0);
        if (diff >0){
            float dotProd =0.0;
            if (useBlinnPhong){
                // calculate using Blinn-Phong model
                vec3 halfwaydir = normalize(lightDir + viewDir);
                dotProd = dot(norm,halfwaydir);
            }else{
                // calculate using Phong model
                vec3 reflectDir = reflect(-lightDir,norm);
                dotProd = dot(viewDir,reflectDir);
            }
            float spec = pow(max(dotProd,0.0f),material.shininess);
            specular = light.specular*(spec * specularMap  );
        }
        // attenuation
        float Distance = length(light.lightPos - modelPos);
        float attenuation = 1.0f/ (light.constant + light.linear*Distance + light.quadratic *Distance);
        // soft Edges
        float epsilon = light.cutOff- light.outerCutOff;
        float intensity = clamp((theta-light.outerCutOff)/epsilon,0.0f,1.0f);

        ambient *= attenuation*intensity;
        diffuse *= attenuation*intensity;
        specular *= attenuation*intensity;

        return ambient + diffuse + specular;
    } else{
        // render just ambient
        return ambient;
    }
}
