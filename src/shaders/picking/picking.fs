#version 330 core

layout (location = 0) out uint FragColor;

uniform uint u_ObjectID;

void main() {
    FragColor = u_ObjectID;
}
