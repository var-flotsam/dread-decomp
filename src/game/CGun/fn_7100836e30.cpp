// A copy of the sonar's resource loader (src/game/CAbilitySonar.cpp, base/UnkResKey.h) in the CGun unit: it
// loads a texture and keeps its holders in the UnkResHolder2 layout.
#include "base/UnkResKey.h"

bool fn_7100836e30(UnkResHolder2* self, const base::global::CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e4540f8>(self, path);
}
