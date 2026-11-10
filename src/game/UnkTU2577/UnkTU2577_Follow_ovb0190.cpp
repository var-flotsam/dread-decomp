// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee5bc4: keeps the position the follow actor is moved to: when the actor is far from the anchor it is placed at the stored position (fn_710117b348 tests the character class). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern "C" float sqrtf(float);

// The actor the manager follows: a transform at +0x38 (its translation at +0x44, +0x54, +0x64)
// and a character class at +0x220; slot 0xd0 sets its position.
struct UnkFollowActor_b190 {
    void** vtable;
    u8 pad_0x8[0x3c];
    f32 pos_x;  // 0x44
    u8 pad_0x48[0xc];
    f32 pos_y;  // 0x54
    u8 pad_0x58[0xc];
    f32 pos_z;  // 0x64
    u8 pad_0x68[0x1b8];
    UnkAnimationComponent* field_0x220;

    void SetPosition(const UnkVec3f* pos, bool flag) {
        reinterpret_cast<void (*)(UnkFollowActor_b190*, const UnkVec3f*, bool)>(vtable[0xd0 / 8])(this, pos, flag);
    }
};
static_assert(__builtin_offsetof(UnkFollowActor_b190, pos_x) == 0x44, "layout");
static_assert(__builtin_offsetof(UnkFollowActor_b190, field_0x220) == 0x220, "layout");

struct UnkSubAreaMgr_b190 {
    u8 pad_0x0[0x11c];
    UnkVec3f field_0x11c;
    u8 pad_0x128[0xc];
    UnkVec3f field_0x134;
    u8 pad_0x140[8];
    UnkFollowActor_b190* actor;  // 0x148
    bool active;                 // 0x150
};
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, field_0x134) == 0x134, "layout");
static_assert(__builtin_offsetof(UnkSubAreaMgr_b190, actor) == 0x148, "layout");

void fn_7100ee5bc4(UnkSubAreaMgr_b190* self) {
    UnkFollowActor_b190* actor = self->actor;
    if (actor && self->active) {
        UnkAnimationComponent* character = actor->field_0x220;
        if (!(character && fn_710117b348(character, fn_7100c4041c()))) {
            const UnkVec3f& a = self->field_0x11c;
            if (a.x * a.x + a.y * a.y + a.z * a.z != 0.0f) {
                f32 px = actor->pos_x;
                f32 py = actor->pos_y;
                f32 pz = actor->pos_z;
                f32 dx = px - a.x;
                f32 dy = py - a.y;
                f32 dz = pz - a.z;
                f32 dist_sq = dx * dx + dy * dy + dz * dz;
                bool go_far = false;
                if (sqrtf(dist_sq) < 200.0f) {
                    const UnkVec3f& b = self->field_0x134;
                    f32 bx = b.x;
                    f32 by = b.y;
                    f32 bz = b.z;
                    if (bx * bx + by * by + bz * bz != 0.0f) {
                        f32 ex = bx - a.x;
                        f32 ey = by - a.y;
                        f32 ez = bz - a.z;
                        go_far = !(ex * ex + ey * ey + ez * ez <= dist_sq);
                    }
                    if (!go_far) {
                        a_u32 cx = *(a_u32*)&actor->pos_x;
                        a_u32 cy = *(a_u32*)&actor->pos_y;
                        a_u32 cz = *(a_u32*)&actor->pos_z;
                        ((a_u32*)&self->field_0x134)[0] = cx;
                        ((a_u32*)&self->field_0x134)[1] = cy;
                        ((a_u32*)&self->field_0x134)[2] = cz;
                    }
                } else {
                    go_far = sqrtf(dist_sq) > 200.0f;
                }
                if (go_far) {
                    if (character && fn_710117b348(character, fn_7100d24338())) {
                        UnkVec3f v;
                        a_u32 vx = ((a_u32*)&self->field_0x134)[0];
                        a_u32 vy = *(a_u32*)&actor->pos_y;
                        a_u32 vz = ((a_u32*)&self->field_0x134)[2];
                        ((a_u32*)&v)[0] = vx;
                        ((a_u32*)&v)[1] = vy;
                        ((a_u32*)&v)[2] = vz;
                        actor->SetPosition(&v, true);
                    } else {
                        actor->SetPosition(&self->field_0x134, true);
                    }
                }
            }
        }
    }
}
