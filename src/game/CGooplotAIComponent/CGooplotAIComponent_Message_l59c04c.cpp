// CGooplotAIComponent unit (tools/tu_map.py unit 851): a message handler. After the base handler, when
// the message's flag is set and the owner's attack component reports true, counts the hit; in movement
// state 3, once the player is out of range or after a second hit, resets the owner's attack, restores
// the saved position and heading and clears the count. Names and layouts are placeholders.
#include "base/math/CVectorOps.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/CAttackComponent.h"
#include "game/CEnemyMovement.h"
#include "game/CGooplotAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2702.h"
#include "game/UnkTU54.h"
#include "types.h"

using base::math::CVector3D;

struct UnkGooplotPos_l59c04c {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
};

struct UnkGooplotAttack_l59c04c {
    void** vtable;
    void vf_0x178() { reinterpret_cast<void (*)(UnkGooplotAttack_l59c04c*)>(vtable[0x178 / 8])(this); }
};

struct UnkGooplotOwner_l59c04c : UnkGooplotPos_l59c04c {
    u8 pad_0x68[0x218 - 0x68];
    void* movement;  // 0x218
    void* field_0x220;
    u8 pad_0x228[0x278 - 0x228];
    UnkGooplotAttack_l59c04c* attack;  // 0x278
};

struct UnkGooplotTunable_l59c04c {
    u8 pad_0x0[0x34];
    f32 distance;  // 0x34
};

struct UnkGooplotAI_l59c04c {
    u8 pad_0x0[0x30];
    UnkGooplotOwner_l59c04c* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    bool field_0x146c8;
    u8 pad_0x146c9[0xf];
    bool field_0x146d8;
    u8 pad_0x146d9[0x147f8 - 0x146d9];
    s32 field_0x147f8;
    u8 pad_0x147fc[0x14871 - 0x147fc];
    bool field_0x14871;
    u8 pad_0x14872;
    bool field_0x14873;
    UnkVec3f angle;  // 0x14874
    UnkVec3f pos;  // 0x14880
    u8 pad_0x1488c[0x1489c - 0x1488c];
    UnkVec3f saved_pos;  // 0x1489c
    f32 saved_heading;  // 0x148a8
    s32 hits;  // 0x148ac
};
static_assert(__builtin_offsetof(UnkGooplotAI_l59c04c, hits) == 0x148ac, "");

struct UnkGooplotMsg_l59c04c {
    u8 pad_0x0[0x20];
    bool flag;  // 0x20
};

static inline CVector3D Pos(const UnkGooplotPos_l59c04c* p) {
    return {p->x, p->y, p->z};
}

u32 fn_710059c04c(UnkGooplotAI_l59c04c* self, UnkGooplotMsg_l59c04c* msg) {
    u32 result = fn_7101111dc8(reinterpret_cast<UnkNailongAI_b46a2*>(self),
                               reinterpret_cast<UnkNailongMsg_b46a2*>(msg));
    if (msg->flag) {
        void* attack = self->owner->attack;
        if (attack && fn_7101136308(attack)) {
            self->hits++;
            void* movement = self->owner->movement;
            if (movement && fn_71010cc3a4(movement) == 3) {
                UnkGooplotPos_l59c04c* player =
                    reinterpret_cast<UnkGooplotPos_l59c04c*>(fn_7100dec788(g_unk_7101d47310));
                if (player) {
                    UnkGooplotOwner_l59c04c* owner = self->owner;
                    if (owner->field_0x220) {
                        CVector3D a = Pos(owner);
                        CVector3D b = Pos(player);
                        f32 distSq = DistanceSq(a, b);
                        f32 limit = reinterpret_cast<const UnkGooplotTunable_l59c04c*>(&g_unk_71727bc558)->distance;
                        if (distSq > limit * limit || self->hits > 1) {
                            owner->attack->vf_0x178();
                            self->pos = self->saved_pos;
                            self->angle.x = self->saved_heading;
                            self->angle.y = 1.5707964f;
                            self->angle.z = 0.0f;
                            fn_710007a220(&self->angle);
                            self->field_0x14873 = true;
                            self->field_0x146c8 = true;
                            self->field_0x14871 = false;
                            self->hits = 0;
                        }
                    }
                }
            }
        }
    }
    if (self->field_0x147f8 < 1) {
        self->field_0x146d8 = false;
        fn_710059aa2c(reinterpret_cast<UnkGooplotAI_b55*>(self));
    }
    return result;
}
