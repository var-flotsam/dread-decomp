// A copy of the sonar's resource loader (src/game/CAbilitySonar.cpp, base/UnkResKey.h) in unit 441 of
// tools/tu_map.py (0x710035d4e0-0x710035debc, no reflected class): it loads a texture and keeps its
// holders at other offsets.
#include "base/UnkResKey.h"
#include "types.h"

struct UnkUnit441TextureHolder {
    u8 pad_0x0[0x2];
    u8 flag;  // 0x2, passed to the resource lookup
    u8 pad_0x3[0x5];
    UnkResRef first;  // 0x8
    u8 pad_0x10[0x8];
    u8 notified[0x8];  // 0x18, an object told about the change
    u8 pad_0x20[0x70];
    UnkResRef second;  // 0x90

    UnkResHolderNotified* Notified() { return reinterpret_cast<UnkResHolderNotified*>(notified); }
};
static_assert(__builtin_offsetof(UnkUnit441TextureHolder, second) == 0x90, "UnkUnit441TextureHolder layout");

bool fn_710035db30(UnkUnit441TextureHolder* self, const base::global::CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e4540f8>(self, path);
}
