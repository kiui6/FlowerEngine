#version 450 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec2 inUV;

layout(binding = 0) buffer WorldBuffer { 
    mat4 projectionView;
    vec3 position;
} worldData;

struct Sprite {
        vec4 uv;
        vec4 tint;
        vec2 position;
        vec2 size;
        vec2 pivot;
        uint depth;
        float rotation;
};

layout(binding = 1) buffer SpriteBuffer { Sprite sprites[]; } spriteData;

layout(location = 0) out vec2 fragUV;
layout(location = 1) out vec3 fragTint;

void main() {
    Sprite sprite = spriteData.sprites[gl_InstanceIndex];

    vec4 vertexPosition;
    
    vertexPosition.x = (sprite.position.x + (inPos.x * sprite.size.x)) - worldData.position.x;
    vertexPosition.y = (sprite.position.y + (inPos.y * sprite.size.y)) - worldData.position.y;
    vertexPosition.z = float(sprite.depth) - worldData.position.z;
    vertexPosition.w = 1.0;
    
    gl_Position = worldData.projectionView * vertexPosition;

    fragUV = sprite.uv.xy + inUV * (sprite.uv.zw - sprite.uv.xy);
    fragTint = vec3(sprite.tint.x, sprite.tint.y, sprite.tint.z);
}