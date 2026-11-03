// behaviortree::CBigFistChangeToCombatAction unit (tools/tu_map.py): on an event of kind 1 whose
// action id (+0x8 -> +0x10 -> +0x38) is one of five ids of the table g_unk_71726c09a8 (entries 0,
// 2, 3, 4, 5), sets the flag at +0x60. The result is only set on that path, as in the original
// (its other paths return whatever x0 holds). Names and layouts are placeholders.
#include "types.h"

extern u64 g_unk_71726c09a8[6] __attribute__((visibility("hidden")));

struct UnkBigFistActionDef {
    u8 pad_0x0[0x38];
    u64 id;  // 0x38
};

struct UnkBigFistActionRef {
    u8 pad_0x0[0x10];
    UnkBigFistActionDef* def;  // 0x10
};

struct UnkBigFistEvent {
    u32 kind;
    UnkBigFistActionRef* action;  // 0x8
};

struct UnkBigFistChangeToCombatAction {
    u8 pad_0x0[0x60];
    bool triggered;  // 0x60
};

bool fn_71003e35a0(UnkBigFistChangeToCombatAction* self, const UnkBigFistEvent* ev) {
    bool result;
    if (ev->kind == 1) {
        u64 id = ev->action->def->id;
        if (id == g_unk_71726c09a8[0] || id == g_unk_71726c09a8[2] || id == g_unk_71726c09a8[3] ||
            id == g_unk_71726c09a8[4] || id == g_unk_71726c09a8[5]) {
            self->triggered = true;
            result = true;
        }
    }
    return result;
}
