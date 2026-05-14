# RenderStateStore

Render State Store contains mutable global state for the renderer. It's a noexcept nofail Get-Or-Default container for [RenderState](./RenderState.md).

It's expected for RenderStateStore to be passed as a reference in a constructor for object to retrieve states it needs using `const T& RenderStateStore::Get<T>()`. Example code:

```c++
// On RenderPass creation
m_renderPasses[(uint32_t)RenderPassType::Opaque] = std::make_unique<OpaqueRenderPass>(m_ctx, m_stateStore);

// Inside the constructor
OpaqueRenderPass::OpaqueRenderPass(GPUContext& context, RenderStateStore& stateStore)
: m_gpu(context), tilemapState(stateStore.Get<TilemapRenderState>())
{
    ...
}
```
This ensures zero runtime overhead.

`const T& RenderStateStore::Get<T>()` and `T& RenderStateStore::GetMutable<T>()` can never fail, if state isn't initialized yet, it will be initialized inside the method.

It's valid for consumer to get both mutable and immutable references depending on the needs.

### Thread Safety
All render code is guaranteed to be called only inside the main thread, hence no synchronization or atomicity is required.