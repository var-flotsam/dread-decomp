// CChozoCommanderKiGrabAttack's unit (tools/tu_map.py): the attack's end. After the base reset
// (fn_71004269a4), a held grab (+0x138) tells the owner's CChozoCommanderAIComponent (fn_71004546b4);
// otherwise the target's life takes the release hit (a request with the static id g_unk_71726c7df0) and
// the player's life drops by what it leaves, or ends (+0x13b). A boss camera gets 1.0 at +0x344, the
// state machine (+0x88) goes to state 1 and the link at +0x80 is dropped. Names and layouts are
// placeholders.
#include "../CSensorDoorComponent/UnkDoorStateMachine_b58.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCharClassAttackComponent.h"
#include "game/CPlayerLifeComponent.h"
#include "game/UnkActor.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1370.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkChozoCommanderAttack.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;

extern CStrId g_unk_71726c7df0 __attribute__((visibility("hidden")));

struct UnkGrabLife_A2_b71;
struct UnkGrabRequest_A2_b71;

struct UnkKiGrabAI_l430aa0 {
    void** vtable;
    const base::reflection::CClass* vf_0x0() {
        return reinterpret_cast<const base::reflection::CClass* (*)(UnkKiGrabAI_l430aa0*)>(vtable[0])(this);
    }
};
void fn_71004546b4(UnkKiGrabAI_l430aa0* ai);

// The camera controller once it tests as a CBossCameraCtrl (fn_710074eca8).
struct UnkKiGrabBossCamera_l430aa0 : UnkCameraCtrl {
    u8 pad_0x28[0x31c];
    f32 field_0x344;
};

struct UnkKiGrabRequest_l430aa0 {
    u8 pad_0x0[0x8];
    UnkWeakHandle* source;  // 0x8
    u8 pad_0x10[0x11];
    bool field_0x21;
    u8 pad_0x22[0x96];
    u8 id_raw[0x8];  // 0xb8: a CStrId
    u8 pad_0xc0[0x38];
    f32 left;  // 0xf8
    u8 pad_0xfc[0x4];

    CStrId& Id() { return *reinterpret_cast<CStrId*>(id_raw); }

    UnkKiGrabRequest_l430aa0() { fn_71008c0c74(reinterpret_cast<UnkStruct_71008c0c74*>(this)); }
    ~UnkKiGrabRequest_l430aa0() { fn_71008c1558(reinterpret_cast<UnkBase_71003cc474*>(this)); }
};
static_assert(sizeof(UnkKiGrabRequest_l430aa0) == 0x100, "request layout");

struct UnkKiGrabLife_l430aa0 {
    void** vtable;
    u8 pad_0x8[0x4c];
    f32 life;  // 0x54
    u8 pad_0x58[0x4];
    bool guarded;  // 0x5c

    void vf_0x168(f32 value, bool flag) {
        reinterpret_cast<void (*)(UnkKiGrabLife_l430aa0*, f32, bool)>(vtable[0x168 / 8])(this, value, flag);
    }
    void vf_0x180(bool a, bool b) {
        reinterpret_cast<void (*)(UnkKiGrabLife_l430aa0*, bool, bool)>(vtable[0x180 / 8])(this, a, b);
    }
};

struct UnkKiGrabBody_l430aa0 {
    u8 pad_0x0[0x1c0];
    UnkKiGrabLife_l430aa0* life;  // 0x1c0
};

struct UnkKiGrabAttack_l430aa0 {
    u8 pad_0x0[0x8];
    UnkActor* owner;  // 0x8
    u8 pad_0x10[0x58];
    u8 link[0x18];             // 0x68
    UnkWeakHandle* handle;     // 0x80
    UnkDoorStateMachine_b58 states;  // 0x88
    bool grabbed;  // 0x138
    u8 pad_0x139[2];
    bool ended;  // 0x13b
};
static_assert(__builtin_offsetof(UnkKiGrabAttack_l430aa0, states) == 0x88, "layout");
static_assert(__builtin_offsetof(UnkKiGrabAttack_l430aa0, grabbed) == 0x138, "layout");

void fn_7100430aa0(UnkKiGrabAttack_l430aa0* self) {
    fn_71004269a4(reinterpret_cast<UnkChozoCommanderAttack*>(self));
    if (self->grabbed) {
        auto* ai = static_cast<UnkKiGrabAI_l430aa0*>(self->owner->field_0x240);
        if (ai && fn_710009d50c(ai->vf_0x0(), fn_710044e708()))
            fn_71004546b4(ai);
    } else {
        auto* target = reinterpret_cast<UnkKiGrabBody_l430aa0*>(fn_7100d7c990(self->owner));
        if (UnkKiGrabLife_l430aa0* life = target->life) {
            UnkKiGrabRequest_l430aa0 request;
            request.field_0x21 = true;
            request.Id() = g_unk_71726c7df0;
            fn_71008c1538(reinterpret_cast<UnkRequest*>(&request), fn_7100789f5c(&request.Id()));
            AssignRef(&request.source, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(self->owner)));
            fn_7101166c6c(reinterpret_cast<UnkGrabLife_A2_b71*>(life), reinterpret_cast<UnkGrabRequest_A2_b71*>(&request));
            f32 left = request.left;
            auto* player = reinterpret_cast<UnkKiGrabBody_l430aa0*>(fn_7100dec788(g_unk_7101d47310));
            if (player && player->life && !player->life->guarded) {
                UnkKiGrabLife_l430aa0* player_life = player->life;
                if (!(player_life->life <= left)) {
                    player_life->vf_0x168(player_life->life - left, true);
                } else {
                    self->ended = true;
                    player_life->vf_0x180(true, false);
                }
            }
        }
    }
    if (void* holder = fn_7100defcec(g_unk_7101d47310)) {
        UnkCameraCtrl* camera = fn_71010e2080(holder);
        if (camera && fn_710009d50c(camera->vf_0x0(), fn_710074eca8()))
            static_cast<UnkKiGrabBossCamera_l430aa0*>(camera)->field_0x344 = 1.0f;
    }
    RequestState_b58(&self->states, 1);
    if (UnkWeakHandle* handle = self->handle) {
        if (handle->target)
            fn_710117b3f0(handle->target, self->link);
        ResetRef(&self->handle);
    }
}
