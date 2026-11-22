// CChozoCommanderXLifeComponent's unit (tools/tu_map.py unit 1369): when the value at +0x26c reached the
// AI's limit and the flag at +0x268 is set, marks the component (+0x270), sends the owner's animation a
// request and counts "<scenario>#<owner name>" (fn_7100ec78e8); then turns the player's input and aim keys
// back on, updates its hyper beam and animation, and spawns the actor of g_unk_71727302b0 at the player's
// position. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "base/global/CRntVector.h"
#include "game/CBossAIComponent.h"
#include "game/CCharClassEventScenarioComponent.h"
#include "game/CGunComponent.h"
#include "game/CInputComponent.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2571.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game/UnkTU57.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::global::UnkStringData;
using base::reflection::UnkReflObject;
using base::reflection::UnkReflTypeIdArg;

struct UnkStruct_710083e9d4;
struct UnkItem_710083e9d4;
struct UnkCounters_b0166;
struct UnkAimKeyListOwner;

void fn_7100ec78e8(UnkCounters_b0166* self, const CStrId* name);
void fn_71003448d4(UnkAimKeyListOwner* self, bool on, base::global::CStrId* key);
void fn_71003448ec(UnkAimKeyListOwner* self, bool on, base::global::CStrId* key);
void fn_7100846e7c(UnkItem_710083e9d4* gun, bool flag);
void fn_71008439f4(UnkItem_710083e9d4* gun);

struct UnkRefl_fn_71004daf78;
template <>
base::reflection::UnkReflObject*
base::reflection::refl_FindTyped<UnkRefl_fn_71004daf78>(void*);

extern __attribute__((visibility("hidden"))) char g_unk_71727302a0[];
// The aim key the player's aim lists are switched with.
extern __attribute__((visibility("hidden"))) CStrId g_unk_7172730298;
// The id of the actor spawned at the player.
extern __attribute__((visibility("hidden"))) CStrId g_unk_71727302b0;

// A string id built from a string (fn_7100000850).
struct UnkCommanderXName_l8be4e0 {
    UnkStringData* data;

    explicit UnkCommanderXName_l8be4e0(const CRntString& str) {
        fn_7100000850(reinterpret_cast<base::global::UnkStrIdBase*>(this), str, true);
    }
    ~UnkCommanderXName_l8be4e0() { reinterpret_cast<CStrId*>(this)->~CStrId(); }
    const CStrId* Id() const { return reinterpret_cast<const CStrId*>(this); }
};

struct UnkCommanderXAI_l8be4e0 {
    u8 pad_0x0[0xa4];
    f32 field_0xa4;
};

struct UnkCommanderXOwner_l8be4e0 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x1e8];
    void* animation;  // 0x220
};

struct UnkCommanderXLife_l8be4e0 {
    u8 pad_0x0[0x30];
    UnkCommanderXOwner_l8be4e0* owner;
    u8 pad_0x38[0x208];
    UnkCommanderXAI_l8be4e0* ai;  // 0x240
    u8 pad_0x248[0x20];
    bool field_0x268;
    f32 field_0x26c;
    bool defeated;  // 0x270
};
static_assert(__builtin_offsetof(UnkCommanderXLife_l8be4e0, field_0x26c) == 0x26c, "layout");

struct UnkCommanderXPlayer_l8be4e0 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0xc];
    f32 y;  // 0x54
    u8 pad_0x58[0xc];
    f32 z;  // 0x64
    u8 pad_0x68[0x170];
    UnkInputComponent* input;  // 0x1d8
    u8 pad_0x1e0[0x40];
    UnkAnimationComponent* animation;  // 0x220
    u8 pad_0x228[0x80];
    UnkStruct_710083e9d4* guns;  // 0x2a8
    UnkAimKeyListOwner* aim;     // 0x2b0
};
static_assert(__builtin_offsetof(UnkCommanderXPlayer_l8be4e0, input) == 0x1d8, "layout");
static_assert(__builtin_offsetof(UnkCommanderXPlayer_l8be4e0, aim) == 0x2b0, "layout");

struct UnkCommanderXRockAI_l8be4e0 {
    u8 pad_0x0[0x14724];
    u32 state;
};

// The string an id holds, counting the read.
static inline const CRntString& Str(const CStrId& id) {
    ++id.data->reads;
    return id.data->String();
}

static inline CRntString operator+(const CRntString& a, const char* b) { return CRntString_ConcatCString(&a, b); }
static inline CRntString operator+(const CRntString& a, const CRntString& b) { return CRntString_Concat(&a, &b); }

// The counters (created on first use).
static inline UnkCounters_b0166* Counters() {
    u8* counters = g_unk_71726bc558;
    if (!counters) {
        counters = static_cast<u8*>(fn_71000839a0(0x10, 0x3f));
        fn_7100ec65fc(counters);
        g_unk_71726bc558 = counters;
    }
    return reinterpret_cast<UnkCounters_b0166*>(counters);
}

// Counts `name` (fn_7100ec78e8).
static inline void Count(UnkCounters_b0166* counters, const UnkCommanderXName_l8be4e0& name) {
    fn_7100ec78e8(counters, name.Id());
}

// The player's hyper beam, or null.
static inline UnkItem_710083e9d4* HyperBeam(UnkStruct_710083e9d4* guns) {
    UnkItem_710083e9d4* gun;
    {
        CStrId name("HyperBeam", true);
        gun = fn_710083e9d4(guns, reinterpret_cast<const u64*>(&name), true);
    }
    if (gun && fn_710009d50c(reinterpret_cast<UnkReflObject*>(gun)->vf_0x0(), fn_71008424e8()))
        return gun;
    return nullptr;
}

// Sends `target` the request `hash` with `mode` under the all-flags mask; the arguments go in `request`
// and `mask`.
static inline void Send(void* target, u64 hash, UnkReflTypeIdArg* request, u32 mode, UnkReflTypeIdArg* mask,
                        const void* desc = &g_unk_710164b958) {
    request->id = mode;
    mask->id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, request, desc, &g_unk_7172769740, mask);
}

void fn_71008be4e0(UnkCommanderXLife_l8be4e0* self) {
    UnkReflTypeIdArg owner_request, owner_mask;
    if (self->field_0x26c >= self->ai->field_0xa4 && self->field_0x268) {
        self->defeated = true;
        if (void* animation = self->owner->animation)
            Send(animation, 0x5656449f3bcd0112, &owner_request, 0x1000, &owner_mask);
        Count(Counters(),
              UnkCommanderXName_l8be4e0(Str(*fn_7100de9bf8(g_unk_7101d47310)) + "#" + Str(self->owner->name)));
    }

    UnkCommanderXPlayer_l8be4e0* player =
        reinterpret_cast<UnkCommanderXPlayer_l8be4e0*>(fn_7100dec788(g_unk_7101d47310));
    if (!player)
        return;
    if (player->input)
        fn_7100872460(player->input, true, false, g_unk_71727302a0);
    if (player->aim) {
        fn_71003448d4(player->aim, false, &g_unk_7172730298);
        fn_71003448ec(player->aim, false, &g_unk_7172730298);
    }
    if (player->guns) {
        UnkItem_710083e9d4* beam = HyperBeam(player->guns);
        if (beam && self->defeated)
            fn_7100846e7c(beam, false);
    }

    UnkReflTypeIdArg idle_request, idle_mask;
    UnkReflTypeIdArg defeat_request, defeat_mask;
    if (void* animation = player->animation) {
        if (!self->defeated)
            Send(animation, 0x054287c91177295f, &idle_request, 0, &idle_mask);
        else
            Send(animation, 0x8b433509a5380892, &defeat_request, 0, &defeat_mask);
    }
    if (player->guns && !fn_710117b348(player->animation, fn_7100c1e458())) {
        if (UnkItem_710083e9d4* beam = HyperBeam(player->guns))
            fn_71008439f4(beam);
    }

    u32 type = g_unk_7172a0ba48;
    UnkReflTypeIdArg rock_request, rock_mask;
    auto* rock = reinterpret_cast<UnkActor*>(
        fn_710119438c(g_unk_71726a0220, reinterpret_cast<const char*>(&Str(g_unk_71727302b0)), &g_unk_71727302b0,
                      reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
    if (!rock)
        return;
    auto* ai = reinterpret_cast<UnkCommanderXRockAI_l8be4e0*>(
        base::reflection::refl_FindTyped<UnkRefl_fn_71004daf78>(rock));
    UnkWeakTarget* animation = fn_71007e4924(rock);
    if (!ai || !animation)
        return;
    ai->state = 4;
    Send(animation, 0x8b433509a5380892, &rock_request, 2, &rock_mask, &g_unk_710164b968);
    UnkVec3f position(player->x, player->y, player->z);
    rock->vf_0xd0(&position, false);
}
