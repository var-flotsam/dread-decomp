// Unit 176 (no reliable name): the unit's static initialiser. It builds the header ids, the two memory
// pools, a table of 66 cleared slots and the two handle sets of each kind. Names and layouts are placeholders.
#include <new>
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "game/UnkTU170.h"
#include "types.h"

using base::global::CStrId;

struct UnkPool {
    u8 pad_0x0[0x120];
};

struct UnkHandle {
    u8 pad_0x0[0x8];
    void* field_0x8;
};

// Up to 16 handles (stored 4-byte aligned) and an owner.
typedef UnkHandle* UnkHandlePtr __attribute__((aligned(4)));
struct UnkHandleSet {
    u32 count;
    UnkHandlePtr items[16];
    UnkHandle* owner;

    UnkHandleSet() {}
    UnkHandleSet(UnkHandle* const& item, UnkHandle* owner_) : count(1), owner(owner_) { items[0] = item; }
    // Keeps the non-null handles of a list.
    UnkHandleSet(UnkHandle* const* list, u32 n, UnkHandle* owner_) : count(0), owner(owner_) {
        for (u32 i = 0; i < n; i++) {
            if (list[i])
                items[count++] = list[i];
        }
    }
    UnkHandleSet& operator=(const UnkHandleSet& other) {
        __builtin_memcpy(items, other.items, (u32)(other.count * sizeof(UnkHandlePtr)));
        count = other.count;
        owner = other.owner;
        return *this;
    }
};
static_assert(sizeof(UnkHandleSet) == 0x90, "");

// The unit's array destructors (pools, then the two handle-set arrays).
__attribute__((visibility("hidden"))) void fn_71001580c0(void*);
__attribute__((visibility("hidden"))) void fn_71001580f0(void*);
__attribute__((visibility("hidden"))) void fn_7100158110(void*);

extern UnkPool g_unk_716e29bcd0[];
extern UnkHandleSet g_unk_716e29bf30[2];
extern UnkHandleSet g_unk_716e29c050[2];

// A cleared slot: three pointers and two flags, 4-byte aligned.
#pragma pack(push, 4)
struct UnkSlot176 {
    void* field_0x0;
    void* field_0x8;
    void* field_0x10;
    bool field_0x18;
    bool field_0x19;
};
#pragma pack(pop)
static_assert(sizeof(UnkSlot176) == 0x1c, "UnkSlot176 layout");

// The unit's statics, laid out (4-byte aligned) as the compiler placed them.
#pragma pack(push, 4)
struct UnkUnit176Statics {
    u32 header;
    u8 pad_0x4[8];
    CStrId binaries;  // 0xc
    CStrId sources;   // 0x14
    u8 pad_0x1c[0x10];
    UnkSlot176 slots[66];  // 0x2c
};
#pragma pack(pop)
static_assert(__builtin_offsetof(UnkUnit176Statics, slots) == 0x2c, "UnkUnit176Statics layout");
extern UnkUnit176Statics g_unk_716e29c6d4 __attribute__((visibility("hidden")));

// NON_MATCHING(functional): Operations differ from the original; behaves the same on every input tools/equiv_test.py --any-kind tried, with full coverage. Owes a match.
void fn_7100158130() {
    UnkUnit176Statics& s = g_unk_716e29c6d4;
    s.header = 6;
    new (&s.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &s.binaries, g_unk_7101a19000);
    new (&s.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &s.sources, g_unk_7101a19000);
    fn_71001537c0(&g_unk_716e29bcd0[0]);
    fn_71001537c0(&g_unk_716e29bcd0[1]);
    fn_7100000250(fn_71001580c0, nullptr, g_unk_7101a19000);
    // Cleared one by one in the original, not in a loop.
#pragma clang loop unroll(full)
    for (s32 i = 0; i < 66; i++) {
        UnkSlot176& slot = s.slots[i];
        slot.field_0x0 = nullptr;
        slot.field_0x8 = nullptr;
        slot.field_0x10 = nullptr;
        slot.field_0x18 = false;
        slot.field_0x19 = true;
    }
    for (s32 i = 0; i < 2; i++) {
        g_unk_716e29bf30[i].count = 0;
        g_unk_716e29bf30[i].owner = nullptr;
    }
    fn_7100000250(fn_71001580f0, nullptr, g_unk_7101a19000);
    for (s32 i = 0; i < 2; i++) {
        g_unk_716e29c050[i].count = 0;
        g_unk_716e29c050[i].owner = nullptr;
    }
    fn_7100000250(fn_7100158110, nullptr, g_unk_7101a19000);
}
