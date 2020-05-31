# Vofog Game Engine

![SS1](https://github.com/Compiler/Vofog-Game-Engine/blob/master/Vofog-Game-Engine/Vofog/ss/ss1.PNG)
## Public version of Vofog Engine (in development)

Vofog is a game engine that is focused on lightweight and fast development to test/launch feutures quickly.

### Feature list
- Fully featured rendering engine
- ImGui UI 
  - Drag & Drop
    - Level design
    - Drag/Drop lighting
    - scaling / rotating / moving entites
- Scripting system
  - Full Lua scripting support
  - write scripts at runtime in UI, just save script and watch changes
- Bullet Physics
  - Collision shape will map to entities based on model, primitive, etc 
    - LOD slider for how precise collision shape is
- Fully Pure ECS
- Save/Load(copy/paste) individual scenes
  - Serialized with binary but supports all C++17 features
- Fully programmable shader stack
  - GPU pipeline modifiable (limited due to OpenGL context)

![SS1](https://github.com/Compiler/Vofog-Game-Engine/blob/master/Vofog-Game-Engine/Vofog/ss/ss2.PNG)
