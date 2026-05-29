# Render Loop
Render Loop consists of following steps

| Step                    |          Description          |
|-------------------------|         -------------         |
| Execute Render Jobs | Executes Render Jobs enqueued in a [RenderView](./RenderView.md), such as relief map autogeneration|
| Compile Elements | Sorts [RenderElements](./RenderElement.md) in [RenderObjects](./RenderObject.md) provided by RenderView [RenderView](./RenderView.md) based on their target render pass and requests data & resource compilation of RenderElements in RenderPass|
| State Update | [RenderState](./RenderState.md) updates provided by [RenderView](./RenderView.md) are performed |
| Prepare Frame | [RenderPasses](./RenderPass.md) prepare and upload CPU data onto GPU|
| Render | Renders frame |
| Cleanup | Resets frame's RenderView and switches frame in flight|

## Execute Render Jobs
Before we start actual frame proccessing, we might need to generate resources for it, hence this stage runs first and ensures all requested runtime resources are generated before we start processing the frame.

`Compile Elements` stage decides whether [RenderResource](./RenderResource.md) is valid based on unique ID, therefore runtime assets might be required to be generated beforehand in this stage.

## Compile Elements
This is the first stage of frame processing. It disassembles all [RenderObjects](./RenderObject.md), retrieves RenderElements from them and requests target [RenderPasses](./RenderPass.md) to compile resources and elements into more efficient storage.

For example, OpaqueRenderPass stores all compiled elements data in a hash map like `std::unordered_map<uint64_t, OpaqueSpriteElementBatch> m_dynamicOpaqueSpriteElements;`, where key is an atlas ID and value is a batch structure containing staging Assembly and GPU buffers:

```c++
struct OpaqueSpriteElementBatch
{
    struct GPUBufferData {
        Float2 position;
        uint32_t depth;
        Float2 scale;
        float rotation;
        Float3 tint;
        Float4 uv;
    };
public:
    constexpr static size_t elementSize = sizeof(GPUBufferData);

    // Assembly
    std::vector<GPUBufferData> assembly;
    bool bIsDirty = false;

    // Compiled resources
    std::array<SDL_GPUBuffer*, FRAMES_IN_FLIGHT> uniformBuffer = {};
    std::array<SDL_GPUTransferBuffer*, FRAMES_IN_FLIGHT> transferBuffer = {};
    std::array<size_t, FRAMES_IN_FLIGHT> bufferSize = {};
};
```

## State Update

## Prepare Frame

## Render

## Cleanup