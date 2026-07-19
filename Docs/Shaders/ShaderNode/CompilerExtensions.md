# ShaderNode Flower Compiler Extensions

## Modules
Flower Shader Compiler allows usage of modules from external `.shaderlib` and `.shaderbase` files.

Usage:
```json
{
    "shaderName": "MyTilemapProceduralLayer",
    "shaderType": "fragment",
    "version": 1,
    "modules": ["TilemapProceduralLayerFragmentBase"],
    "functions": { ... }
}
```

## Editor View
Flower Editor features a node editor. To store its spatial representation it provides `view` blocks in functions

Usage:
```json
{
    "functions": {
        "main": {
            "nodes": {
                "node_1": {...}
            },
            "conn": [
                {...}
            ],
            "view": {
                "node_1": {
                    "x": 13,
                    "y": 258 
                }
            }
        }
    }
}
```