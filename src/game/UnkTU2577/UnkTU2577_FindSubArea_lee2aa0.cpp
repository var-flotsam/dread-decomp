// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee2aa0: finds the
// sub-area of the array at +0x860 (0x188 bytes each) that fn_7100fad66c accepts for the id, or null.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkSubareaIdLists;
bool fn_7100fad66c(UnkSubareaIdLists* self, const CStrId* id);

struct UnkSubAreaEntry_lee2aa0 {
    u8 pad_0x0[0x188];
};

struct UnkSubAreaMgr_lee2aa0 {
    u8 pad_0x0[0x860];
    UnkSubAreaEntry_lee2aa0* entries;  // 0x860
    u32 count;                         // 0x868
};

UnkSubAreaEntry_lee2aa0* fn_7100ee2aa0(UnkSubAreaMgr_lee2aa0* self, const CStrId* id) {
    UnkSubAreaEntry_lee2aa0* found = self->entries;
    UnkSubAreaEntry_lee2aa0* end = found + self->count;
    {
        CStrId key(*id);
        for (; found != end; ++found) {
            CStrId probe(key);
            if (fn_7100fad66c(reinterpret_cast<UnkSubareaIdLists*>(found), &probe))
                break;
        }
    }
    UnkSubAreaEntry_lee2aa0* limit = self->entries + self->count;
    return limit == found ? nullptr : found;
}
