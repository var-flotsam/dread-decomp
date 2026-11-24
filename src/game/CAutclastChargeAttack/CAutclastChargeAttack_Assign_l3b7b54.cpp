// CAutclastChargeAttack (tools/tu_map.py unit 555): its copy assignment (fn_71003b7b54). It copies the plain
// words, shares the listener's weak handle and copies the charge entries at +0x90 unless assigned to itself.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/math/CVector.h"
#include "game/UnkChargeEntry_ovb0016.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::math::CVector3D;

using UnkChargeEntries_ovb0016 = base::global::CRntVector<UnkChargeEntry_ovb0016>;

void CAutclastChargeAttack_ReserveChargeEntries(UnkChargeEntries_ovb0016* entries, u32 n);
void CAutclastChargeAttack_PushBackEntry(UnkChargeEntries_ovb0016* entries, const UnkChargeEntry_ovb0016& entry);

struct UnkWords32_l3b7b54 {
    u64 v[4];
};

struct UnkWords12_l3b7b54 {
    u32 v[3];
};

struct UnkWords16_l3b7b54 {
    u64 v[2];
};

struct UnkHalves22_l3b7b54 {
    u16 v[11];
};

struct UnkHalves6_l3b7b54 {
    u16 v[3];
};

struct UnkAutclastChargeAttack_l3b7b54 {
    void** vtable;
    UnkWords32_l3b7b54 field_0x8;
    UnkWords12_l3b7b54 field_0x28;
    void** listener_vtable;  // 0x38
    UnkWords16_l3b7b54 field_0x40;
    UnkWeakHandle* listener_ref;  // 0x50
    UnkHalves22_l3b7b54 field_0x58;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    UnkHalves6_l3b7b54 field_0x80;
    u32 field_0x88;
    u32 field_0x8c;
    UnkChargeEntries_ovb0016 entries;  // 0x90
};
static_assert(__builtin_offsetof(UnkAutclastChargeAttack_l3b7b54, field_0x58) == 0x58, "");
static_assert(__builtin_offsetof(UnkAutclastChargeAttack_l3b7b54, entries) == 0x90, "");

UnkAutclastChargeAttack_l3b7b54* fn_71003b7b54(UnkAutclastChargeAttack_l3b7b54* self,
                                               const UnkAutclastChargeAttack_l3b7b54* other) {
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = other->field_0x28;
    self->field_0x40 = other->field_0x40;
    if (self != other)
        CopyWeakRef(&self->listener_ref, other->listener_ref);
    self->field_0x58 = other->field_0x58;
    self->field_0x70 = other->field_0x70;
    self->field_0x74 = other->field_0x74;
    self->field_0x78 = other->field_0x78;
    self->field_0x7c = other->field_0x7c;
    self->field_0x80 = other->field_0x80;
    self->field_0x88 = other->field_0x88;
    self->field_0x8c = other->field_0x8c;
    if (self != other) {
        self->entries.Clear();
        CAutclastChargeAttack_ReserveChargeEntries(&self->entries, other->entries.capacity);
        if (other->entries.count != 0) {
            const UnkChargeEntry_ovb0016* end = other->entries.data + other->entries.count;
            for (const UnkChargeEntry_ovb0016* p = other->entries.data; p != end; ++p)
                CAutclastChargeAttack_PushBackEntry(&self->entries, *p);
        }
    }
    return self;
}
