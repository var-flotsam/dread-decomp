// The render manager's unit (tools/tu_map.py unit 315; include/game/UnkRenderManager.h): fn_7100219a34
// sets the layer bits of a scene node (as fn_7100219ae0 does, src/game_lo/fn_7100219ae0.cpp) and hands
// the type-specific member and the previous layer to the pass list (fn_71001fd490), when the node
// is enabled; fn_710021a114 and fn_7100218b68 set pass parameters. Names and layouts are placeholders.
#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "types.h"

// Per-type payload; each node type has its own layout with the handed-over member at Offset.
template <int Type, int Offset>
struct UnkPayload_b0231 {
    u8 pad[Offset];
    u8 member[8];
};

template <int Type, int Offset>
void* NodeMember(const UnkSceneNode* node) {
    return static_cast<UnkPayload_b0231<Type, Offset>*>(node->field_0x10)->member;
}

bool fn_7100217e50(u32 flags, u32 mask);
void fn_71001fd490(void* list, void* obj, u32 layer);

bool fn_7100219a34(UnkRenderManager* self, UnkSceneNode* entry) {
    if (!fn_7100217e50(entry->bits & 0x3fff, 1))
        return false;
    u32 bits = entry->bits;
    u32 layer = (bits >> 12) & 0xf;
    entry->bits = bits | 0xf000;
    void* obj = nullptr;
    switch (entry->type_at_3()) {
#define C(t, o) case t: obj = NodeMember<t, o>(entry); break;
    C(3, 0x20) C(4, 0x20) C(5, 0x28) C(6, 0x28) C(8, 0x18) C(9, 0x18) C(10, 0x18) C(11, 0x18)
    C(12, 0x20) C(13, 0x20) C(14, 0x28) C(15, 0x20) C(16, 0x20) C(17, 0x20) C(18, 0x18)
    C(19, 0x20) C(21, 0x18) C(22, 0x18) C(23, 0x18) C(24, 0x28)
#undef C
    }
    fn_71001fd490(self->field_0x220, obj, layer);
    return true;
}

void fn_71001fc7bc(void* passes, u64 hash, const f32* value, u32 count);

// Zeroes the two pass parameters with these hashes (fn_710021a210, src/game/UnkRenderManager.cpp, inlined).
static inline void ZeroPair(UnkRenderManager* self) {
    f32 zero = 0.0f;
    fn_71001fc7bc(self->field_0x220, 0x38bb3d82ecfaffaa, &zero, 0);
    fn_71001fc7bc(self->field_0x220, 0xcc53b066013b1983, &zero, 0);
}

// Sets the pass parameters to their defaults: a half, a zero, two 2.5s and the zero pair.
void fn_710021a114(UnkRenderManager* self) {
    f32 half = 0.5f;
    fn_71001fc7bc(self->field_0x220, 0xa1fcaf230447d553, &half, 0);
    f32 zero = 0.0f;
    fn_71001fc7bc(self->field_0x220, 0xc66c8984dda950d6, &zero, 0);
    f32 two_and_half = 2.5f;
    fn_71001fc7bc(self->field_0x220, 0xfe815708d5f2b892, &two_and_half, 0);
    fn_71001fc7bc(self->field_0x220, 0x0a0b60deb29d8a1b, &two_and_half, 0);
    ZeroPair(self);
}

// The vectors the pass parameters take in each mode.
extern const base::math::CVector4D g_unk_7101647b1c __attribute__((visibility("hidden")));
extern const base::math::CVector4D g_unk_7101647b8c __attribute__((visibility("hidden")));
extern const base::math::CVector4D g_unk_7101647c0c __attribute__((visibility("hidden")));

struct UnkRenderManagerMode_b0231 {
    u8 pad_0x0[0x220];
    void* passes;  // 0x220
    u8 pad_0x228[0x38];
    s32 mode;  // 0x260
};

// Switches the mode (0, 1 or 2) and sets the two pass parameters it implies, each for the two kinds
// of use (0 and 3): zeros in mode 0, else the common vector and the mode's own.
void fn_7100218b68(UnkRenderManager* manager, s32 mode) {
    auto* self = reinterpret_cast<UnkRenderManagerMode_b0231*>(manager);
    if (self->mode == mode)
        return;
    self->mode = mode;
    base::math::CVector4D first;
    base::math::CVector4D second;
    switch (mode) {
    case 0:
        first = {};
        fn_71001fc7bc(self->passes, 0x44930a8a500dff97, &first.x, 0);
        fn_71001fc7bc(self->passes, 0x44930a8a500dff97, &first.x, 3);
        second = {};
        break;
    case 1:
        first = g_unk_7101647b1c;
        fn_71001fc7bc(self->passes, 0x44930a8a500dff97, &first.x, 0);
        fn_71001fc7bc(self->passes, 0x44930a8a500dff97, &first.x, 3);
        second = g_unk_7101647c0c;
        break;
    case 2:
        first = g_unk_7101647b1c;
        fn_71001fc7bc(self->passes, 0x44930a8a500dff97, &first.x, 0);
        fn_71001fc7bc(self->passes, 0x44930a8a500dff97, &first.x, 3);
        second = g_unk_7101647b8c;
        break;
    default:
        return;
    }
    fn_71001fc7bc(self->passes, 0x4561259075b4cc71, &second.x, 0);
    fn_71001fc7bc(self->passes, 0x4561259075b4cc71, &second.x, 3);
}
