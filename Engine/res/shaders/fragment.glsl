#version 330 core
out vec4 frag_colour;

uniform vec3 viewerPosition;
uniform vec3 lightPosition;
uniform vec3 lightColour;
uniform vec3 colour;
uniform sampler2D albedoMap;

in vec3 outPosition;
in vec3 outNormal;
in vec2 outUv;

float specularStrength = 0.2f;
float ambientStrength = 0.5f;
float shinyness = 2.0f;

void main()
{
    //vec3 normalizedNormal = normalize(outNormal);
    //vec3 ambientLight = lightColour * ambientStrength;
    
    //vec3 lightDirection = normalize(lightPosition - outPosition);
    //float incidentAngle = max(dot(normalizedNormal, lightDirection), 0.0f);
    //vec3 diffuseLight = incidentAngle * lightColour;
    
    //vec3 viewDir = viewerPosition - outPosition;
    //vec3 reflectDir = reflect(-lightDirection, normalizedNormal);
    
    //float specular = pow(max(dot(viewDir, reflectDir), 0.0f), shinyness);
    //vec3 specularLight = specularStrength * specular * lightColour;
  
    //vec3 totalLight = (ambientLight + diffuseLight + specularLight) * vec3(texture(albedoMap, outUv));
    frag_colour = vec4(vec3(0.1, 0.2, 0.4), 0.0f);
}