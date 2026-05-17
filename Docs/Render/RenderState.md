# RenderState

Plain data container for [RenderEngine](./RenderEngine.md)'s global data.

## OnChanged event
State's consumer can subscribe to the event using `RenderState::SubscribeToChange(user, method)`.
You can subscribe to events even with immutable reference.

Example:
```c++
DebugUIRenderPass::DebugUIRenderPass(GPUContext &context, RenderStateStore& stateStore)
    : m_gpu(context), m_state(stateStore.Get<DebugUIRenderState>())
{
    m_state.SubscribeToChange(this, &DebugUIRenderPass::cbOnChange);
}
```