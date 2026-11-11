// CCameraToRailLogicAction unit (tools/tu_map.py, the name from reflection): the action's step. After the
// base step (fn_7100a55ac8), with the flag at +0x10 set it points the rail camera controller (when the
// holder's controller for the id at +0x1d30 of the id table is a CRailCameraCtrl) at the +0x30 part of the
// rail component of the entity at +8, then switches to that id; with the flag clear it runs
// fn_7100a4dffc on the holder and switches to the id at +0x17e0. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CCameraComponent.h"
#include "game/CTriggerLogicAction.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkTutoExitAction_b47;

struct UnkRailCameraCtrl_b53 {
    void** vtable;
    u8 pad_0x8[0x2a0 - 0x8];
    void* field_0x2a0;

    const void* vf_0x0() { return reinterpret_cast<const void* (*)(UnkRailCameraCtrl_b53*)>(vtable[0])(this); }
};
struct UnkCameraCtrl;
void fn_71010e4168(void* holder, const CStrId* id, bool flag);

struct UnkRefl_fn_7100a4df20;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_7100a4df20>(void*);

void fn_7100a4dffc(void* holder);

struct UnkCameraToRailAction_b53 {
    void* vtable;
    void* entity;  // 0x8
    bool enable;   // 0x10
};

static inline char* IdTable() {
    return reinterpret_cast<char*>(&base::reflection::refl_GetSingleton());
}

void fn_7100a4dd3c(UnkCameraToRailAction_b53* self) {
    fn_7100a55ac8(reinterpret_cast<UnkTutoExitAction_b47*>(self));
    bool enable = self->enable;
    void* holder = fn_7100defcec(g_unk_7101d47310);
    if (enable) {
        if (!holder)
            return;
        auto* ctrl = reinterpret_cast<UnkRailCameraCtrl_b53*>(fn_71010e389c(holder, IdTable() + 0x1d30));
        if (ctrl && ctrl->vf_0x0() == fn_71009d6164()) {
            char* rail = reinterpret_cast<char*>(
                base::reflection::refl_FindTyped<UnkRefl_fn_7100a4df20>(self->entity));
            if (rail)
                ctrl->field_0x2a0 = rail + 0x30;
        }
        fn_71010e4168(holder, reinterpret_cast<const CStrId*>(IdTable() + 0x1d30), false);
    } else {
        if (!holder)
            return;
        fn_7100a4dffc(holder);
        fn_71010e4168(holder, reinterpret_cast<const CStrId*>(IdTable() + 0x17e0), false);
    }
}
