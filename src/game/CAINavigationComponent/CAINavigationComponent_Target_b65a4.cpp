// CAINavigationComponent's unit (tools/tu_map.py): the target position of the current record. The
// component's +0xb0 holds a table of objects by kind; its first entry's +0x10 is the current record,
// whose kind (+0x10) picks the object: kind 1 and kind 7 copy the position kept deep inside their
// object (+0x3810c, +0x3811c), kind 2 gives the x/y at +0xdc of its object with z 0. Returns whether a
// position was written. Names and layouts are placeholders.
#include "types.h"

struct UnkNavVec_b65a4 {
    f32 x, y, z;
};

struct UnkNavRecord_b65a4 {
    u8 pad_0x0[0x10];
    volatile s32 kind;  // 0x10 (the original re-reads it for each test)
};

struct UnkNavHead_b65a4 {
    u8 pad_0x0[0x10];
    UnkNavRecord_b65a4* record;  // 0x10
};

struct UnkNavFlat_b65a4 {
    u8 pad_0x0[0xdc];
    f32 x, y;  // 0xdc
};

struct UnkNavTable_b65a4 {
    UnkNavHead_b65a4* head;  // 0x0
    char* kind1;             // 0x8
    UnkNavFlat_b65a4* kind2; // 0x10
    u8 pad_0x18[0x20];
    char* kind7;             // 0x38
};

struct UnkNavTarget_b65a4 {
    u8 pad_0x0[0xb0];
    UnkNavTable_b65a4* table;  // 0xb0
};

// The position copied as an x/y word then z, stored in that order (a plain struct copy stores z first).
typedef u64 __attribute__((may_alias)) UnkNavPair_b65a4;
static inline void CopyVec(UnkNavVec_b65a4* out, const char* src) {
    UnkNavPair_b65a4 xy = *reinterpret_cast<const UnkNavPair_b65a4*>(src);
    f32 z = *reinterpret_cast<const f32*>(src + 8);
    *reinterpret_cast<UnkNavPair_b65a4*>(out) = xy;
    out->z = z;
}

bool fn_71006f4f28(UnkNavTarget_b65a4* self, UnkNavVec_b65a4* out) {
    UnkNavTable_b65a4* table = self->table;
    UnkNavHead_b65a4* head = table->head;
    if (!head)
        return false;
    UnkNavRecord_b65a4* record = head->record;
    if (!record)
        return false;
    if (record->kind == 1 && table->kind1) {
        CopyVec(out, table->kind1 + 0x3810c);
        return true;
    }
    if (record->kind == 7 && table->kind7) {
        CopyVec(out, table->kind7 + 0x3811c);
        return true;
    }
    if (record->kind == 2 && table->kind2) {
        UnkNavVec_b65a4 v = {table->kind2->x, table->kind2->y, 0.0f};
        *out = v;
        return true;
    }
    return false;
}
