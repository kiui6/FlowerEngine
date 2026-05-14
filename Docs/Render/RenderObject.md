# Render Object

Render Objects correspond to a single Actor that produces them. They can be static or dynamic, which influences their rules of updating.

Render Objects contain multiple [RenderElement](./RenderElement.md), that provide actual draw instructions for [RenderPasses](./RenderPass.md) to the [RenderEngine](./RenderEngine.md).  

## Static vs Dynamic
### Static
Static Render Objects are kept persistent in [RenderView](./RenderView.md) while they're being referenced. They're only recompiled once something is added or removed. Static Render Objects can't be updated after they're added to the RenderView, unless they're implicitly removed and reloaded with static render objects list invalidation(see `RenderView::RemoveStaticRenderObject()`).

After [RenderView](./RenderView.md) is passed to the [RenderEngine](./RenderEngine.md), if dirty flag is set(meaning something was added or removed, since direct updates are not supported for static objects), Static Render Objects will be checked against which exact batch was changed and will be recompiled if dirty.

### Dynamic
Dynamic Render Objects are expected to be volatile and be updated every frame, therefore renderer applies dirtiness optimizations loosely. This means that GPU buffers of one specific batch will be updated completely if at least one object in it is marked dirty.