// Unit 2730 (tools/tu_map.py; no reflected class): fn_7101197cbc calls every callback in the list at
// +0x30 (count +0x38; each a context and a function) with the value at +0x58 and 3, then hands the render
// manager to fn_710021a114 (in its unit). Names and layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

struct UnkCallback_b49 {
    void* context;
    void (*fn)(void* context, u64 value, u32 kind);
};

struct UnkCallbackOwner_b49 {
    u8 pad_0x0[0x30];
    UnkCallback_b49* callbacks;
    u32 count;
    u8 pad_0x3c[0x1c];
    u64 value;  // 0x58
};

void fn_710021a114(UnkRenderManager* manager);

void fn_7101197cbc(UnkCallbackOwner_b49* self) {
    if (u32 n = self->count) {
        UnkCallback_b49* it = self->callbacks;
        UnkCallback_b49* end = it + n;
        u64 value = self->value;
        do {
            it->fn(it->context, value, 3);
            ++it;
        } while (it != end);
    }
    fn_710021a114(g_unk_7101d47300);
}
