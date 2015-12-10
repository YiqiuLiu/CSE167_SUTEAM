#version 330 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
in vec4 FragPosLightSpace;

uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D texture_diffuse1;

float ShadowCalculation(vec4 fragPosLightSpace)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // Transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // Get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // Get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // Calculate bias (based on depth map resolution and slope)
    vec3 normal = normalize(Normal);
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 lightDir = normalize(lightPos);
    float bias = min(0.005 * (1.0 - dot(normal, lightDir)), 0.005);
	//float bias = 0;
    // Check whether current frag pos is in shadow
    // float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r; 
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;        
        }    
    }
    shadow /= 9.0;
    
    // Keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
        shadow = 0.0;
        
    return shadow;
}

void main()
{
    vec4 color_1 = vec4(texture(texture_diffuse1, TexCoords));
    
    // Ambient
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.4f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambient + diffuse + specular) * color_1.xyz;
//    color = vec4(result, 1.0f);
    color = color_1;
}