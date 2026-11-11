// CShipRechargeComponent unit (tools/tu_map.py, unit 1598): the update. Unless the use was flagged as
// without a level (+0x53a), counts the use time (+0x53c) and, once the usable component allows it
// (fn_7100aa2948), starts the use animation's next part and, when the recharge music is no longer
// playing at the start (or the use took 10 s), calls the owner's "OnLevelChangeUseEnd" script and gives the
// user's input back. Then requests state 1 from the state machine at +0x100 ("none" 8) while the user
// moves sideways. Names and layouts are placeholders.
#include "../UnkSmRequest_A5_b73.h"
#include "base/global/CFilePathStrId.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CInputComponent.h"
#include "game/CUsableComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU2523.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkScriptCaller;
struct UnkUsableVirt_A6_b62;
extern const u8 g_unk_710164b5d8[] __attribute__((visibility("hidden")));
struct UnkUsableComponent_A6_b62;
struct UnkInputComponent;
extern __attribute__((visibility("hidden"))) char g_unk_71727515d8[];

// As src/game/CSluggerAcidBallMovementComponent/CSluggerAcidBallMovementComponent_b38.cpp declares it.
struct UnkKraidCreatureVec {
    f32 x;
    f32 y;
    f32 z;
    UnkKraidCreatureVec(const UnkKraidCreatureVec& other) : x(other.x), y(other.y), z(other.z) {}
};

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
struct UnkSoundEntry_A5_b76 {
    void* playing;
    u64 id;
};
struct UnkSoundRef_A5_b76 {
    UnkSoundEntry_A5_b76* entry;
    u64 id;
};
UnkSoundRef_A5_b76 fn_7100108aa4(UnkSoundMgr* self, const CFilePathStrId* path, bool flag);

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

// The count is bumped through an aliasing type: the original reloads the name pointer after it.
typedef s32 __attribute__((may_alias)) UnkAliasS32_A5_b76;

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

void fn_7100a8bfc8(UnkShipRecharge_A5_b76* self, f32 dt) {
    if (!self->no_level) {
        self->time += dt;
        static const CFilePathStrId s_music("streams/music/matad_jintojo_32728k.wav", true);
        UnkSoundRef_A5_b76 ref = fn_7100108aa4(g_unk_7101d4d820, &s_music, false);
        bool early = Playing_A5_b76(ref) && !(self->time >= 10.0f);
        if (fn_7100aa2948(reinterpret_cast<UnkUsableComponent_A6_b62*>(self))) {
            if (UnkShipRechargeUser_A5_b76* user = User_A5_b76(self)) {
                UnkAnimationComponent* animation = user->animation;
                u64 key = fn_7100aa1f50(Cast_A5_b76(self), 6);
                if (fn_710117ae38(animation, (*reinterpret_cast<UnkShipRechargeKey_A5_b76**>(key))->hash, false)) {
                    u64 a = fn_7100aa1f50(Cast_A5_b76(self), 1);
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
