// behaviortree::CSwitchBetweenAIModesAction's unit: updates the node. Once the animation component (+0x88)
// is playing the first mode (+0x48) or the AI (+0x80) reports it, it is done (5, or 2 while the AI runs it);
// otherwise it picks the turn direction towards the target, plays the track named by the id (+0x70), and on
// success tells the owner's two components which way it turns. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/CAnimationComponent.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/behaviortree/CSwitchBetweenAIModesAction.h"
#include "game_s2/UnkParams.h"
#include "types.h"

using base::global::CStrId;

struct UnkAnimComponent_ovb0009;
bool fn_7101178394(UnkAnimComponent_ovb0009* self, u64 key, void* id, const u32* flags);
bool fn_710006ba58(void* tracks, const base::reflection::CClass* cls);

struct UnkModesAI_l36b830 {
    void** vtable;
    bool vf_0x608(u32 mode) { return reinterpret_cast<bool (*)(UnkModesAI_l36b830*, u32)>(vtable[0x608 / 8])(this, mode); }
};

struct UnkSwitchModes_l36b830 {
    u8 pad_0x0[0x18];
    UnkActor* owner;  // 0x18
    u8 pad_0x20[0x18];
    CStrId target_id;  // 0x38
    u8 pad_0x40[0x8];
    u32 first;   // 0x48
    u32 second;  // 0x4c
    u32 result;  // 0x50
    u8 pad_0x54[0x14];
    bool started;  // 0x68
    bool field_0x69;
    bool field_0x6a;
    bool field_0x6b;
    CStrId track_id;  // 0x70
    bool field_0x78;
    bool field_0x79;
    bool played;  // 0x7a
    u8 pad_0x7b[5];
    UnkModesAI_l36b830* ai;  // 0x80
    void* anim;              // 0x88
    bool failed;             // 0x90
};

// Two parameter blocks for the owner's components (their vtables are g_unk_7101a24b58 and g_unk_7101a24b80).
struct UnkTurnParamsA_l36b830 {
    void** vtable;
    f32 value;  // 0x8
    u8 field_0xc[8];
    u8 field_0x14;
    u8 pad_0x15[7];
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
};

struct UnkTurnParamsB_l36b830 {
    void** vtable;
    u8 pad_0x8[4];
    u8 field_0xc;
    u8 pad_0xd[7];
    u8 field_0x14;
    u8 pad_0x15[3];
    f32 value;  // 0x18
    u8 field_0x1c[8];
    u8 pad_0x24;
    u8 field_0x24;
    u8 pad_0x25[3];
    u8 field_0x28[8];
    u8 pad_0x30[5];
    u8 field_0x2d[8];
    u8 pad_0x35[3];
    u32 field_0x38;
};

using base::math::CVector3D;

static inline CVector3D Direction(const CVector3D& to, const CVector3D& from) {
    f32 dx = to.x - from.x;
    f32 dy = to.y - from.y;
    f32 dz = to.z - from.z;
    f32 len = __builtin_sqrtf(dx * dx + dy * dy + dz * dz);
    f32 inv = 1.0f / (len + 0.0001f);
    CVector3D dir;
    dir.x = dx * inv;
    dir.y = dy * inv;
    dir.z = dz * inv;
    return dir;
}

static inline f32 TurnSign(UnkActor* owner) {
    return fn_710002614c(reinterpret_cast<const UnkMatrix44*>(&owner->field_0x38)).m[2] > 0 ? 1.0f : -1.0f;
}

u32 fn_710036b830(UnkSwitchModes_l36b830* self) {
    if (self->failed)
        return 6;
    if (self->track_id.data == &g_unk_7101d47108->empty ||
        (self->anim && self->first && fn_7101178e6c(static_cast<UnkTakumakuComponent*>(self->anim), self->first)))
        return 5;
    if (self->ai && self->ai->vf_0x608(self->first))
        return 2;
    if (!self->started) {
        if (self->target_id.data != &g_unk_7101d47108->empty) {
            UnkActor* target = fn_7100d7c990(self->owner);
            CVector3D to = {target->field_0x44, target->field_0x54, target->field_0x64};
            CVector3D from = {self->owner->field_0x44, self->owner->field_0x54, self->owner->field_0x64};
            CVector3D dir = {to.x - from.x, to.y - from.y, to.z - from.z};
            f32 len = __builtin_sqrtf(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
            f32 inv = 1.0f / (len + 0.0001f);
            dir.x *= inv;
            dir.y *= inv;
            dir.z *= inv;
            f32 side = dir.y * 0.0f + dir.x * TurnSign(self->owner) + dir.z * 0.0f;
            void* anim = self->anim;
            bool turning = fn_710117b348(static_cast<UnkAnimationComponent*>(anim), fn_7100d2b3a8());
            if (side < 0.0f && !turning && self->track_id.data != self->target_id.data)
                self->track_id = self->target_id;
        }
        u32 zero = 0;
        if (!fn_7101178394(static_cast<UnkAnimComponent_ovb0009*>(self->anim), 0xf4061ff25a2163d6,
                           reinterpret_cast<void*>(self->track_id.data->Key()), &zero))
            return self->result;
        base::reflection::UnkReflTypeIdArg mode, mask;
        mode.id = 0;
        mask.id = UnkAll_SpitSend().value;
        if (!CAnimationComponent_SendActionToTrack(self->anim, self->track_id.data->Key(), 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                           &g_unk_7172769740, &mask))
            return self->result;
        self->started = true;
        if (self->anim) {
            void* first = self->owner->field_0x298;
            void* second = self->owner->field_0x218;
            void* entry = fn_7101176d34(static_cast<UnkStruct_7101176ea8*>(self->anim), reinterpret_cast<void*>(self->track_id.data->Key()));
            if (entry && fn_710006ba58(static_cast<char*>(entry) + 0x170, fn_7100d2b3a8())) {
                f32 turn = -TurnSign(self->owner);
                if (first) {
                    UnkTurnParamsA_l36b830 a;
                    a.field_0x1c = 0;
                    a.field_0x34 = 0;
                    a.vtable = g_unk_7101a24b58 + 2;
                    __builtin_memset(a.field_0x20, 0, 16);
                    a.field_0x30 = 0;
                    a.value = turn;
                    __builtin_memset(a.field_0xc, 0, 8);
                    a.field_0x14 = 1;
                    fn_71006f4b0c(first, &a, 1);
                }
                if (second) {
                    UnkTurnParamsB_l36b830 b;
                    b.field_0xc = 0;
                    b.field_0x14 = 0;
                    b.vtable = g_unk_7101a24b80 + 2;
                    b.field_0x38 = 0;
                    __builtin_memset(b.field_0x28, 0, 8);
                    __builtin_memset(b.field_0x2d, 0, 8);
                    b.value = turn;
                    __builtin_memset(b.field_0x1c, 0, 8);
                    b.field_0x24 = 1;
                    fn_71010c7e6c(static_cast<UnkEffectSink*>(second), &b, 0);
                }
            }
        }
    }
    if (self->field_0x6a) {
        self->field_0x69 = true;
        self->field_0x6a = false;
        if (!self->played) {
            if (self->anim) {
                if (self->second)
                    fn_7101178e08(static_cast<UnkActorTarget*>(self->anim), self->second, false, false, -1.0f);
                if (self->first)
                    fn_710117662c(static_cast<UnkActorTarget*>(self->anim), self->first, 0, self->field_0x78,
                                  self->field_0x79, -1.0f);
            }
            self->played = true;
        }
    }
    if (!self->field_0x6b)
        return 5;
    return self->field_0x69 ? 5 : 2;
}
