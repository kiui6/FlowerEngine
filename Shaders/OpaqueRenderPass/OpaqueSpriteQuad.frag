#version 450 core

layout(set = 2, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragUV;
layout(location = 1) in vec3 fragTint;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = texture(texSampler, fragUV) * vec4(fragTint, 1.0);
    
    // We normaize alpha to either 0 or 1 to prioritize correctness of masked opaque sprites rendering
    if (outColor.w < 0.05) { discard; }
    else { outColor.w = 1.0; }
}