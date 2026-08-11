#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

uniform sampler2D u_Texture;
uniform sampler2D u_ShadowMap;
uniform vec3 u_ViewPos = vec3(0.0, 0.0, 0.0);
uniform vec3 u_LightPos = vec3(0.0, 10.0, 0.0);
uniform vec3 u_LightColor = vec3(1.0, 1.0, 1.0);

uniform float u_LightConstant = 1.0;
uniform float u_LightLinear = 0.09;
uniform float u_LightQuadratic = 0.032;

float calculateShadow(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5; // [-1; 1] -> [0; 1]
    if (projCoords.z > 1.0) {
        return 0.0;
    }

    float bias = max(0.005 * (1.0 - dot(normal, lightDir)), 0.0005);
    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(u_ShadowMap, 0);
    for (int x = -1; x <= 1; ++x){
        for (int y = -1; y <= 1; ++y){
            float pcfDepth = texture(u_ShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            if (projCoords.z - bias > pcfDepth) {
                shadow += 1.0;
            }
        }
    }
    shadow /= 9.0;
    return shadow;
}

void main() {

    vec3 color = texture(u_Texture, TexCoord).rgb;
    // фоновый свет
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * u_LightColor;

    // используем диффузную модель света
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(u_LightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * u_LightColor;

    // модель Блинна-Фонга https://habr.com/ru/articles/353054/
    vec3 viewDir = normalize(u_ViewPos - FragPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    vec3 specular = spec * u_LightColor * 0.4;

    // затухание света с расстоянием
    float distance = length(u_LightPos - FragPos);
    float attenuation = 1.0 / (u_LightConstant + u_LightLinear * distance + u_LightQuadratic * (distance * distance));
    
    float shadow = calculateShadow(FragPosLightSpace, norm, lightDir);
    vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * attenuation;
    FragColor = vec4(lighting * color, texture(u_Texture, TexCoord).a);
}
