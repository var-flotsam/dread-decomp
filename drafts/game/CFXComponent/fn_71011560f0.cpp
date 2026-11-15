// CFXComponent's unit: message handler that tints the effect named by the message's id: the four colour
// values (the first three scaled by the factor at +0x38, clamped to non-negative) go to the effect's
// vf_0xe8 with the message's parameter hash, or to vf_0xe0 when the message names no parameter.
// Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkFXName_l1560f0 {
    u8 pad_0x0[0x30];
    u64 hash;  // 0x30
};

struct UnkFXEffect_l1560f0 {
    void** vtable;
};

struct UnkFXEntry_l1560f0 {
    UnkFXEffect_l1560f0* effect;
    u8 pad_0x8[0x18];
};

struct UnkFXEffects_l1560f0 {
    UnkFXEntry_l1560f0* values;  // 0x50
    UnkFXName_l1560f0** names;   // 0x58
    u32 count;                   // 0x60

    UnkFXEntry_l1560f0* End() { return &values[count]; }
    UnkFXEntry_l1560f0* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i]->hash == hash)
                return &values[i];
        return End();
    }
    UnkFXEntry_l1560f0* Get(u64 hash) {
        UnkFXEntry_l1560f0* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXComponent_l1560f0 {
    u8 pad_0x0[0x50];
    UnkFXEffects_l1560f0 effects;  // 0x50
};

struct UnkFXMessage_l1560f0 {
    u8 pad_0x0[0x10];
    UnkStringData* id;      // 0x10
    UnkStringData* param;   // 0x18
    u32 field_0x20;
    u32 field_0x24;
    f32 r, g, b, a;         // 0x28
    f32 scale;              // 0x38
};

template <u32 Off, class R = void, class... A>
static inline R VCall_l1560f0(void* obj, A... a) {
    return reinterpret_cast<R (*)(void*, A...)>((*reinterpret_cast<void***>(obj))[Off / 8])(obj, a...);
}

static inline f32 NonNegative(f32 v) { return v <= 0.0f ? 0.0f : v; }

void fn_71011560f0(UnkFXComponent_l1560f0* self, UnkFXMessage_l1560f0* msg) {
    if (msg->id == &g_unk_7101d47108->empty)
        return;
    UnkFXEntry_l1560f0* entry = self->effects.Get(reinterpret_cast<UnkFXName_l1560f0*>(msg->id)->hash);
    UnkFXEffect_l1560f0* effect = entry ? entry->effect : nullptr;
    if (!effect)
        return;
    f32 r = msg->r, g = msg->g, b = msg->b, a = msg->a, k = msg->scale;
    f32 values[4] = {NonNegative(r * k), NonNegative(k * g), NonNegative(k * b), NonNegative(a)};
    if (msg->param == &g_unk_7101d47108->empty)
        VCall_l1560f0<0xe0>(effect, msg->field_0x20, values, msg->field_0x24);
    else
        VCall_l1560f0<0xe8>(effect, reinterpret_cast<UnkFXName_l1560f0*>(msg->param)->hash, msg->field_0x20, values,
                            msg->field_0x24);
}
