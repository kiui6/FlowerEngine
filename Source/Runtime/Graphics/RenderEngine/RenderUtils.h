#pragma once

#ifdef NDEBUG
#   define BeginGPULabel(cmd, label)
#   define EndGPULabel(cmd)
#else
#   define BeginGPULabel(cmd, label) SDL_PushGPUDebugGroup( cmd , label )
#   define EndGPULabel(cmd) SDL_PopGPUDebugGroup( cmd )
#endif