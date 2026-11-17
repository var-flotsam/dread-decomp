// animtree::CAnimatedNode's unit: an update step that sets the player's looping flag, advances it by the
// context's time, blends the player's motion into the context's two motion parts around it while the
// motion has a mode, then runs the shared update (fn_71002dfbec). Names and layouts are placeholders.
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU1141.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkFlags_71007142e8 {
    u32 bits;
    UnkFlags_71007142e8() : bits(0) {}
    UnkFlags_71007142e8(u32 b) : bits(b) {}
    UnkFlags_71007142e8(const UnkFlags_71007142e8& o) : bits(o.bits) {}
};

inline const UnkFlags_71007142e8& UnkFlagsNone_l2d2aa0() {
    static UnkFlags_71007142e8 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_71007142e8& UnkFlagsAll_l2d2aa0() {
    static UnkFlags_71007142e8 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_l2d2aa0().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    UnkXform xform;  // 0x0
    u32 mode;  // 0x5c
    base::math::CVector3D vector;  // 0x60
    f32 field_0x6c;
};
static_assert(sizeof(UnkStruct_71007142e8) == 0x70, "layout");

struct UnkUnit1134_b64a4;
struct UnkUnit1134Upd_b68a4;
void fn_710070d778(UnkUnit1134_b64a4* self, bool value);
void fn_710070d970(UnkUnit1134Upd_b68a4* self, f32 dt);
void fn_7100714808(void* part, UnkStruct_71007142e8* motion, s32);

struct UnkAnimPlayer_l2d2aa0 {
    u8 pad_0x0[0x38];
    UnkStruct_71007142e8 motion;
};

struct UnkMotionPart_l2d2aa0 {
    u8 pad_0x0[0x70];
};

struct UnkAnimContext_l2d2aa0 {
    f32 time;
    u8 pad_0x4[0x78 - 0x4];
    UnkMotionPart_l2d2aa0 part_0x78;
    UnkMotionPart_l2d2aa0 part_0xe8;
};

struct UnkAnimatedNode_l2d2aa0 {
    u8 pad_0x0[0x64];
    f32 field_0x64;
    u8 pad_0x68[0x70 - 0x68];
    UnkAnimPlayer_l2d2aa0* player;
    f32 field_0x78;
    u8 pad_0x7c[1];
    bool field_0x7d;
    bool field_0x7e;
};

void fn_71002dfbec(UnkAnimatedNode_l2d2aa0* self, void* arg1, s32 arg2, UnkAnimContext_l2d2aa0* context);

static inline void BlendMotion(UnkAnimatedNode_l2d2aa0* self, UnkStruct_71007142e8* motion, UnkMotionPart_l2d2aa0* part) {
    if (motion->mode != 0) {
        if (self->field_0x7e)
            fn_7100714808(part, motion, 0);
        UnkFlags_71007142e8 flags = UnkFlagsAll_l2d2aa0();
        fn_7100714eb4(motion, &flags.bits);
    }
}

void fn_71002d2aa0(UnkAnimatedNode_l2d2aa0* self, void* arg1, s32 arg2, UnkAnimContext_l2d2aa0* context) {
    fn_710070d778(reinterpret_cast<UnkUnit1134_b64a4*>(self->player),
                  self->field_0x7d && self->field_0x64 >= self->field_0x78);
    UnkStruct_71007142e8* motion = &self->player->motion;
    BlendMotion(self, motion, &context->part_0xe8);
    fn_710070d970(reinterpret_cast<UnkUnit1134Upd_b68a4*>(self->player), context->time);
    BlendMotion(self, motion, &context->part_0x78);
    fn_71002dfbec(self, arg1, arg2, context);
}
