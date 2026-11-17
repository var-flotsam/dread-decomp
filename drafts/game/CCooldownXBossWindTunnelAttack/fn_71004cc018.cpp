// CCooldownXBossWindTunnelAttack's unit: each frame, sets the wind strength the player feels from the
// distance between the player and the boss along the tunnel, in bands (faded over 100 units at each edge).
// Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

void fn_7100f0e4dc(void* target, bool on);
void fn_7100f0e478(void* target, f32 strength);

extern f32 g_unk_7101a3eac8;  // the tunnel's length
extern f32 g_unk_7101a3eacc;
extern f32 g_unk_7101a3ead0;
extern f32 g_unk_7101a3ead4;
extern f32 g_unk_7101a3ead8;

struct UnkSuit_l4cc018 {
    u8 pad_0x0[0x3a8];
    bool field_0x3a8;
};

struct UnkTarget_l4cc018 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x1f8 - 0x68];
    UnkSuit_l4cc018* field_0x1f8;
    u8 pad_0x200[0x10];
    void* field_0x210;
    u8 pad_0x218[0x8];
    UnkAnimationComponent* field_0x220;
};

struct UnkWindTunnel_l4cc018 {
    u8 pad_0x0[0x8];
    UnkActor* owner;
    u8 pad_0x10[0x60];
    s32 field_0x70;
    u8 pad_0x74[0x4];
    bool field_0x78;
    u8 pad_0x79[0xb];
    bool field_0x84;
    u8 pad_0x85[0x3];
    f32 field_0x88;
};

struct UnkVec_l4cc018 {
    f32 x;
    f32 y;
    f32 z;
};

static inline UnkVec_l4cc018 Add(const UnkVec_l4cc018& a, const UnkVec_l4cc018& b) {
    UnkVec_l4cc018 r;
    r.x = a.x + b.x;
    r.y = a.y + b.y;
    r.z = a.z + b.z;
    return r;
}

static inline UnkVec_l4cc018 Sub(const UnkVec_l4cc018& a, const UnkVec_l4cc018& b) {
    UnkVec_l4cc018 r;
    r.x = a.x - b.x;
    r.y = a.y - b.y;
    r.z = a.z - b.z;
    return r;
}

static inline f32 Dot(const UnkVec_l4cc018& a, const UnkVec_l4cc018& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static inline UnkVec_l4cc018 Scale(const UnkVec_l4cc018& a, f32 s) {
    UnkVec_l4cc018 r;
    r.x = a.x * s;
    r.y = a.y * s;
    r.z = a.z * s;
    return r;
}

static inline f32 Clamp(f32 v, f32 lo, f32 hi) {
    return v > lo ? (v >= hi ? hi : v) : lo;
}

// 1 at `lo`, falling to 0 at `hi`.
static inline f32 FadeOut(f32 v, f32 lo, f32 hi) {
    return 1.0f - Clamp((Clamp(v, lo, hi) - lo) / (hi - lo), 0.0f, 1.0f);
}

// 1 at the edge, falling to 0 100 units past it.
static inline f32 FadeOutEdge(f32 past) {
    return 1.0f - Clamp(Clamp(past, 0.0f, 100.0f) / 100.0f, 0.0f, 1.0f);
}

static inline f32 Lerp(f32 a, f32 b, f32 t) {
    return a + (b - a) * t;
}

void fn_71004cc018(UnkWindTunnel_l4cc018* self, f32 dt) {
    fn_7100f0e4dc(g_unk_7101d47310->field_0x38, false);
    if (self->field_0x84)
        self->field_0x88 += dt;
    if (self->field_0x78)
        return;
    UnkTarget_l4cc018* target = reinterpret_cast<UnkTarget_l4cc018*>(fn_7100d7c990(self->owner));
    if (!target)
        return;

    f32 outer;
    f32 inner;
    f32 low;
    f32 lowest;
    if (!target->field_0x210 && target->field_0x1f8) {
        bool upgraded = target->field_0x1f8->field_0x3a8;
        outer = upgraded ? 1850.0f : 1460.0f;
        inner = upgraded ? 1587.6001f : 1360.8f;
        low = upgraded ? 960.0f : 576.0f;
        lowest = upgraded ? 300.0f : 180.0f;
    } else {
        lowest = 300.0f;
        low = 960.0f;
        inner = 1587.6001f;
        outer = 1850.0f;
    }
    f32 high = self->field_0x70 == 0 ? 3000.0f : outer;
    f32 strength = self->field_0x70 == 0 ? 3000.0f : inner;

    bool ignoring = target->field_0x220 && fn_710117b348(target->field_0x220, fn_7100c2d5d8());
    bool held = target->field_0x220 && fn_710117ae38(target->field_0x220, 0xc906c3bd30e0562f, false);

    UnkActor* owner = self->owner;
    UnkVec_l4cc018 ownerPos = {owner->field_0x44, owner->field_0x54, owner->field_0x64};
    UnkVec_l4cc018 offset = {g_unk_7101a3eac8 * 0.5f, 0.0f, 0.0f};
    UnkVec_l4cc018 targetPos = {target->x, target->y, target->z};
    UnkVec_l4cc018 delta = Sub(targetPos, Add(ownerPos, offset));
    UnkVec_l4cc018 axis = {-1.0f, 0.0f, 0.0f};
    UnkVec_l4cc018 along = Scale(axis, Dot(delta, axis));
    f32 distance = __builtin_sqrtf(Dot(along, along));

    if (ignoring) {
        strength = 0.0f;
        if (held)
            fn_7100f0e4dc(g_unk_7101d47310->field_0x38, true);
    } else if (distance <= g_unk_7101a3ead8) {
        strength = 3000.0f;
    } else if (distance <= g_unk_7101a3ead0) {
        if (distance <= g_unk_7101a3ead8 + 100.0f)
            strength = Lerp(high, 3000.0f, FadeOutEdge(distance - g_unk_7101a3ead8));
        else
            strength = Lerp(strength, high, FadeOut(distance, g_unk_7101a3ead4, g_unk_7101a3ead0));
    } else if (!fn_7100df5874(reinterpret_cast<UnkPlayer*>(g_unk_7101d47310))) {
        if (distance <= g_unk_7101a3ead0 + 100.0f)
            strength = Lerp(low, strength, FadeOutEdge(distance - g_unk_7101a3ead0));
        else
            strength = Lerp(lowest, low, FadeOut(distance, g_unk_7101a3eacc, g_unk_7101a3eac8));
    }
    fn_7100f0e478(g_unk_7101d47310->field_0x38, -strength);
}
