# RenderState

Plain data container for [`RenderEngine`](./RenderEngine.md)'s global data.

## What can be in Render State
Render States supposed to store rarely updating data, both CPU & GPU resources can be stored.

Use case examples:

|  |      Use case      | Explanation |
|--|--------------------|--------------|
|✕| Camera Position    | Camera position gets updated every frame, which requires access to frame context and double buffering. Although possible, it's not efficient. Use [`FrameBufferAttachmentUpdateHandlers`](./FrameBufferAttachmentUpdateHandler.md) instead
|〇| Tilemap Grid Data  | Tilemap's global data is static after the world is loaded, and it's only updated at world load. Therefore it gets updated only once off-screen and doesn't require double buffering or advanced synchronization outside of waiting for GPU device to idle.

## OnChanged event
State's consumer can subscribe to the event using `RenderState::SubscribeToChange(user, method)`. Method is expected to have signature `void UserClass::(void)`.
You can subscribe to events even with immutable reference.

Example:
```c++
DebugUIRenderPass::DebugUIRenderPass(GPUContext &context, RenderStateStore& stateStore)
    : m_gpu(context), m_state(stateStore.Get<DebugUIRenderState>())
{
    m_state.SubscribeToChange(this, &DebugUIRenderPass::cbOnChange);
}
```