// CTunableAIManager unit: switches a mode at +0x47 on or off while the object the weak handle at +0x48 points
// to is alive. On, the player's input component gets a list of eight flags under a key and the current
// camera controller is kept through the weak handle at +0x58: an autofocus aim controller is settled,
// any other is replaced by the one the id table's +0x868 names. Off, an autofocus aim controller is told
// (fn_7100723464) or else the cameras are dropped, and the flags are taken back. Names and layouts are
// placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCameraComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkFlagListS7.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU2553.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using unk_flag_list::UnkFlagList;
using unk_flag_list::UnkFlagListKey;

struct UnkAutofocusCtrl_b54;
void fn_7100723408(UnkAutofocusCtrl_b54* self);
void fn_7100723464(UnkCameraCtrl* ctrl);
void fn_7100723120(void* holder);

extern UnkFlagListKey g_unk_71727bc7d0 __attribute__((visibility("hidden")));

inline const CStrId& AutofocusCameraId_ldae000() {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    // Needs UnkGameManagerIdTable::field_0x868 (a CStrId) in game/UnkGameManagerIdTable.h.
    return s_table.field_0x868;
}

struct UnkAimPlayer_ldae000 {
    u8 pad_0x0[0x1d8];
    void* input;  // 0x1d8
};

struct UnkAimMode_ldae000 {
    u8 pad_0x0[0x47];
    bool enabled;            // 0x47
    UnkWeakHandle* owner;    // 0x48
    u8 pad_0x50[0x8];
    UnkWeakHandle* camera;   // 0x58
};

static inline bool SameId(const CStrId& a, const CStrId& b) { return a.data == b.data; }

static inline UnkCameraCtrl* TargetOf(UnkWeakHandle* handle) {
    return handle ? reinterpret_cast<UnkCameraCtrl*>(handle->target) : nullptr;
}

// The holder's current camera controller, or null without a holder.
static inline UnkCameraCtrl* CurrentCamera(void* holder) { return holder ? fn_71010e2080(holder) : nullptr; }

static inline bool IsAutofocus(UnkCameraCtrl* ctrl) {
    if (!ctrl)
        return false;
    const base::reflection::CClass* cls = ctrl->vf_0x0();
    return fn_710009d50c(cls, fn_7100720464());
}

static inline UnkCameraCtrl* AsAutofocus(UnkCameraCtrl* ctrl) {
    if (!ctrl)
        return nullptr;
    const base::reflection::CClass* cls = ctrl->vf_0x0();
    return fn_710009d50c(cls, fn_7100720464()) ? ctrl : nullptr;
}

void fn_7100dae000(UnkAimMode_ldae000* self, bool enable) {
    if (!self->owner || !self->owner->target)
        return;
    if (enable == self->enabled)
        return;
    self->enabled = enable;
    UnkAimPlayer_ldae000* player = reinterpret_cast<UnkAimPlayer_ldae000*>(fn_7100dec788(g_unk_7101d47310));
    void* input = player ? player->input : nullptr;
    void* holder = fn_7100defcec(g_unk_7101d47310);
    if (self->enabled) {
        if (input) {
            UnkFlagList flags;
            flags.PushBack(0x100);
            flags.PushBack(0x8000);
            flags.PushBack(0x20);
            flags.PushBack(0x80);
            flags.PushBack(0x100000);
            flags.PushBack(0x800);
            flags.PushBack(0x40000);
            flags.PushBack(0x80000);
            fn_71008724f8(input, &g_unk_71727bc7d0, &flags, false, false, false, true);
        }
        UnkCameraCtrl* ctrl = fn_71010e2080(holder);
        AssignRef(&self->camera, GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(ctrl)));
        if (SameId(TargetOf(self->camera)->field_0x20, AutofocusCameraId_ldae000()))
            fn_7100723408(reinterpret_cast<UnkAutofocusCtrl_b54*>(AsAutofocus(fn_71010e2080(holder))));
        else
            fn_71010e4168(holder, &AutofocusCameraId_ldae000(), false);
    } else {
        if (IsAutofocus(CurrentCamera(holder)))
            fn_7100723464(AsAutofocus(CurrentCamera(holder)));
        else
            fn_7100723120(holder);
        if (input)
            fn_7100872858(input, &g_unk_71727bc7d0);
    }
}
