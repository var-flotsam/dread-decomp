// A music trigger's destructors in unit 2595 of tools/tu_map.py (no class name known): three parts
// (each its vtable, four cleared word pairs and its reflected value), a list header (cleared), then the
// music trigger base's destructor (fn_71011867d4). Names and layouts are placeholders.
#include "base/reflection/UnkReflCast.h"
#include "types.h"

extern char g_unk_7101caa6d8[];  // the trigger's vtable
extern char g_unk_7101caa7e8[];  // the parts'
extern char g_unk_7101caa840[];  // the list header's

void fn_71011867d4(void* self);  // the music trigger base's destructor

struct UnkTrigger2595Base_b44a3 {
    char* vtable;
    u8 pad_0x8[0x50];
    ~UnkTrigger2595Base_b44a3() { fn_71011867d4(this); }
};

struct UnkTrigger2595List_b44a3 {
    char* vtable;
    u8 pad_0x8[0x8];
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;
    u8 pad_0x24[0x8c];

    ~UnkTrigger2595List_b44a3() {
        vtable = g_unk_7101caa840 + 0x10;
        field_0x20 = 0;
        field_0x10 = 0;
        field_0x18 = 0;
    }
};

struct UnkTrigger2595Pair_b44a3 {
    u64 first;
    u64 second;
    ~UnkTrigger2595Pair_b44a3() {
        first = 0;
        second = 0;
    }
};

struct UnkTrigger2595ValueHolder_b44a3 {
    base::reflection::UnkReflValue value;
    ~UnkTrigger2595ValueHolder_b44a3() { fn_71000a1c60(&value); }
};

// The part base: its vtable and reflected value.
struct UnkTrigger2595PartBase_b44a3 {
    char* vtable;
    u8 pad_0x8[0x10];
    UnkTrigger2595ValueHolder_b44a3 value;  // 0x18

    ~UnkTrigger2595PartBase_b44a3() { vtable = g_unk_7101caa7e8 + 0x10; }
};

// A part: four word pairs on top of the base, cleared when destroyed (before the base's vtable
// is restored).
struct UnkTrigger2595Part_b44a3 : UnkTrigger2595PartBase_b44a3 {
    UnkTrigger2595Pair_b44a3 pair_0x30;
    UnkTrigger2595Pair_b44a3 pair_0x40;
    UnkTrigger2595Pair_b44a3 pair_0x50;
    UnkTrigger2595Pair_b44a3 pair_0x60;
};

struct UnkTrigger2595_b44a3 : UnkTrigger2595Base_b44a3 {
    UnkTrigger2595List_b44a3 list;      // 0x58
    UnkTrigger2595Part_b44a3 parts[3];  // 0x108

    ~UnkTrigger2595_b44a3() { vtable = g_unk_7101caa6d8 + 0x10; }
};
static_assert(sizeof(UnkTrigger2595_b44a3) == 0x258, "UnkTrigger2595_b44a3 placeholder layout");

void fn_7100f13418(UnkTrigger2595_b44a3* self) {
    self->~UnkTrigger2595_b44a3();
}

void fn_7100f13518(UnkTrigger2595_b44a3* self) {
    self->~UnkTrigger2595_b44a3();
    __builtin_operator_delete(self);
}
