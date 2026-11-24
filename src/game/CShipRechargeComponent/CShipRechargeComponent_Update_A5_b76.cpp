// CShipRechargeComponent unit (tools/tu_map.py, unit 1598): the three functions with function-local
// statics, in the unit's order: the save station's loop sound (fn_7100a8b840), the recharge sound
// (fn_7100a8bc44) and the update (fn_7100a8bfc8).
//
// They are in one file because their statics are laid out together: the compiler merges a unit's
// internal globals into one block sorted by size, so each function's 8-byte statics (paths and guards)
// come first, in the order the functions appear here, and the 0x70-byte sound descriptions after them.
// A function built apart would find its description right after its own guards.
//
// Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/snd/CSoundSystem.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CInputComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkKraidCreatureVec.h"
#include "game/UnkSaveGamePair_b54.h"
#include "game/UnkSoundEntry_A5_b76.h"
#include "game/UnkSoundRef.h"
#include "game/UnkSoundRef_A5_b76.h"
#include "game/UnkTU23.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

// Whether the sound a handle refers to is still the one it was started as, and playing.
static inline bool IsPlaying(const UnkSoundHandle& handle) {
    const UnkSoundRef* ref = handle.ref;
    return ref && ref->serial == handle.serial && handle.serial != -1 && ref->object;
}

// Called back once the game is saved.
void fn_7100a8c670();

struct UnkShipRechargeOwner_la8b840 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkShipRecharge_la8b840;
struct UnkShipRechargeVtable_la8b840 {
    u8 pad_0x0[0x2b0];
    void (*vf_0x2b0)(UnkShipRecharge_la8b840* self);
};

struct UnkShipRecharge_la8b840 {
    UnkShipRechargeVtable_la8b840* vtable;
    u8 pad_0x8[0x28];
    UnkShipRechargeOwner_la8b840* owner;  // 0x30
    u8 pad_0x38[0x540 - 0x38];
    UnkSoundHandle loop_sound;  // 0x540
};

// The id's text as a string, counting the read.
static inline CRntString* NameString(const CStrId& id) {
    ++id.data->reads;
    return const_cast<CRntString*>(&id.data->String());
}

// After the base step, saves the game at the ship (the owner's name, "ST_SamusShip", with this component
// and its callback), then starts the save station's loop sound unless it is already playing.
void fn_7100a8b840(UnkShipRecharge_la8b840* self) {
    self->vtable->vf_0x2b0(self);
    {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkShipRechargeOwner_la8b840* owner = self->owner;
        CRntString* name = NameString(owner->name);
        CRntString tag("ST_SamusShip");
        fn_7100dfd174(manager, name, &tag, true, 1,
                      UnkSaveGamePair_b54{reinterpret_cast<u64>(self), reinterpret_cast<u64>(&fn_7100a8c670)});
    }
    if (IsPlaying(self->loop_sound))
        return;
    static CFilePathStrId s_path("props/savestation/savestation_loop.wav", true);
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.path = s_path;
    s_desc.field_0x38 = 0.6f;
    s_desc.field_0x3c = 500.0f;
    s_desc.field_0x40 = 3000.0f;
    s_desc.field_0x44 = 1.0f;
    s_desc.field_0x6c = true;
    s_desc.field_0x6d = true;
    s_desc.field_0x4c = 0.25f;
    self->loop_sound = fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->owner), &s_desc, &g_unk_7101d470e0,
                                     false, true, true);
}

struct UnkShipRechargeLink_la8bc44 {
    u8 pad_0x0[0x10];
    void* object;
};

struct UnkShipRecharge_la8bc44;
void fn_7100aa35ac(UnkShipRecharge_la8bc44* self);
void fn_7100aa35ec(UnkShipRecharge_la8bc44* self);
void fn_7100aa35cc(UnkShipRecharge_la8bc44* self);

struct UnkShipRecharge_la8bc44 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;  // +0x30
    u8 pad_0x38[0xb0];
    UnkShipRechargeLink_la8bc44* link;  // +0xe8
    u8 pad_0xf0[0x449];
    bool field_0x539;
    u8 pad_0x53a[0x6];
    UnkSoundHandle sound;  // +0x540
};
static_assert(__builtin_offsetof(UnkShipRecharge_la8bc44, field_0x539) == 0x539, "");
static_assert(__builtin_offsetof(UnkShipRecharge_la8bc44, sound) == 0x540, "");

// While linked and allowed to recharge, starts the recharge sound ("savestation_recharge.wav") on the
// owner entity unless it is already playing, then runs the three recharge steps.
void fn_7100a8bc44(UnkShipRecharge_la8bc44* self) {
    if (!self->link || !self->link->object)
        return;
    if (self->field_0x539 || fn_7100aa3330(reinterpret_cast<UnkLifeRecharge_b45a2*>(self)))
        return;
    if (!IsPlaying(self->sound)) {
        static CFilePathStrId s_path("props/savestation/savestation_recharge.wav", true);
        static UnkSoundDesc s_params;
        s_params.kind.field_0x0 = 0;
        s_params.path = s_path;
        s_params.field_0x38 = 0.6f;
        s_params.field_0x3c = 500.0f;
        s_params.field_0x40 = 5000.0f;
        s_params.field_0x44 = 1.0f;
        s_params.field_0x6c = true;
        s_params.field_0x6d = true;
        s_params.field_0x4c = 0.25f;
        self->sound = fn_7100d7c374(self->entity, &s_params, &g_unk_7101d470e0, false, true, true);
    }
    fn_7100aa35ac(self);
    fn_7100aa35ec(self);
    fn_7100aa35cc(self);
}

// The update. Unless the use was flagged as without a level (+0x53a), counts the use time (+0x53c) and,
// once the usable component allows it (fn_7100aa2948), starts the use animation's next part and, when the
// recharge music is no longer playing at the start (or the use took 10 s), calls the owner's
// "OnLevelChangeUseEnd" script and gives the user's input back. Then requests state 1 from the state
// machine at +0x100 ("none" 8) while the user moves sideways.

struct UnkScriptCaller;
struct UnkUsableVirt_A6_b62;
extern const u8 g_unk_710164b5d8[] __attribute__((visibility("hidden")));
struct UnkUsableComponent_A6_b62;
struct UnkInputComponent;
extern __attribute__((visibility("hidden"))) char g_unk_71727515d8[];

struct UnkSoundMgr;

// As src/game/CShipRechargeComponent/CShipRechargeComponent_Use_A5_b73.cpp declares them.
struct UnkShipRecharge_A5_b73;

struct UnkShipRechargeOwner_A5_b76 {
    u8 pad_0x0[0x1e0];
    UnkScriptCaller* script;
};

struct UnkShipRechargeName_A5_b76 {
    u8 pad_0x0[0x10];
    const char* name;
    u8 pad_0x18[0x44 - 0x18];
    s32 reads;
};

struct UnkShipRechargeUser_A5_b76 {
    u8 pad_0x0[0x30];
    UnkShipRechargeName_A5_b76* name;
    u8 pad_0x38[0x1d8 - 0x38];
    UnkInputComponent* input;
    u8 pad_0x1e0[0x220 - 0x1e0];
    UnkAnimationComponent* animation;
};

struct UnkShipRechargeLink_A5_b76 {
    u8 pad_0x0[0x10];
    UnkShipRechargeUser_A5_b76* user;
};

struct UnkShipRechargeKey_A5_b76 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkShipRecharge_A5_b76 {
    u8 pad_0x0[0x30];
    UnkShipRechargeOwner_A5_b76* owner;
    u8 pad_0x38[0xe8 - 0x38];
    UnkShipRechargeLink_A5_b76* link;
    u8 pad_0xf0[0x100 - 0xf0];
    UnkSmMachine_A5_b73 machine;
    u8 pad_0x1b0[0x53a - 0x1b0];
    bool no_level;
    u8 pad_0x53b[0x53c - 0x53b];
    f32 time;
};
static_assert(__builtin_offsetof(UnkShipRecharge_A5_b76, time) == 0x53c, "layout");

static inline UnkShipRecharge_A5_b73* Cast_A5_b76(UnkShipRecharge_A5_b76* self) {
    return reinterpret_cast<UnkShipRecharge_A5_b73*>(self);
}

typedef s32 UnkAliasS32_A5_b76;

static inline bool Playing_A5_b76(const UnkSoundRef_A5_b76& ref) {
    if (!ref.entry)
        return false;
    if (ref.entry->id != ref.id || ref.id == (u64)-1)
        return false;
    return ref.entry->playing != nullptr;
}

static inline UnkShipRechargeUser_A5_b76* User_A5_b76(UnkShipRecharge_A5_b76* self) {
    return self->link ? self->link->user : nullptr;
}

static inline const char* UserName_A5_b76(UnkShipRecharge_A5_b76* self) {
    if (UnkShipRechargeUser_A5_b76* user = User_A5_b76(self)) {
        ++*reinterpret_cast<UnkAliasS32_A5_b76*>(&user->name->reads);
        const char* name = user->name->name;
        return name ? name : "";
    }
    return "";
}

void CShipRechargeComponent_UpdateLevelChangeUse(UnkShipRecharge_A5_b76* self, f32 dt) {
    if (!self->no_level) {
        self->time += dt;
        static const CFilePathStrId s_music("streams/music/matad_jintojo_32728k.wav", true);
        UnkSoundRef_A5_b76 ref = fn_7100108aa4(g_unk_7101d4d820, &s_music, false);
        bool early = Playing_A5_b76(ref) && !(self->time >= 10.0f);
        if (fn_7100aa2948(reinterpret_cast<UnkUsableComponent_A6_b62*>(self))) {
            if (UnkShipRechargeUser_A5_b76* user = User_A5_b76(self)) {
                UnkAnimationComponent* animation = user->animation;
                u64 key = CUsableComponent_GetUserAnimationIdForKind(Cast_A5_b76(self), 6);
                if (fn_710117ae38(animation, (*reinterpret_cast<UnkShipRechargeKey_A5_b76**>(key))->hash, false)) {
                    u64 a = CUsableComponent_GetUserAnimationIdForKind(Cast_A5_b76(self), 1);
                    u64 b = fn_7100aa1888(Cast_A5_b76(self), 1);
                    if (fn_7100aa0b04(reinterpret_cast<UnkUsableVirt_A6_b62*>(self), a, b, g_unk_710164b5d8)) {
                        if (UnkInputComponent* input = self->link->user->input)
                            fn_7100872460(input, true, false, g_unk_71727515d8);
                    }
                }
            }
            if (!early) {
                if (UnkShipRechargeUser_A5_b76* user = User_A5_b76(self)) {
                    if (UnkInputComponent* input = user->input) {
                        if (fn_710087238c(input)) {
                            if (UnkScriptCaller* script = self->owner->script) {
                                fn_7100305088(script, "OnLevelChangeUseEnd", "s", UserName_A5_b76(self));
                                fn_7100872460(input, false, false, g_unk_71727515d8);
                            }
                        }
                    }
                }
            }
        }
    }
    UnkKraidCreatureVec move = fn_7100d7be08(User_A5_b76(self));
    if (__builtin_fabsf(move.x) > 0.005f)
        RequestState_A5_b73<15, 8>(&self->machine, 1);
}
