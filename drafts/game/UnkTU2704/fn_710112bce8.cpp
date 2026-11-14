// CSpawnPointComponent's unit: looks a string property up by key in a property bag and reads it into a string id.
// Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CStrId;

void fn_710007ca94(CRntFile* file, char* buf);

struct UnkPropKey {
    u8 pad_0x0[0x30];
    u64 key;
};

struct UnkPropKeyRef {
    UnkPropKey* target;
};

struct UnkPropEntry {
    u64 offset;
    u64 key;
    u32 type;
    u32 pad_0x14;
};

struct UnkPropBag {
    UnkPropEntry* entries;
    u32 count;
    u32 pad_0xc;
    u8 pad_0x10[0x18];
    CRntFile file;
};

bool fn_710112bce8(UnkPropBag* bag, UnkPropKeyRef* ref, CStrId* out) {
    UnkPropEntry* end = bag->entries + bag->count;
    UnkPropEntry* it = bag->entries;
    u64 key = ref->target->key;
    if (bag->count != 0 && it->key != key) {
        for (;;) {
            if (++it == end)
                return false;
            if (it->key == key)
                break;
        }
    }
    if (it == end || it->type != 0x73)
        return false;
    fn_710007c00c(&bag->file, it->offset, 0);
    char buf[0x100];
    fn_710007ca94(&bag->file, buf);
    CStrId id(buf, true);
    out->ReleaseAndTake(id);
    return true;
}
