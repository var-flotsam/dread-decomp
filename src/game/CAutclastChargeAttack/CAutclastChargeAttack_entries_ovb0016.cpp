// CAutclastChargeAttack (tools/tu_map.py): the attack's list of charge entries at +0x90 (base/UnkVector.h's
// layout; 0x28-byte entries, a position, five words and a counted reference at +0x20): growing it
// (fn_71003b78f8, copying each entry over and destroying the old one), appending a copy (fn_71003b7a64), and
// the attack's destructor (fn_71003b457c: the list, the listener at +0x38, then CAttack's destructor).
// Names and layouts are placeholders.
#include <new>

#include "../../game_5821e4/CAttackDtor.h"
#include "base/UnkVector.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a2ae00[];  // the attack's vtable
extern void* g_unk_7101a2aef8[];  // the listener's vtable
void fn_71010cb2fc(void*, void*);

struct UnkChargePos_ovb0016 {
    f32 x, y, z;
};

struct UnkChargeEntry_ovb0016 {
    UnkChargePos_ovb0016 pos;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    UnkRefObject* ref;

    UnkChargeEntry_ovb0016(const UnkChargeEntry_ovb0016& o)
        : pos(o.pos), field_0xc(o.field_0xc), field_0x10(o.field_0x10), field_0x14(o.field_0x14), field_0x18(o.field_0x18),
          field_0x1c(o.field_0x1c), ref(nullptr) {
        AssignRef(&ref, o.ref);
    }
    ~UnkChargeEntry_ovb0016() {
        if (UnkRefObject* p = ref) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

using UnkChargeEntries_ovb0016 = base::UnkVector<UnkChargeEntry_ovb0016>;

void fn_71003b78f8(UnkChargeEntries_ovb0016* entries, u32 n) {
    if (entries->capacity >= n)
        return;
    auto* p = static_cast<UnkChargeEntry_ovb0016*>(
        entries->allocator->vf_0x40((u64)n * sizeof(UnkChargeEntry_ovb0016), 8, entries->flags));
    if (UnkChargeEntry_ovb0016* src = entries->data) {
        UnkChargeEntry_ovb0016* dst = p;
        for (u32 i = 0; i < entries->count; ++i, ++dst, ++src) {
            new (dst) UnkChargeEntry_ovb0016(*src);
            src->~UnkChargeEntry_ovb0016();
        }
        entries->allocator->vf_0x48(entries->data);
    }
    entries->data = p;
    entries->capacity = n;
}

void fn_71003b7a64(UnkChargeEntries_ovb0016* entries, const UnkChargeEntry_ovb0016& entry) {
    u32 n = entries->count;
    u32 capacity = entries->capacity;
    if (capacity <= n && entries->step != 0)
        capacity = (n / entries->step + 1) * entries->step;
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    fn_71003b78f8(entries, capacity);
    new (&entries->data[entries->count]) UnkChargeEntry_ovb0016(entry);
    ++entries->count;
}

struct UnkAutclastChargeAttack_ovb0016 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkAttackListenerRaw listener;  // 0x38
    u8 pad_0x58[0x38];
    UnkChargeEntries_ovb0016 entries;  // 0x90
};

void fn_71003b457c(UnkAutclastChargeAttack_ovb0016* self) {
    self->vtable = g_unk_7101a2ae00 + 2;
    self->entries.Free();
    self->listener.Destroy(g_unk_7101a2aef8, fn_71010cb2fc);
    fn_71003606c0(self);
}
