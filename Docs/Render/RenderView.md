# RenderView

Render View is an object that allows recording draw instructions and state changes for rendering a frame by the [RenderEngine](./RenderEngine.md). 

## Contents
| Type                               | Description |
|------------------------------------|----------|
| [RenderObject](./RenderObject.md)  | Represent a single unique actor, either static or dynamic. Render Objects consist of multiple [RenderElements](./RenderElement.md)|
| [RenderStateUpdate](./RenderStateUpdate.md)  | A payload that requests change of the states of the [RenderStateStore](./RenderStateStore.md), like tilemap's grid parameters.|
| [RenderJob](./RenderJob.md)  | Offscreen render job, performed before the render view compilation, but after the render state update. Allows preparing resources. (E.g. Relief texture auto generation)|