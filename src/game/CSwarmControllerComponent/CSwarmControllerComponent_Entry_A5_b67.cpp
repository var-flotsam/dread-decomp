// CSwarmControllerComponent's unit (tools/tu_map.py): returns a copy of entry `index` of the table reached through
// +0x1e0 (entries of 0x18 at +0x2e8: a string id, a file-path id and a counted reference, count at +0x54). Names are
// placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkCounted_A5_b67 {
    u8 pad_0x0[0x54];
    s32 count;
};

struct UnkCountedRef_A5_b67 {
    UnkCounted_A5_b67* p;
    UnkCountedRef_A5_b67(const UnkCountedRef_A5_b67& other) : p(nullptr) {
        if (other.p) {
            ++other.p->count;
            p = other.p;
        }
    }
};

struct UnkSwarmEntry_A5_b67 {
    CStrId name;
    CFilePathStrId path;
    UnkCountedRef_A5_b67 ref;
};

struct UnkSwarmTable_A5_b67 {
    u8 pad_0x0[0x2e8];
    UnkSwarmEntry_A5_b67 entries[1];
};

struct UnkSwarmController_A5_b67 {
    u8 pad_0x0[0x1e0];
    UnkSwarmTable_A5_b67* table;
};

UnkSwarmEntry_A5_b67 fn_710074e2c0(UnkSwarmController_A5_b67* self, s32 index) {
    return self->table->entries[index];
}
