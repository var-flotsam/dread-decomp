// CGameManager::CTunableGameManager's unit (tools/tu_map.py): a runtime asset's path,
// "_runtime_assets/<name>.runtime", with the name taken from a string id (whose pooled data counts its reads
// at +0x44 and holds the text at +0x10). The pattern is a guarded static string with inline storage,
// destroyed at exit by fn_7100de9f54. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/UnkTU112.h"
#include "types.h"

using base::global::CRntString;

// A string with 0x100 bytes of inline storage (as src/base/UnkTunableRegistry.cpp has it).
struct UnkTunableLine {
    u8 str[0x30];
    char* storage_vtable;  // +0x30
    u8 buffer[0x100];
    u64 field_0x138;
    u32 field_0x140;

    CRntString* Str() { return reinterpret_cast<CRntString*>(str); }
    UnkTunableLine(const char* text) {
        fn_710007e6e4(Str(), reinterpret_cast<base::UnkAllocator*>(&storage_vtable));
        storage_vtable = reinterpret_cast<char*>(g_unk_7101a194b8 + 2);
        field_0x138 = 0;
        field_0x140 = 0;
        fn_710007e77c(Str(), text);
    }
    ~UnkTunableLine() asm("fn_7100de9f54");
};

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

UnkTunableLine fn_7100e05430(UnkAssetNameData_ovb0113** id) {
    UnkTunableLine& pattern = RuntimeAssetPattern_ovb0113();
    ++(*id)->reads;
    const char* text = (*id)->text;
    return fn_710013c834(pattern.Str(), "%NAME%", text ? text : g_unk_710162e2ed);
}
