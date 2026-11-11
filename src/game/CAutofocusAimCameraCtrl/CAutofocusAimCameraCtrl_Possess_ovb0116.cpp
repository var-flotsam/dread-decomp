// CAutofocusAimCameraCtrl unit (tools/tu_map.py, the name from reflection): taking and giving up the
// camera. Taking it (fn_7100722254) sets the start, current and slide slots from the argument's
// definition (+0x130, its +0xb4) and node position (+0x20, fn_7100206154), clears three members, runs
// the base step (fn_71010ed98c), gives the player's aim object (+0x2b0) the aim tunables' colours
// (g_unk_71726b1438; fn_71003440dc), tells the central-unit registry (fn_7100ddbabc) and every E.M.M.I.
// (fn_7100540810) and calls the script's CurrentScenario.OnAimCameraPossess. Giving it up: after
// the base reset (fn_71010edd64) every E.M.M.I. of the global list (g_unk_71726af0a8: a count and
// 4-byte-aligned ref-counted pointers, each entry's actor at +0x10) is told the aim camera is off
// (fn_7100540810), three members are cleared, the render manager's colour g_unk_710164a910 is set
// (fn_710021a0b4), the player's aim object (+0x2b0) gets the neutral colours (fn_71003440dc) and the
// lazily created central-unit registry g_unk_71726bee10 is told (fn_7100ddbb7c). Names and layouts
// are placeholders.
#include "base/UnkRefHandle.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/math/CVector4D.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU437.h"
#include "game/UnkXformNode.h"
#include "types.h"

struct UnkGrabCameraCtrl;
void fn_710021a0b4(UnkRenderManager* self, const u64* hash, const base::math::CVector4D* value);
extern const base::math::CVector4D g_unk_71016477b4;
extern const u64 g_unk_710164a910 __attribute__((visibility("hidden")));
extern const base::math::CVector4D g_unk_7101a6cfe8 __attribute__((visibility("hidden")));
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
char* fn_71003440dc(char* p0, u32 p1, char* p2, char* p3, char* p4, char* p5);

struct UnkDoorRegistry_b61;
struct UnkUnit2547;
void fn_7100ddbb7c(UnkUnit2547* self, u32 expected);
void fn_7100ddbabc(UnkUnit2547* self, u32 state, f32 value);
struct UnkFixedCamera_fc;
struct UnkArg_fc;
extern base::reflection::UnkTunable g_unk_717270f910;
struct UnkAimTunable;

// The aim tunables' colours (g_unk_71726b1438).
struct UnkAimColours_afc {
    u8 pad_0x0[0x64];
    UnkVec3f lock_inner;  // 0x64
    UnkVec3f lock_outer;  // 0x70
    UnkVec3f aim_inner;   // 0x7c
    UnkVec3f aim_outer;   // 0x88
};

struct UnkAutofocusTunable_afc {
    u8 pad_0x0[0x48];
    f32 field_0x48;
};

struct UnkEmmyActor_afc;
void fn_7100540810(UnkEmmyActor_afc* actor, bool on);

struct UnkEmmyEntry_afc : base::UnkRefCounted {
    u8 pad_0xc[4];
    UnkEmmyActor_afc* actor;  // 0x10
};

struct __attribute__((packed, aligned(4))) UnkEmmyEntryPtr_afc {
    base::UnkRefHandle h;
};

struct UnkEmmyList_afc {
    u32 count;
    UnkEmmyEntryPtr_afc items[2];
};

struct UnkEmmyRegistry_b55;

struct UnkPlayerAim_afc {
    u8 pad_0x0[0x2b0];
    char* aim;  // 0x2b0
};

struct UnkAutofocusDef_afc {
    u8 pad_0x0[0xb4];
    UnkVec3f dir;  // 0xb4
};

struct UnkAutofocusArg_afc {
    u8 pad_0x0[0x20];
    UnkXformNode node;  // 0x20
    UnkAutofocusDef_afc* def;  // 0x130
};

struct UnkAutofocusCtrl_afc {
    u8 pad_0x0[0x38];
    UnkVec3f start_pos;  // 0x38
    UnkVec3f start_dir;  // 0x44
    u8 pad_0x50[0x18];
    UnkVec3f pos;  // 0x68
    UnkVec3f dir;  // 0x74
    u8 pad_0x80[0x158];
    f32 field_0x1d8;
    UnkVec3f slide_dir;  // 0x1dc
    UnkVec3f slide_pos;  // 0x1e8
    u8 pad_0x1f4[0x1c];
    u64 field_0x210;
    u64 field_0x218;
    u64 field_0x220;
    u8 pad_0x228[8];
    u32 field_0x230;
    u8 pad_0x234[0xc];
    bool pending;  // 0x240
};
static_assert(__builtin_offsetof(UnkAutofocusCtrl_afc, pending) == 0x240, "UnkAutofocusCtrl_afc layout");

static inline void NotifyEmmys_afc(bool on) {
    UnkEmmyList_afc* list =
        reinterpret_cast<UnkEmmyList_afc*>(reinterpret_cast<char*>(g_unk_71726af0a8) + 8);
    for (UnkEmmyEntryPtr_afc *it = list->items, *end = list->items + list->count; it != end; ++it) {
        UnkEmmyActor_afc* actor = it->h.ptr ? static_cast<UnkEmmyEntry_afc*>(it->h.ptr)->actor : nullptr;
        fn_7100540810(actor, on);
    }
}

static inline UnkUnit2547* Registry_afc() {
    UnkDoorRegistry_b61* reg = g_unk_71726bee10;
    if (!reg) {
        reg = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
        fn_7100ddb6dc(reg);
        g_unk_71726bee10 = reg;
    }
    return reinterpret_cast<UnkUnit2547*>(reg);
}

// A colour: the tunable's RGB and an alpha.
struct UnkColour_afc {
    UnkVec3f rgb;
    f32 alpha;
    UnkColour_afc(const UnkVec3f& c, f32 a) : rgb(c), alpha(a) {}
};

void fn_7100722254(UnkAutofocusCtrl_afc* self, UnkAutofocusArg_afc* arg) {
    self->slide_dir = arg->def->dir;
    self->dir = self->slide_dir;
    self->start_dir = self->dir;
    {
        UnkVec3f pos = fn_7100206154(&arg->node);
        self->slide_pos = pos;
        self->pos = self->slide_pos;
        self->start_pos = self->pos;
    }
    self->field_0x1d8 = 0.0f;
    self->pending = false;
    self->field_0x230 = 0;
    fn_71010ed98c(reinterpret_cast<UnkFixedCamera_fc*>(self), reinterpret_cast<UnkArg_fc*>(arg), 1);
    UnkPlayerAim_afc* player = reinterpret_cast<UnkPlayerAim_afc*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->aim) {
        const UnkAimColours_afc* t = reinterpret_cast<const UnkAimColours_afc*>(&g_unk_71726b1438);
        UnkColour_afc lock_inner(t->lock_inner, 1.0f);
        UnkColour_afc lock_outer(t->lock_outer, 0.5f);
        UnkColour_afc aim_inner(t->aim_inner, 1.0f);
        UnkColour_afc aim_outer(t->aim_outer, 0.5f);
        fn_71003440dc(player->aim, 1, reinterpret_cast<char*>(&lock_inner), reinterpret_cast<char*>(&lock_outer),
                      reinterpret_cast<char*>(&aim_inner), reinterpret_cast<char*>(&aim_outer));
    }
    const UnkAutofocusTunable_afc* tunable = reinterpret_cast<const UnkAutofocusTunable_afc*>(&g_unk_717270f910);
    fn_7100ddbabc(Registry_afc(), 1, tunable->field_0x48);
    NotifyEmmys_afc(true);
    fn_7100000790("CurrentScenario.OnAimCameraPossess", g_unk_710162e2ed);
}

void fn_710072241c(UnkAutofocusCtrl_afc* self) {
    fn_71010edd64(reinterpret_cast<UnkGrabCameraCtrl*>(self));
    NotifyEmmys_afc(false);
    self->field_0x220 = 0;
    self->field_0x218 = 0;
    self->field_0x210 = 0;
    fn_710021a0b4(g_unk_7101d47300, &g_unk_710164a910, &g_unk_7101a6cfe8);
    UnkPlayerAim_afc* player = reinterpret_cast<UnkPlayerAim_afc*>(fn_7100dec788(g_unk_7101d47310));
    if (player && player->aim) {
        char* neutral = reinterpret_cast<char*>(const_cast<base::math::CVector4D*>(&g_unk_71016477b4));
        fn_71003440dc(player->aim, 0, neutral, neutral, neutral, neutral);
    }
    fn_7100ddbb7c(Registry_afc(), 1);
}
