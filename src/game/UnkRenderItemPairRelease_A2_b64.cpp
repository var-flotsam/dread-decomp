// Unit 441 (tools/tu_map.py, 0x710035d4e0, no class hint): two identical releases of a pair of render
// items. When both are set, a unit-wide count of live pairs drops by one and each item is handed back
// to the render manager's pool (fn_71002199d8, layer 0) and cleared, the second first. Names and
// layouts are placeholders.
#include "game/UnkRenderManager.h"
#include "types.h"

extern u32 g_unk_71726b21d4 __attribute__((visibility("hidden")));  // live pairs

struct UnkRenderItemPair_b64a2 {
    UnkItemBase_71011a91c4* first;
    UnkItemBase_71011a91c4* second;
};

static inline __attribute__((always_inline)) void ReleasePair(UnkRenderItemPair_b64a2* self) {
    if (self->second && self->first) {
        --g_unk_71726b21d4;
        fn_71002199d8(g_unk_7101d47300, self->second, 0);
        self->second = nullptr;
        if (self->first) {
            fn_71002199d8(g_unk_7101d47300, self->first, 0);
            self->first = nullptr;
        }
    }
}

void fn_710035d4ec(UnkRenderItemPair_b64a2* self) { ReleasePair(self); }
void fn_710035d558(UnkRenderItemPair_b64a2* self) { ReleasePair(self); }
