// A copy of the sonar's resource loader (src/game/CAbilitySonar.cpp, base/UnkResKey.h) in unit 441 of
// tools/tu_map.py (0x710035d4e0-0x710035debc, no reflected class): it loads a texture and keeps its
// holders at other offsets (UnkResHolder2).
#include "base/UnkResKey.h"

bool fn_710035db30(UnkResHolder2* self, const base::global::CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e4540f8>(self, path);
}
