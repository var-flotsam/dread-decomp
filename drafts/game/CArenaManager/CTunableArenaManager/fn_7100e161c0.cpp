// CArenaManager::CTunableArenaManager's unit (tools/tu_map.py, unit 2558): finds the object named "TG"
// followed by a name without its first two characters, through the game manager's id lookup.
// Names and layouts are placeholders.
#include "../../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntStringAssign.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CRntString;

using UnkNameBuffer_le161c0 = UnkBufferString<0x100, g_unk_7101a194b8>;

struct UnkArenaName_le161c0 {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u8 pad_0x40[0x4];
    s32 reads;  // 0x44
};

struct UnkArenaNameRef_le161c0 {
    UnkArenaName_le161c0* name;
};

void* fn_7100d7842c(void* found);

// The object the manager holds under the key of `name` (its CRC-64), looked up with `flags`.
inline void* FindByName_le161c0(UnkGameManager* manager, u32 kind, const CRntString* name) {
    u64 id;
    fn_7100066630(&id, name);
    alignas(8) u32 flags = kind;
    return fn_7100deba8c(manager, &id, &flags);
}

void* fn_7100e161c0(void* self, UnkArenaNameRef_le161c0* ref) {
    UnkNameBuffer_le161c0 text("TG");
    {
        ++ref->name->reads;
        const CRntString& name = ref->name->text;
        u32 restLength = name.length - 2;
        const char* rest = name.data + 2;
        UnkNameBuffer_le161c0 suffix;
        fn_710007e7f8(suffix.Str(), rest, restLength);
        fn_710007f3b4(text.Str(), suffix.Str());
    }

    void* found = FindByName_le161c0(g_unk_7101d47310, g_unk_71727bebb8, text.Str());
    return found ? fn_7100d7842c(found) : nullptr;
}
