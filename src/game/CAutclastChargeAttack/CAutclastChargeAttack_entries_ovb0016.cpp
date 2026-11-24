// CAutclastChargeAttack (tools/tu_map.py): the attack's list of charge entries at +0x90 (base/global/CRntVector.h's
// layout; 0x28-byte entries, a position, five words and a counted reference at +0x20): growing it
// (fn_71003b78f8, copying each entry over and destroying the old one), appending a copy (fn_71003b7a64), and
// the attack's destructor (fn_71003b457c: the list, the listener at +0x38, then CAttack's destructor).
// Names and layouts are placeholders.
#include <new>

#include "../../game_5821e4/CAttackDtor.h"
#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/CAutclastChargeAttack.h"
#include "game/CEnemyMovement.h"
#include "game/UnkChargeEntry_ovb0016.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

using UnkChargeEntries_ovb0016 = base::global::CRntVector<UnkChargeEntry_ovb0016>;

void CAutclastChargeAttack_ReserveChargeEntries(UnkChargeEntries_ovb0016* entries, u32 n) {
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

void CAutclastChargeAttack_PushBackEntry(UnkChargeEntries_ovb0016* entries, const UnkChargeEntry_ovb0016& entry) {
    u32 n = entries->count;
    u32 capacity = entries->capacity;
    if (capacity <= n && entries->step != 0)
        capacity = entries->GrownCapacity(n);
    if ((s32)n >= (s32)capacity)
        for (;;) {
        }
    CAutclastChargeAttack_ReserveChargeEntries(entries, capacity);
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
