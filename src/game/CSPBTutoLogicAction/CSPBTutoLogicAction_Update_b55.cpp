// CSPBTutoLogicAction unit (tools/tu_map.py, the name from reflection): the action's update. While enabled
// (+0x159), after the base step (fn_7100a55ac8) and with the game manager's tutorial composition
// (+0x2270, its +0x10): when the tutorial runs (+0x11), it follows the player's grab (the +0x1a0 object
// of +0x1b0, a CEmmyGrabSamusAttack: state 4 closes the tutorial once, states 13 and 7 reopen it),
// shows "#CAVE_CENTRALUNIT_TUTO_CAMERA" the first time, and once input 0x200 (sound holder
// g_unk_7101d47340) is pressed with the autofocus aim camera active, closes it, shows
// "#CAVE_CENTRALUNIT_TUTO_HEATING" and logs "#MLOG_TUTO_OMEGA_STREAM" (kind 2) to the game manager's
// +0x2128. Then, with +0x190 set, runs the inner action at +0xa0 (fn_7100a5377c). Names and layouts are
// placeholders.
#include "base/UnkAtExit.h"
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/CEmmyOverrideDeathPositionDef.h"
#include "game/CTriggerLogicAction.h"
#include "game/GUI/CTutorialComposition.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/nn/time.h"
#include "types.h"

using base::global::CStrId;

struct UnkTutoExitAction_b47;

struct UnkTutorialComposition;
struct UnkTutorialRequest;

struct UnkGrabAttack_b55 {
    const base::reflection::CType* GetType() {
        return (*reinterpret_cast<const base::reflection::CType* (***)(UnkGrabAttack_b55*)>(this))[0](this);
    }
};
s32 fn_710050b910(UnkGrabAttack_b55* self);

struct UnkSoundHolder;

struct UnkInput_b55 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual bool IsPressed(u32 button, u32 mask);
};

struct UnkSPBInner_b55;
void fn_7100a5377c(UnkSPBInner_b55* self, void* arg);

struct UnkMlogMessage_b42a1;

// The message's text id: copied like a CStrId, released in two steps like base::UnkStrIdMember.
struct UnkMlogText_b55 : base::global::UnkStrIdBase {
    __attribute__((always_inline)) UnkMlogText_b55(const CStrId& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    __attribute__((always_inline)) ~UnkMlogText_b55() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkMlogMessage_b55 {
    u32 kind;
    UnkMlogText_b55 text;
    base::UnkVector<base::UnkStrIdMember> args;

    __attribute__((always_inline)) UnkMlogMessage_b55(u32 k, const CStrId& t) : kind(k), text(t), args(0xc) {}
    __attribute__((always_inline)) ~UnkMlogMessage_b55() { args.Free(); }
};

struct UnkSPBTutoAction_b55 {
    u8 pad_0x0[0x10];
    bool shown;     // 0x10
    bool running;   // 0x11
    bool closed;    // 0x12
    u8 pad_0x13;
    s32 step;       // 0x14
    u8 pad_0x18[0xa0 - 0x18];
    u8 inner[0x190 - 0xa0];  // 0xa0
    bool inner_on;           // 0x190
};

static inline char* At(void* p, u32 off) {
    return *reinterpret_cast<char**>(reinterpret_cast<char*>(p) + off);
}

static inline UnkSoundHolder* SoundHolder() {
    if (!g_unk_7101d47340) {
        auto* h = static_cast<UnkSoundHolder*>(fn_71000839a0(0x48, 0x3f));
        fn_710006cd9c(h);
        g_unk_7101d47340 = h;
    }
    return g_unk_7101d47340;
}

void fn_7100a527f8(char* self_, void* arg) {
    auto* self = reinterpret_cast<UnkSPBTutoAction_b55*>(self_);
    if (!*reinterpret_cast<bool*>(self_ + 0x159))
        return;
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    if (!g_unk_7101d47310->field_0x2270)
        return;
    auto* tuto = reinterpret_cast<UnkTutorialComposition*>(At(g_unk_7101d47310->field_0x2270, 0x10));
    if (!tuto)
        return;
    if (self->running) {
        if (At(self, 0x1b0)) {
            if (auto* grab = reinterpret_cast<UnkGrabAttack_b55*>(At(At(self, 0x1b0), 0x1a0))) {
                if (fn_710009d50c(grab->GetType(), fn_7100504d60())) {
                    s32 state = fn_710050b910(grab);
                    if (state == 4) {
                        if (!self->closed) {
                            self->closed = true;
                            fn_7100b8903c(tuto, nullptr);
                        }
                        return;
                    }
                    if (self->closed && (state == 13 || state == 7)) {
                        self->closed = false;
                        self->shown = false;
                        self->step = -1;
                    }
                }
            }
        }
        if (self->closed)
            return;
        if (self->step == -1 && !self->shown) {
            static CStrId s_camera("#CAVE_CENTRALUNIT_TUTO_CAMERA", true);
            fn_7100b88f10(tuto, &s_camera);
            g_unk_71726e1ee8 = 2;
            self->shown = true;
        }
        auto* input = *reinterpret_cast<UnkInput_b55**>(SoundHolder());
        if (!input->IsPressed(0x200, 0xfd)) {
            self->step = -1;
        } else {
            auto* ctrl = fn_71010e2080(fn_7100defcec(g_unk_7101d47310));
            if (ctrl && fn_710009d50c(reinterpret_cast<UnkGrabAttack_b55*>(ctrl)->GetType(), fn_7100720464()) &&
                self->step == -1) {
                fn_7100b8903c(tuto, nullptr);
                static CStrId s_heating("#CAVE_CENTRALUNIT_TUTO_HEATING", true);
                fn_7100b88f10(tuto, &s_heating);
                UnkMlogMessage_b55 message(2, CStrId("#MLOG_TUTO_OMEGA_STREAM", true));
                fn_7100456b7c(reinterpret_cast<u8*>(g_unk_7101d47310) + 0x2128,
                              reinterpret_cast<const UnkMlogMessage_b42a1*>(&message));
                self->shown = false;
                self->step = 0;
            }
        }
    }
    if (self->inner_on)
        fn_7100a5377c(reinterpret_cast<UnkSPBInner_b55*>(self->inner), arg);
}
