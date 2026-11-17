// CChozoCommanderSentenceSphereLifeComponent's unit (tools/tu_map.py unit 672): after the base's handler
// (fn_71008a4090), when the hit's amount (+0xf8) is positive, shows the owner's sphere part scaled by the
// ratio at +0x5dc / +0x5e0 and keeps the direction from the hit point to the owner at +0x27c.
// Names and layouts are placeholders.
#include "game/CFXComponent.h"
#include "game/UnkActor.h"
#include "game/UnkTU134.h"
#include "game/UnkTU285.h"
#include "game/UnkXformNode.h"
#include "types.h"

template <u32 Base, u32 FlagOffset>
struct UnkRange275;

void fn_7100208d1c(UnkRange275<0x118, 0x18>* self, f32 value, f32 limit, f32 fallback, bool refresh, bool flag);

struct UnkSphereLife_l45b078;
struct UnkSphereHit_l45b078;

u32 fn_71008a4090(UnkSphereLife_l45b078* self, UnkSphereHit_l45b078* hit);

struct UnkSphereData_l45b078 {
    u8 pad_0x0[0x5dc];
    f32 size;      // 0x5dc
    f32 max_size;  // 0x5e0
};

UnkSphereData_l45b078* fn_710043abd8(UnkActor* owner);

struct UnkSpherePart_l45b078 {
    u8 pad_0x0[0xb];
    u8 kind;  // 0xb
    u8 pad_0xc[0x28 - 0xc];
    UnkXformNode node;  // 0x28
};

struct UnkSphereHit_l45b078 {
    u8 pad_0x0[0x6c];
    f32 x;  // 0x6c
    f32 y;  // 0x70
    f32 z;  // 0x74
    u8 pad_0x78[0xf8 - 0x78];
    f32 amount;  // 0xf8
};

struct UnkSphereLife_l45b078 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x27c - 0x38];
    f32 dir_x;  // 0x27c
    f32 dir_y;  // 0x280
    f32 dir_z;  // 0x284
};

struct UnkDir_l45b078 {
    f32 x;
    f32 y;
    f32 z;
};

static inline UnkDir_l45b078 operator-(const UnkDir_l45b078& a, const UnkDir_l45b078& b) {
    return {a.x - b.x, a.y - b.y, a.z - b.z};
}

static inline UnkDir_l45b078 Pos(const UnkActor* actor) {
    return {actor->field_0x44, actor->field_0x54, actor->field_0x64};
}

static inline UnkDir_l45b078 Pos(const UnkSphereHit_l45b078* hit) { return {hit->x, hit->y, hit->z}; }

// The part with this hash, or null when the world does not show it.
static inline UnkSpherePart_l45b078* LivePart(UnkWorld* world, u64 hash) {
    auto* part = reinterpret_cast<UnkSpherePart_l45b078*>(fn_7101152b90(world, hash));
    if (!part)
        return nullptr;
    return fn_710114e668(reinterpret_cast<UnkObject*>(world), part->kind) ? part : nullptr;
}

// NON_MATCHING(minor): the same instructions in another order or with other registers; passes tools/equiv_test.py.
u32 fn_710045b078(UnkSphereLife_l45b078* self, UnkSphereHit_l45b078* hit) {
    u32 result = fn_71008a4090(self, hit);
    if (hit->amount > 0.0f) {
        UnkSpherePart_l45b078* part = LivePart(static_cast<UnkWorld*>(self->owner->field_0x280), 0x05311d74e6491d37ull);
        UnkSphereData_l45b078* data = fn_710043abd8(self->owner);
        f32 scale = data->size / data->max_size;
        UnkXformNode* node = &part->node;
        UnkVec3f v(scale, scale, scale);
        fn_7100208fa4(node, &v);
        fn_7100146dac(reinterpret_cast<UnkSwitchObject*>(part), true);
        fn_7100208a40(node);
        fn_7100208d1c(reinterpret_cast<UnkRange275<0x118, 0x18>*>(node), 0.12f, 1.0f, 1.0f, true, true);
        UnkDir_l45b078 d = Pos(self->owner) - Pos(hit);
        f32 inv = 1.0f / (__builtin_sqrtf(d.x * d.x + d.y * d.y + d.z * d.z) + 0.0001f);
        self->dir_x = d.x * inv;
        self->dir_y = d.y * inv;
        self->dir_z = d.z * inv;
    }
    return result;
}
