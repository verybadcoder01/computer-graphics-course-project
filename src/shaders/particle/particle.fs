#version 330 core

in float Life;
in vec2 TexCoord;
in vec4 ViewPos;

out vec4 FragColor;

uniform vec3 u_LightDir = vec3(0.2, 1.0, 0.5);
uniform vec4 u_WaterColor = vec4(0.2, 0.65, 0.9, 0.65);
uniform float u_MaxLife = 2.0;

void main() {
    if (Life <= 0.0) {
        discard;
    }

    // 2. Центрируем координаты квада: [-0.5, 0.5] -> [-1.0, 1.0]
    vec2 nCoords = (TexCoord - vec2(0.5)) * 2.0;
    float distSq = dot(nCoords, nCoords);
    if (distSq > 1.0) {
        discard;
    }

    float nz = sqrt(1.0 - distSq);
    vec3 normal = vec3(nCoords.x, nCoords.y, nz);
    vec3 lightDir = normalize(u_LightDir);
    
    // Рассеянное освещение (Diffuse)
    float diff = max(dot(normal, lightDir), 0.25); 

    // модель блинна-фонга
    vec3 viewDir = vec3(0.0, 0.0, 1.0);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    float edgeAlpha = smoothstep(1.0, 0.85, distSq);
    float lifeAlpha = clamp(Life / u_MaxLife, 0.0, 1.0);

    vec3 finalRGB = u_WaterColor.rgb * diff + vec3(1.0) * spec;
    float finalAlpha = u_WaterColor.a * edgeAlpha * lifeAlpha;
    FragColor = vec4(finalRGB, finalAlpha);
}