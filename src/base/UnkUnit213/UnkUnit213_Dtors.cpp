// Unit 213 of tools/tu_map.py (0x710019545c-0x710019565c): the destructor and the deleting
// destructor of a class over fn_710019e82c's (vtable unk_7101a1dfe8): while active (+0x38) it
// returns its two pooled objects (+0x28, +0x30) and clears them and the word at +0x20. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "game/UnkTU174.h"
#include "game/UnkTU182.h"
#include "game/UnkTU226.h"
#include "types.h"

struct UnkObj_7100161b90;
struct UnkObj_71001541a0;
struct UnkStruct_710019e82c;

extern char g_unk_7101a1dfe8[];  // vtable (reached through the GOT)

// fn_710019e82c's class, the base.
struct UnkPassBase213 {
    char* vtable;
    u8 pad_0x8[0x18];

    __attribute__((always_inline)) ~UnkPassBase213() {
        fn_710019e82c(this);
    }
};

struct UnkPass213 : UnkPassBase213 {
    u64 field_0x20;
    UnkObj_7100161b90* field_0x28;
    UnkObj_71001541a0* field_0x30;
    bool active;  // 0x38

    __attribute__((always_inline)) ~UnkPass213() {
        vtable = g_unk_7101a1dfe8 + 0x10;
        if (active) {
            active = false;
            fn_7100161b90(field_0x28);
            fn_71001541a0(field_0x30);
            field_0x28 = nullptr;
            field_0x30 = nullptr;
            field_0x20 = 0;
        }
    }
};
static_assert(__builtin_offsetof(UnkPass213, active) == 0x38, "UnkPass213 layout");

void fn_7100195498(UnkPass213* self) { self->~UnkPass213(); }

void fn_71001954ec(UnkPass213* self) {
    self->~UnkPass213();
    fn_7100083c00(self);
}
