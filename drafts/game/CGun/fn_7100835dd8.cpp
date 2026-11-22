// CGun unit: from a non-empty name id, builds the ids "<name>0" and "<name>1" and stores them at
// +0x50 and +0x58. Names and layouts are placeholders.
#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkGunIds_l835dd8 {
    u8 pad_0x0[0x50];
    CStrId first;   // 0x50
    CStrId second;  // 0x58
};

// The id's string, counting the read.
inline const CRntString& IdString(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

void fn_7100835dd8(UnkGunIds_l835dd8* self, const CStrId* name) {
    if (name->data == &g_unk_7101d47108->empty)
        return;

    // Room for the ids fn_7100000850 builds; each is handed on, leaving the empty id behind.
    alignas(CStrId) u8 slot[sizeof(CStrId)];
    CStrId* id = reinterpret_cast<CStrId*>(slot);
    const CRntString& text = IdString(*name);
    UnkBufferString<0x80, g_unk_7101a1b750> buffer;
    CRntString_Assign(buffer.Str(), &text);
    CRntString_AppendCString(buffer.Str(), "0");
    fn_7100000850(id, *buffer.Str(), true);
    self->first.ReleaseAndTake(*id);
    id->~CStrId();

    CRntString_Assign(buffer.Str(), &IdString(*name));
    CRntString_AppendCString(buffer.Str(), "1");
    fn_7100000850(id, *buffer.Str(), true);
    self->second.ReleaseAndTake(*id);
    id->~CStrId();
}
