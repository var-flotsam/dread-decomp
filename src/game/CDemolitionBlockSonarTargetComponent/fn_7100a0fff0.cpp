// A copy of the sonar's resource loader (src/game/CAbilitySonar.cpp, base/UnkResKey.h) in the CDemolitionBlockSonarTargetComponent
// unit: it loads a model and keeps its holders at other offsets.
#include "base/UnkResKey.h"
#include "types.h"

struct UnkDemolitionModelHolder {
    u8 pad_0x0[0xa];
    u8 flag;  // 0xa, passed to the resource lookup
    u8 pad_0xb[0x5];
    UnkResRef first;  // 0x10
    u8 pad_0x18[0x8];
    u8 notified[0x8];  // 0x20, an object told about the change
    u8 pad_0x28[0x70];
    UnkResRef second;  // 0x98

    UnkResHolderNotified* Notified() { return reinterpret_cast<UnkResHolderNotified*>(notified); }
};
static_assert(__builtin_offsetof(UnkDemolitionModelHolder, second) == 0x98, "UnkDemolitionModelHolder layout");

bool fn_7100a0fff0(UnkDemolitionModelHolder* self, const base::global::CFilePathStrId& path) {
    return UnkResHolderLoad<g_unk_716e453ff8>(self, path);
}
