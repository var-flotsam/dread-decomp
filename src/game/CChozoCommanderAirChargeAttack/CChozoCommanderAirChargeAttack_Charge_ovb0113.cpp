// CChozoCommanderAirChargeAttack's unit (tools/tu_map.py, unit 643): whether the attack can start. Past the
// base check (fn_7100424b50), it can when there is no target (fn_7100d7c990) or the target's +0x208 object
// does not pass fn_71009e84e4, and cannot while the target has +0x210; otherwise only when the target is at
// least 160 away horizontally from the AI component's charge position (+0x15d1c, or +0x15d24 unless the
// actor's +0x220 component is in mode 4). Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAnimationComponent.h"
#include "game/CChozoCommanderAirAttack.h"
#include "game/UnkActor.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1728.h"
#include "game_s2/UnkParams.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

using base::reflection::UnkReflObject;

struct UnkTakumakuComponent;

struct UnkSetup;
struct UnkRequester;

struct UnkChargeAI_ovb0113 {
    u8 pad_0x0[0x15d1c];
    f32 field_0x15d1c;
    u8 pad_0x15d20[4];
    f32 field_0x15d24;
};

struct UnkChargeActor_ovb0113 {
    u8 pad_0x0[0x220];
    UnkTakumakuComponent* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    UnkReflObject* ai;  // 0x240
};

struct UnkChargeTarget_ovb0113 {
    u8 pad_0x0[0x44];
    f32 x;  // 0x44
    u8 pad_0x48[0x208 - 0x48];
    char* field_0x208;
    void* field_0x210;
};

struct UnkChargeAttack_ovb0113 {
    u8 pad_0x0[8];
    UnkChargeActor_ovb0113* actor;
};

bool fn_710042510c(UnkChargeAttack_ovb0113* self, void* request, u32 flag) {
    if (!fn_7100424b50(reinterpret_cast<UnkRequester*>(self), request, flag))
        return false;
    auto* target = reinterpret_cast<UnkChargeTarget_ovb0113*>(
        fn_7100d7c990(reinterpret_cast<UnkActor*>(self->actor)));
    if (!target)
        return true;
    if (target->field_0x210)
        return false;
    if (!target->field_0x208 || !fn_71009e84e4(target->field_0x208))
        return true;
    UnkReflObject* o = self->actor->ai;
    auto* ai = reinterpret_cast<UnkChargeAI_ovb0113*>(
        o && fn_710009d50c(o->vf_0x0(), fn_710044e708()) ? o : nullptr);
    f32 x = fn_7101178e6c(self->actor->field_0x220, 4) ? ai->field_0x15d1c : ai->field_0x15d24;
    return __builtin_fabsf(target->x - x) >= 160.0f;
}

// fn_7100425840: the charge's end offset (+0x5c, three floats): cleared, then, when the actor's actions
// (+0x220) have the charge timeline (fn_7101176e0c), the translation of the motion fn_71007142e8 sets up as
// that timeline reports it over its whole length (fn_7100b8b3bc), as fn_710042dbfc
// (src/game/CChozoCommanderHypersparkAttack/CChozoCommanderHypersparkAttack_Aim_ovb0023.cpp) does.
struct UnkStruct_7101176ea8;

struct UnkFlags_ovb0113 {
    u32 bits;
    UnkFlags_ovb0113() : bits(0) {}
    UnkFlags_ovb0113(u32 b) : bits(b) {}
    UnkFlags_ovb0113(const UnkFlags_ovb0113& o) : bits(o.bits) {}
};

inline const UnkFlags_ovb0113& UnkFlagsNone_ovb0113() {
    static UnkFlags_ovb0113 s_none asm("g_unk_71726a4340");
    return s_none;
}

inline const UnkFlags_ovb0113& UnkFlagsAll_ovb0113() {
    static UnkFlags_ovb0113 s_all asm("g_unk_71726a4330")(~UnkFlagsNone_ovb0113().bits & 7);
    return s_all;
}

struct UnkStruct_71007142e8 {
    f32 m[12];
    u8 pad_0x30[0x40];
};

struct UnkMotionTracks;
struct UnkTimeline {
    u8 pad_0x0[0x24];
    s32 frame_count;
    u8 pad_0x28[0x58];
    UnkMotionTracks* tracks;  // 0x80
};

void fn_71007142e8(UnkStruct_71007142e8* self);

struct UnkChargeOwner_ovb0113 {
    u8 pad_0x0[0x220];
    UnkStruct_7101176ea8* actions;  // 0x220
};

struct UnkChargeAttack2_ovb0113 {
    u8 pad_0x0[8];
    UnkChargeOwner_ovb0113* owner;
    u8 pad_0x10[0x5c - 0x10];
    f32 end_x;  // 0x5c
    f32 end_y;
    f32 end_z;
};

void fn_7100425840(UnkChargeAttack2_ovb0113* self) {
    self->end_x = 0.0f;
    self->end_y = 0.0f;
    self->end_z = 0.0f;
    UnkStruct_7101176ea8* actions = self->owner->actions;
    if (!actions)
        return;
    UnkTimeline* timeline =
        static_cast<UnkTimeline*>(fn_7101176e0c(actions, 0xe3779ee8dacd6c46ull, 0x137e114c0de4fb8dull));
    if (!timeline)
        return;
    UnkStruct_71007142e8 motion;
    fn_71007142e8(&motion);
    UnkMotionTracks* tracks = timeline->tracks;
    UnkFlags_ovb0113 flags = UnkFlagsAll_ovb0113();
    fn_7100b8b3bc(timeline, &motion, tracks, &flags.bits, 0.0f, timeline->frame_count);
    f32 x = motion.m[3];
    f32 y = motion.m[7];
    f32 z = motion.m[11];
    self->end_x = x;
    self->end_y = y;
    self->end_z = z;
}

// fn_7100425998: the charge's start. The actor's actions (+0x220) get a zero under the charge hash
// (refl_EmitU32, written out); the attack goes to state 2 (+0x58) with +0x14 cleared, and the actor's navigation object
// (+0x298) is handed a cleared parameter block of the vtable-0x7101a25b38 kind (fn_71006f4b0c), as
// src/game/CChozoCommanderLandingSlamAttack/CChozoCommanderLandingSlamAttack_End_A2_b72.cpp builds it.
using base::reflection::UnkReflValue;

extern void* g_unk_7101a25b38[];

struct UnkChargeParams_ovb0113 {
    void** vtable;
    u8 field_0x8[13];
    u8 pad_0x15[0x7];
    bool field_0x1c;
    u8 pad_0x1d[0x3];
    u64 field_0x20[2];
    bool field_0x30;
    u8 pad_0x31[0x3];
    u32 field_0x34;
    u8 pad_0x38[0x4];
    bool field_0x3c;
    u8 pad_0x3d;
    bool field_0x3e;
    u8 pad_0x3f;
    bool field_0x40;
    u8 pad_0x41[0x7];
    u64 field_0x48[2];
    bool field_0x58;
    u8 pad_0x59[0xf];
    bool field_0x68;
    u8 pad_0x69;
    bool field_0x6a;
    u8 pad_0x6b[0x5];
    UnkReflValue value;  // 0x70
    bool field_0x88;

    UnkChargeParams_ovb0113() {
        field_0x1c = false;
        vtable = g_unk_7101a25b38 + 2;
        __builtin_memset(field_0x8, 0, 13);
        field_0x3c = false;
        value = base::reflection::refl_EmptyValue();
        field_0x3e = false;
        field_0x40 = false;
        field_0x68 = false;
        field_0x6a = false;
        field_0x58 = false;
        field_0x88 = false;
        field_0x34 = 1;
        field_0x30 = true;
        __builtin_memset(field_0x48, 0, 16);
        __builtin_memset(field_0x20, 0, 16);
    }
    ~UnkChargeParams_ovb0113() {
        vtable = g_unk_7101a25b38 + 2;
        fn_71000a1c60(&value);
    }
};

struct UnkChargeOwner3_ovb0113 {
    u8 pad_0x0[0x220];
    void* actions;  // 0x220
    u8 pad_0x228[0x298 - 0x228];
    void* navigation;  // 0x298
};

struct UnkChargeAttack3_ovb0113 {
    u8 pad_0x0[8];
    UnkChargeOwner3_ovb0113* owner;
    u8 pad_0x10[4];
    bool field_0x14;
    u8 pad_0x15[0x58 - 0x15];
    u32 state;  // 0x58
};

void fn_7100425998(UnkChargeAttack3_ovb0113* self) {
    using namespace base::reflection;
    void* actions = self->owner->actions;
    UnkReflTypeIdArg value;
    value.id = 0;
    UnkReflTypeIdArg arg;
    arg.id = UnkReflTypeId<UnkReflTypeIdTag>::id();
    fn_7101176690(actions, 0x6fb3d416bd10ce27, 0xf4061ff25a2163d6, &value, &g_unk_710164b958,
                  &g_unk_7172769740, &arg);
    self->state = 2;
    self->field_0x14 = false;
    UnkChargeParams_ovb0113 params;
    fn_71006f4b0c(self->owner->navigation, &params, 0);
}
