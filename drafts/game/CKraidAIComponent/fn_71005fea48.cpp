// CKraidAIComponent's unit (tools/tu_map.py 921): whether a hit is blocked. With the mouth open (the
// timer at +0x1499c running or the mouth-opened track playing) a hit is blocked when it lies behind
// one node's facing and in front of the other's, in the horizontal plane; otherwise when the part hit
// is the one with the given name. Unblocked hits in mode 1 go on to fn_71005feca4.
// Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/CVectorOps.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CCollisionComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/UnkAnimationComponent.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector3D;

extern char g_unk_710164a230[];
extern char g_unk_710164a238[];

struct UnkKraidOwner_l5fea48 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
    UnkEntity* model;                  // 0x228
    u8 pad_0x230[0x70];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkHitObject_l5fea48 {
    u8 pad_0x0[0x10];
    void* collider;  // 0x10
};

struct UnkHit_l5fea48 {
    UnkHitObject_l5fea48* object;  // 0x0
    u8 pad_0x8[0xc];
    CVector3D position;  // 0x14
};

struct UnkKraidAI_l5fea48 {
    u8 pad_0x0[0x30];
    UnkKraidOwner_l5fea48* owner;  // 0x30
    u8 pad_0x38[0x1140 - 0x38];
    s32 mode;  // 0x1140
    u8 pad_0x1144[0x1499c - 0x1144];
    f32 mouth_time;  // 0x1499c

    // The matrix of the owner's model node `key` (identity when it has none).
    UnkMatrix44 NodeMatrix(const char* key) {
        UnkMatrix44 m = {};
        m.m[15] = 1.0f;
        m.m[10] = 1.0f;
        m.m[5] = 1.0f;
        m.m[0] = 1.0f;
        fn_71011735e8(owner->model, key, &m, 0);
        return m;
    }
};

bool fn_71005feca4(UnkKraidAI_l5fea48* self, const CVector3D* position);

bool fn_71005fea48(UnkKraidAI_l5fea48* self, UnkHit_l5fea48* hit) {
    bool blocked;
    if (self->mouth_time != 0.0f || fn_710117b348(self->owner->animation, fn_7100c3a598())) {
        UnkMatrix44 front = self->NodeMatrix(g_unk_710164a230);
        UnkMatrix44 back = self->NodeMatrix(g_unk_710164a238);
        const CVector3D& p = hit->position;
        // Each node's facing (its z axis) flattened to the horizontal plane, and its position.
        CVector3D front_facing = {front.m[2], front.m[6], 0.0f};
        CVector3D front_position = {front.m[3], front.m[7], front.m[11]};
        CVector3D back_facing = {back.m[2], back.m[6], 0.0f};
        CVector3D back_position = {back.m[3], back.m[7], back.m[11]};
        bool behind_front = Dot(-(p - front_position), front_facing) > 0.0f;
        bool before_back = Dot(p - back_position, back_facing) > 0.0f;
        blocked = behind_front & before_back;
    } else {
        UnkCollisionContainer* collision = self->owner->collision;
        if (!collision || !hit->object || !hit->object->collider)
            return true;
        CStrId part = *fn_7100864018(collision, hit->object->collider);
        blocked = part.data->Key() == 0x3fdc7cb0960d917dULL;
    }
    if (blocked)
        return true;
    if (self->mode == 1)
        return fn_71005feca4(self, &hit->position);
    return false;
}
