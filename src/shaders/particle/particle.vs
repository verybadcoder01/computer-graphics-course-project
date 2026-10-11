#version 330 core

layout (location = 0) in vec2 aQuadPos;
layout (location = 1) in vec2 aTexCoord;

layout (location = 2) in vec3 iPosition;
layout (location = 3) in float iLife;
layout (location = 4) in float iRadius;

uniform mat4 u_View = mat4(1.0);
uniform mat4 u_Projection = mat4(1.0);

out float Life;
out vec2 TexCoord;
out vec4 ViewPos;

void main() {
    Life = iLife;
    // аналогично aQuadPos.xy + vec2(0.5)
    TexCoord = aTexCoord;

    vec4 viewCenter = u_View * vec4(iPosition, 1.0);
    float diameter = iRadius * 2.0;
    vec3 billboardViewPos = vec4(viewCenter.xy + aQuadPos.xy * diameter, viewCenter.z, 1.0);
    ViewPos = billboardViewPos;
    gl_Position = u_Projection * billboardViewPos;
}
