// Unit 1518 (no reliable name): a placer that puts its model at a position from its virtual accessor
// (fn_7100a0eb24), and one that tints the slots of an owner and writes a position/alpha vector into its
// target block (fn_7100a0ebf4). Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector4D.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU315.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkMatrix44_7100078c9c {
    f32 m[16];
};
UnkMatrix44_7100078c9c fn_7100078cd8(const UnkVec3f& t);

// Returned through x8, so the original's result type is not trivially copyable.
struct UnkVec2Result_b0211 {
    f32 x, y;
    UnkVec2Result_b0211() {}
    UnkVec2Result_b0211(const UnkVec2Result_b0211& o) : x(o.x), y(o.y) {}
};

struct UnkModelPlacer_b0211 {
    virtual UnkVec2Result_b0211 vf_0x0();
    u8 pad_0x8[0x8];
    u64 instance;  // 0x10
    u64 model;     // 0x18
};

void fn_7100a0eb24(UnkModelPlacer_b0211* self) {
    UnkRenderManager* manager = g_unk_7101d47300;
    u64 model = self->model;
    UnkVec2Result_b0211 pos = self->vf_0x0();
    UnkMatrix44_7100078c9c m = fn_7100078cd8(UnkVec3f(pos.x, pos.y, 0.0f));
    fn_7100219dd0(manager, model, reinterpret_cast<const UnkMatrix44*>(&m), 1, 1, &self->instance, 0);
}

using base::math::CVector4D;


struct UnkStruct_71001436f4;
bool fn_71001436f4(UnkStruct_71001436f4* self, u32 a, const base::math::CVector4D* b, u32 c);

struct UnkTargetNode_b0211 {
    u8 pad_0x0[0x30];
    CVector4D* value;  // 0x30
};

struct UnkTargetBlock_b0211 {
    u8 pad_0x0[0x15c];
    CVector4D color;  // 0x15c
    u8 pad_0x16c[0x8c];
    UnkTargetNode_b0211* node;  // 0x1f8
};

struct UnkTargetSlots_b0211 {
    u8 pad_0x0[0x78];
    struct {
        u8 pad_0x0[0x48];
        UnkTargetBlock_b0211** block;  // 0x48
    }* table;  // 0x78
};

struct UnkTargetOwner_b0211 {
    u8 pad_0x0[0x18];
    u8* slots;  // 0x18
};

static inline UnkTargetBlock_b0211* Block(UnkTargetOwner_b0211* self) {
    UnkTargetSlots_b0211* s = *reinterpret_cast<UnkTargetSlots_b0211**>(self->slots + 0x138);
    return *s->table->block;
}

void fn_7100a0ebf4(UnkTargetOwner_b0211* self, const f32* pos, f32 alpha, f32 strength) {
    CVector4D color = {0.0f, 1.0f, 1.0f, strength <= 0.0f ? 0.0f : strength};
    fn_71001436f4(reinterpret_cast<UnkStruct_71001436f4*>(self->slots), 0, &color, 1);
    UnkTargetBlock_b0211* block = Block(self);
    CVector4D value;
    f32 px = pos[0];
    f32 py = pos[1];
    value.x = px;
    value.y = py;
    value.z = 0.0f;
    value.w = alpha;
    fn_710007b518(&block->color, &value);
    if (block->node)
        *block->node->value = value;
}
