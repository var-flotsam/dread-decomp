// CGameManager::CTunableGameManager's unit (tools/tu_map.py): a runtime asset's path,
// "_runtime_assets/<name>.runtime", with the name taken from a string id (whose pooled data counts its reads
// at +0x44 and holds the text at +0x10). The pattern is a guarded static string with inline storage,
// destroyed at exit by fn_7100de9f54. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkTunableLine.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkTU112.h"
#include "types.h"

using base::global::CRntString;

struct UnkAssetNameData_ovb0113 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 reads;  // 0x44
    };
};

inline UnkTunableLine& RuntimeAssetPattern_ovb0113() {
    static UnkTunableLine s_pattern asm("g_unk_71727bec28")("_runtime_assets/%NAME%.runtime");
    return s_pattern;
}

UnkTunableLine CGameManager_BuildRuntimeAssetPath(UnkAssetNameData_ovb0113** id) {
    UnkTunableLine& pattern = RuntimeAssetPattern_ovb0113();
    ++(*id)->reads;
    const char* text = (*id)->text;
    return ReplaceAllSubstrings(pattern.Str(), "%NAME%", text ? text : g_unk_710162e2ed);
}
