// CAutclastChargeAttack: its copy constructor (fn_71003b7704), the implicit copy except that the list of charge
// entries at +0x90 is copied entry by entry. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CAttack.h"
#include "game/CAutclastChargeAttack.h"
#include "types.h"

// The entries are defined in CAutclastChargeAttack_entries_ovb0016.cpp (0x28 bytes each).
struct UnkChargeEntry_ovb0016;
using UnkChargeEntries_ovb0016 = base::global::CRntVector<UnkChargeEntry_ovb0016>;
void fn_71003b78f8(UnkChargeEntries_ovb0016* entries, u32 n);
void fn_71003b7a64(UnkChargeEntries_ovb0016* entries, const UnkChargeEntry_ovb0016& entry);

struct UnkChargeEntry_l3b7704 {
    u64 words[5];
};

struct UnkChargeList_l3b7704 {
    UnkChargeEntry_l3b7704* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkChargeList_l3b7704(const UnkChargeList_l3b7704& other)
        : data(nullptr), count(0), capacity(0), step(other.step), field_0x14(other.field_0x14), flags(other.flags),
          allocator(other.allocator) {
        auto* list = reinterpret_cast<UnkChargeEntries_ovb0016*>(this);
        fn_71003b78f8(list, other.capacity);
        for (u32 i = 0; i < other.count; ++i)
            fn_71003b7a64(list, *reinterpret_cast<const UnkChargeEntry_ovb0016*>(&other.data[i]));
    }
};

struct UnkChargeShort_l3b7704 {
    u32 field_0x0;
    u16 field_0x4;
};

struct UnkAutclastChargeAttack_l3b7704 : CAttack {
    ~UnkAutclastChargeAttack_l3b7704() override;
    u64 field_0x28;
    u32 field_0x30;
    UnkAttackListener_7101a2aef8 field_0x38;
    u64 field_0x58;
    u64 field_0x60;
    u64 field_0x68;
    u32 field_0x70;
    u32 field_0x74;
    u32 field_0x78;
    u32 field_0x7c;
    UnkChargeShort_l3b7704 field_0x80;
    u32 field_0x88;
    u32 field_0x8c;
    UnkChargeList_l3b7704 field_0x90;
};

void fn_71003b7704(UnkAutclastChargeAttack_l3b7704* self, const UnkAutclastChargeAttack_l3b7704* other) {
    new (self) UnkAutclastChargeAttack_l3b7704(*other);
}
