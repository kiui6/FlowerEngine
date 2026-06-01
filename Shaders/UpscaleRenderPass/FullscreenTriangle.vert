#version 450 core

layout(location=0) out vec2 texcoords;

layout(binding = 0) readonly buffer UpscaleData {
    vec2 AspectRatioCompensation;
} upscaleData;

void main() {
    vec2 vertices[3] = vec2[3](vec2(-1,3), vec2(3,-1), vec2(-1, -1));
    gl_Position = vec4(vertices[gl_VertexIndex] * upscaleData.AspectRatioCompensation,0,1);
    texcoords = 0.5 * vertices[gl_VertexIndex] + vec2(0.5);
}