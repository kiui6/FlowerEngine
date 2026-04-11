Example structure
```json
// entity/MyEntity.json
{
  "display": "My Entity",
  "components":
  [
    {
        "type": "health",
        "value": 100
    },
    {
        "type": "custom_value",
        "id": "ability_cooldown",
        "value_type": "float",
        "value": 15.0
    },
    {
        "type": "movement",
        "walking": true,
        "flying": true,
        "walking_speed": 200,
        "flying_speed": 50
    },
    {
        "type": "behavior",
        "value": "behavior/MyEntity"
    },
    {
        "type": "animation_tree",
        "value": "animation_tree/MyEntity"
    },
    {
        "type": "renderer",
        "skeletal_mesh": "skeletal/MyEntity.gltf",
        "material": {
            "template": "material/3D_Shaded_Opaque",
            "override": [
                {
                    "id": "albedo",
                    "value": "texture/MyEntityAlbedo.png"
                }
            ]
        },

    }
  ]
}
```