#version 330 core

#define NUM_MAX_PLIGHTS 4
#define NUM_MAX_SLIGHTS 4

struct Material {
sampler2D texDifuse;
sampler2D texSpecular;
float texShininess;
};  

struct PLight {
vec3 lightPos;
vec3 lightColor;
float c1;
float c2;
float c3;
float ambientStr;
float difuseStr;
float specularStr;
};

struct DLight {
vec3 lightDirection;
vec3 lightColor;
float c1;
float c2;
float c3;
float ambientStr;
float difuseStr;
float specularStr;
};

struct SLight {
vec3 lightPos;
vec3 lightDirection;
vec3 lightColor;
float c1;
float c2;
float c3;
float maxAngle;
float minAngle;
float ambientStr;
float difuseStr;
float specularStr;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
out vec4 color;

uniform Material material;
uniform vec3 cubeColor;
  
uniform vec3 viewPos;
uniform DLight dlight;
uniform PLight plight[NUM_MAX_PLIGHTS];
uniform SLight slight[NUM_MAX_SLIGHTS];

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection);
vec3 PointLight(PLight light, vec3 Normal, vec3 viewDirection);
vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDirection);

void main(){
vec3 res;
for(int i = 0; i < NUM_MAX_PLIGHTS; i++){
res += PointLight(plight[i], Normal, viewPos);
}

for(int i = 0; i < NUM_MAX_SLIGHTS; i++){
res += SpotLight(slight[i], Normal, viewPos);
}

res += DirectionalLight(dlight, Normal, viewPos);

color =  vec4(res, 1.0f);
} 

vec3 DirectionalLight(DLight light, vec3 Normal, vec3 viewDirection){
//Luz Ambiental
    
vec3 ambient = light.ambientStr * light.lightColor*vec3(texture(material.texDifuse,TexCoords));

//Luz Difusa
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.lightDirection);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.difuseStr* light.lightColor*vec3(texture(material.texDifuse,TexCoords));
    
    //Luz Specular

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.texShininess);
    vec3 specular = light.specularStr * spec * light.lightColor*vec3(texture(material.texSpecular,TexCoords));  
        
    return (ambient + diffuse + specular) * cubeColor;
}

vec3 PointLight(PLight light, vec3 Normal, vec3 viewDir){
      // Ambient
    vec3 ambient = light.ambientStr * light.lightColor * vec3(texture(material.texDifuse,TexCoords));
    
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.difuseStr *  light.lightColor *vec3(texture(material.texDifuse,TexCoords));
    
    // Specular

     viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.texShininess);
    vec3 specular = light.specularStr * spec * light.lightColor * vec3(texture(material.texSpecular,TexCoords)); 
    
    float d = length(light.lightPos-FragPos);
    float test = 1.8;
    float Fatt = 1/(light.c1+(light.c2*d)+0.4*(d*d));
        
    return (Fatt*(ambient + diffuse + specular)) * cubeColor;
}

vec3 SpotLight(SLight light, vec3 Normal, vec3 viewDir){

    float Phi = cos(radians(light.maxAngle ));
    vec3 lightDir = normalize(light.lightPos - FragPos);
     float Theta = dot(lightDir, normalize(-light.lightDirection));
    float d = length(light.lightPos-FragPos);
    float Fatt = 1/(light.c1+light.c2*d+1.8f*pow(d,2));

    float epsilon = cos(Phi)-cos(light.minAngle);
    float Intensidad = clamp((Theta - Phi)/epsilon, 0,1);

    // Ambient

    vec3 ambient = light.ambientStr * light.lightColor * vec3(texture(material.texDifuse, TexCoords));

    // Diffuse 
    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * light.difuseStr *light.lightColor * vec3(texture(material.texDifuse, TexCoords));

    // Specular
    viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.texShininess);
    vec3 specular = light.specularStr * spec * light.lightColor * vec3(texture(material.texDifuse, TexCoords));  

    diffuse *= Intensidad;
    specular *= Intensidad;
        
    return (Fatt*(ambient + diffuse + specular)) * cubeColor;
}