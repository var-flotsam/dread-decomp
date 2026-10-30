// A copy of the sonar's resource loader (src/game/CAbilitySonar.cpp, base/UnkResKey.h) in unit 2557 of tools/tu_map.py (0x7100e0f028-0x7100e140c0, no reflected class): it
// loads a texture and keeps its holders in the UnkResHolder2 layout.
#include "base/UnkResKey.h"

bool fn_7100e13c10(UnkResHolder2* self, const base::global::CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e4540f8>(self, path);
}
