# Tilemap

An extensive tilemap based environment, allowing users to compose complex tilemaps constructed of dozens of Atlas or Procedural layers.

Can contain 2 types of layers:
* [Atlas Layer](./World/Tilemap/AtlasLayer.md) - Basic textured layer, allows to texture tiles of a chunk with a sprite from an atlas.
* [Procedural Layer](./World/Tilemap/ProceduralLayer.md) - Supports only a binary alpha mask. Allows applying any orbitrary shader onto the masked surface of a chunk.