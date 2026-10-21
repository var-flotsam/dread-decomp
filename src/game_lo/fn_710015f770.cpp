// Sets or clears slot `index` (0-15) of group `group` in a global table. Setting stores the
// pointer and marks the slot; clearing a set slot nulls it and reports the change through a
// global callback with the group's id.
#include "types.h"

struct UnkSlot_0x78 {
    void* ptr;
    u8 field_0x8[0x68];
    bool marked;  // +0x70
};

// All reached through the GOT (named after their GOT slots).
extern UnkSlot_0x78 g_unk_7101cc5fd8[][16];
extern u32 g_unk_7101cc6020[];
extern void (*g_unk_7101cc5838)(void* context, u32 id, u32 index, void* arg);

void* fn_7100153d50();

void fn_710015f770(u32 index, void* ptr, u32 group) {
    if (index > 15)
        return;
    if (ptr) {
        g_unk_7101cc5fd8[group][index].ptr = ptr;
        g_unk_7101cc5fd8[group][index].marked = true;
        return;
    }
    if (g_unk_7101cc5fd8[group][index].ptr) {
        g_unk_7101cc5fd8[group][index].ptr = nullptr;
        void* context = fn_7100153d50();
        u32 id = g_unk_7101cc6020[group];
        g_unk_7101cc5838(context, id, index, nullptr);
    }
}
