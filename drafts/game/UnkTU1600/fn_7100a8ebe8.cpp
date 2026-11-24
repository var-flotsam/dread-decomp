// CTeleporterUsableComponent unit (tools/tu_map.py unit 1600): shows or hides the teleporter's dialog. It
// first disables the player's input, hooks the listener at +0x568 up to the player's target and sends the
// actor's part at +0x1b40 hash; on show it sets the loading screen's kind (6 when unset), tells the unit's
// listeners and starts the travel to the three names at +0x540, +0x558, +0x560. Names and layouts are
// placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAnimationComponent.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CInputComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2557.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkPlayer;
struct UnkInputComponent;

struct UnkClone_fn_710082d0ac;
using UnkTeleporterListeners = base::UnkDeferredSet<base::UnkDeferredCallback<u64>>;
extern template void base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(UnkTeleporterListeners*, const u64*);

struct UnkLinkGroups;

extern __attribute__((visibility("hidden"))) char g_unk_71727519c0[];

void fn_7100e10d78(UnkLoadingScreen* self, u32 kind);
void fn_7100a8f8f4();

struct UnkTeleporterShowId_la8ebe8 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkTeleporterShowPlayer_la8ebe8 {
    u8 pad_0x0[0x1d8];
    UnkInputComponent* input;
};

struct UnkTeleporterLink_la8ebe8 {
    u8 pad_0x0[0x8];
    void* context;
    void (*callback)();
};

struct UnkTeleporterShow_la8ebe8 {
    u8 pad_0x0[0x30];
    void* actor;
    u8 pad_0x38[0x53c - 0x38];
    u32 kind;  // 0x53c
    CStrId scenario;  // 0x540
    u8 pad_0x548[0x10];
    CStrId field_0x558;  // 0x558
    CStrId field_0x560;  // 0x560
    UnkTeleporterLink_la8ebe8 link;  // 0x568
    UnkWeakHandle* handle;  // 0x580

    // Takes the weak handle of `track` and links the listener to its target.
    void Track(UnkWeakTarget* track) {
        CAbilityGhostAura_AssignWeakHandleToRef(track, &handle);
        fn_710117b3e8(handle ? handle->target : nullptr, &link);
    }
};

static inline u64 PartHash_la8ebe8(u64 offset) {
    return (*reinterpret_cast<UnkTeleporterShowId_la8ebe8**>(
                reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + offset))
        ->hash;
}

// The loading screen, created on first use.
static inline UnkLoadingScreen* LoadingScreen() {
    UnkLoadingScreen* screen = g_unk_7101d47380;
    if (!screen) {
        screen = static_cast<UnkLoadingScreen*>(fn_71000839a0(0x220, 0x3f));
        fn_7100e0f89c(screen);
        g_unk_7101d47380 = screen;
    }
    return screen;
}

// The id's whole string, counting the read.
static inline const CRntString& UseName(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

void fn_7100a8ebe8(UnkTeleporterShow_la8ebe8* self, bool show) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    UnkPlayer* player = fn_7100dec788(g_unk_7101d47310);
    if (player) {
        UnkTeleporterShowPlayer_la8ebe8* p = reinterpret_cast<UnkTeleporterShowPlayer_la8ebe8*>(player);
        if (p->input)
            fn_7100872460(p->input, false, false, g_unk_71727519c0);
        if (UnkWeakTarget* track = fn_71007e4924(player)) {
            UnkWeakHandle* handle = self->handle;
            UnkTeleporterLink_la8ebe8* link = &self->link;
            link->context = self;
            link->callback = fn_7100a8f8f4;
            if (handle) {
                if (UnkWeakTarget* target = handle->target)
                    fn_710117b3f0(target, link);
                ResetRef(&self->handle);
            }
            self->Track(track);
        }
    }
    if (self->actor) {
        if (UnkWeakTarget* target = fn_71007e4924(self->actor)) {
            u64 hash = PartHash_la8ebe8(0x1b40);
            mode.id = 0;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
        }
    }
    fn_7100aa2570(reinterpret_cast<UnkUsableDialog_A6_b62*>(self), show);
    if (!show)
        return;
    if (self->kind == 0)
        self->kind = 6;
    {
        u64 arg = reinterpret_cast<u64>(self);
        base::refl_DeferredInvoke<UnkClone_fn_710082d0ac, u64>(
            reinterpret_cast<UnkTeleporterListeners*>(&g_unk_7172753260), &arg);
    }
    fn_7100e10d78(LoadingScreen(), self->kind);
    UnkGameManager* gm = g_unk_7101d47310;
    const CRntString& a = UseName(self->scenario);
    const CRntString& b = UseName(self->field_0x558);
    const CRntString& c = UseName(self->field_0x560);
    fn_7100df28d0(gm, a, b, c, CStrId(g_unk_710162e2ed, true), true, CRntString(g_unk_710162e2ed),
                  base::global::CStrId::Empty(), 0, nullptr, 0.0f);
}
