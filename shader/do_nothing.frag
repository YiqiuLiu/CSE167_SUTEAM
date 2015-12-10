#version 330 core
out vec4 Fragcolor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
    vec2 TexCoord;
} fs_in;

uniform sampler2D gSampler0;
uniform sampler2D gSampler1;
uniform sampler2D gSampler2;
uniform sampler2D gSampler3;
uniform sampler2D gSampler4;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform bool shadows;
uniform float fRenderHeight;
uniform float fMaxTextureU;
uniform float fMaxTextureV;

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
    vec3 normal = normalize(fs_in.Normal);
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 lightDir = normalize(lightPos);
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.00);
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
    // terrain shader
    vec3 vNormalized = normalize(fs_in.Normal);
    
    vec4 vTexColor = vec4(0.0);
    
    float fScale = fs_in.FragPos.y/fRenderHeight;
    
    const float fRange1 = 0.10f;
    const float fRange2 = 0.15f;
    const float fRange3 = 0.20f;
    const float fRange4 = 0.30f;
    const float fRange5 = 0.35f;
    const float fRange6 = 0.45f;
    const float fRange7 = 0.60f;
    const float fRange8 = 0.65f;
    if(fScale >= 0.0 && fScale <= fRange1)vTexColor = texture(gSampler0, fs_in.TexCoord);
    else if(fScale <= fRange2)
    {
        fScale -= fRange1;
        fScale /= (fRange2-fRange1);
        
        float fScale2 = fScale;
        fScale = 1.0-fScale;
        
        vTexColor += texture(gSampler0, fs_in.TexCoord)*fScale;
        vTexColor += texture(gSampler1, fs_in.TexCoord)*fScale2;
    }
    else if(fScale <= fRange3)vTexColor = texture(gSampler1, fs_in.TexCoord);
    else if(fScale <= fRange4)
    {
        fScale -= fRange3;
        fScale /= (fRange4-fRange3);
        
        float fScale2 = fScale;
        fScale = 1.0-fScale;
        
        vTexColor += texture(gSampler1, fs_in.TexCoord)*fScale;
        vTexColor += texture(gSampler2, fs_in.TexCoord)*fScale2;
    }
    else if(fScale <= fRange5)vTexColor = texture(gSampler2, fs_in.TexCoord);
    else if(fScale <= fRange6)
    {
        fScale -= fRange5;
        fScale /= (fRange6-fRange5);
        
        float fScale2 = fScale;
        fScale = 1.0-fScale;
        
        vTexColor += texture(gSampler2, fs_in.TexCoord)*fScale;
        vTexColor += texture(gSampler3, fs_in.TexCoord)*fScale2;
    }
    else if(fScale <= fRange7)vTexColor = texture(gSampler3, fs_in.TexCoord);
    else if(fScale <= fRange8)
    {
        fScale -= fRange7;
        fScale /= (fRange8-fRange7);
        
        float fScale2 = fScale;
        fScale = 1.0-fScale;
        
        vTexColor += texture(gSampler3, fs_in.TexCoord)*fScale;
        vTexColor += texture(gSampler4, fs_in.TexCoord)*fScale2;
    }
    else vTexColor = texture(gSampler4, fs_in.TexCoord);
    
    //Fragcolor = vTexColor;
    //terrain shader done
    
	// Ambient
    float ambientStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(fs_in.Normal);
    //vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	vec3 lightDir = normalize(lightPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.4f;
    vec3 viewDir = normalize(viewPos - fs_in.FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
    // Calculate shadow
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace);                      
    shadow = min(shadow, 0.75); // reduce shadow strength a little: allow some diffuse/specular light in shadowed regions
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * vTexColor.xyz;
    
    Fragcolor = vec4(lighting, 1.0f);
}