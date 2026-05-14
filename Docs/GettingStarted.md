# Flower++ Getting Started

Flower is a Pixel Perfect 2D Open World capable game engine built on persistant record system.

## Records
[Records](./Records/Record.md) are objects that contain persistant data. Each record has unique [RecordID](./Records/RecordID.md), that is unique for this object among master, all plugins, saves and runtime.
Records are stored in Master, Plugin or Save files. Records themselves don't contain any functionality, code or anything other than plain data.

## World
[Tilemap](./World/Tilemap.md) - An extensive tilemap based environment, allowing users to compose complex tilemaps constructed of dozens of Atlas or Procedural layers.

[In-World Sprite Editor](./World/SpriteEditor.md) - A built in pixel perfect sprite editor right in the game's world, allowing user to create new sprites right inside the world.

## Development Tools
[Atlas Compiler](./Assets/AtlasCompiler.md) - An automatization tool that compiles stray sprites into atlases automatically based on their appearances in the world, but can be guided by the user with support of custom grouping, rules and exclusions.