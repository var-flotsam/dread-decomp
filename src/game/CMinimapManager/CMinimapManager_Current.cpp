// CMinimapManager unit: unless any of the first (+0xd64428) words of the table at +0xd64420 is
// zero (or, for the second, one), passes on (fn_7100b42f58, fn_7100b42fa4) the object at +0x10 of
// the entry at +0xd63f40, or null when there is no entry. Names and layouts are placeholders.
#include "types.h"

struct UnkMinimapEntry {
    u8 pad_0x0[0x10];
    void* object;  // 0x10
};

struct UnkMinimapState {
    UnkMinimapEntry* entry;  // 0xd63f40
    u8 pad_0x8[0x4d8];
    const u32* table;  // 0xd64420
    u32 count;  // 0xd64428
};

struct UnkMinimapManagerState {
    u8 pad_0x0[0xd63f40];
    UnkMinimapState state;
};

void fn_7100b42f58(void* object);
void fn_7100b42fa4(void* object);

// Unless one of the table's words equals Stop, passes the entry's object (or null) to Fn.
template <u32 Stop, void (*Fn)(void*)>
static inline __attribute__((always_inline)) void PassCurrent(UnkMinimapManagerState* self) {
    UnkMinimapState* state = &self->state;
    const u32* p = state->table;
    for (u64 n = state->count; n != 0; --n, ++p)
        if (*p == Stop)
            return;
    UnkMinimapEntry* entry = state->entry;
    Fn(entry ? entry->object : nullptr);
}

void fn_7100e9d654(UnkMinimapManagerState* self) { PassCurrent<0, fn_7100b42f58>(self); }
void fn_7100e9d6a4(UnkMinimapManagerState* self) { PassCurrent<1, fn_7100b42fa4>(self); }
