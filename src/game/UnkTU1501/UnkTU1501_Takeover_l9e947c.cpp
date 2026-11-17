// Unit 1501 (tools/tu_map.py, CSamusMovement): hands the movement over to another one: the base copy
// (fn_71010da468), the morph ball's flag on the other movement, a transition request to the air target,
// then clears the pending vectors, stops two timeline effects and resets the flags. Names and layouts
// are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "game/UnkTimelineComponent.h"
#include "base/global/CStrId.h"
#include "types.h"

extern void* g_unk_7101a87820[];

struct UnkMoveCopy_b0204;
struct UnkMoveSource_b0204;
void fn_71010da468(UnkMoveCopy_b0204* self, UnkMoveSource_b0204* other);

struct UnkMorphBallMovement;
void fn_7100947298(UnkMorphBallMovement* self, bool enable, f32 value);

struct UnkAirTarget_B71a6;
struct UnkAirRequest_B71a6;

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_l9b0d24 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_l9b0d24 {
    s64 id;
    UnkDesc16_l9b0d24 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_l9b0d24(UnkDesc16_l9b0d24 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_l9b0d24()
        : UnkMoveTransition_l9b0d24(*reinterpret_cast<const UnkDesc16_l9b0d24*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_l9b0d24) == 0x30, "layout");

typedef u64 UnkU64Align4_l9e947c __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8) with this request's vtable.
struct UnkTakeoverParams_l9e947c {
    void* vtable;
    UnkMoveTransition_l9b0d24 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_l9e947c field_0x3c;
    u32 field_0x44;

    UnkTakeoverParams_l9e947c() : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = reinterpret_cast<char*>(g_unk_7101a87820) + 0x10;
    }
};
static_assert(sizeof(UnkTakeoverParams_l9e947c) == 0x48, "layout");

// The Samus settings fields read here.
struct UnkTakeoverSettings_l9e947c {
    u8 pad_0x0[0x5c];
    bool field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
};

struct UnkTakeoverState_l9e947c {
    u8 pad_0x0[0x10];
    s32 field_0x10;
    u8 pad_0x14[0x1a0];
    f32 field_0x1b4;
    u8 pad_0x1b8[0x3c];
    bool field_0x1f4;
};

struct UnkTakeoverOther_l9e947c {
    u8 pad_0x0[0x210];
    UnkMorphBallMovement* morph;  // 0x210
};

struct UnkVec3_l9e947c {
    f32 x, y, z;

    UnkVec3_l9e947c() : x(0.0f), y(0.0f), z(0.0f) {}
};


struct UnkTakeover_l9e947c;
struct UnkTakeoverVtable_l9e947c {
    void* pad_0x0[0x77];
    bool (*vf_0x3b8)(UnkTakeover_l9e947c* self);
};

struct UnkTakeover_l9e947c {
    UnkTakeoverVtable_l9e947c* vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x22cc];
    u32 field_0x2304;
    u8 pad_0x2308[0x9d4];
    UnkVec3_l9e947c field_0x2cdc;
    UnkVec3_l9e947c field_0x2ce8;
    u8 pad_0x2cf4[4];
    u8 air_target[0x10];  // 0x2cf8
    UnkTakeoverState_l9e947c* field_0x2d08;
    u8 pad_0x2d10[0x2494];
    bool field_0x51a4;
    bool field_0x51a5;
};
static_assert(__builtin_offsetof(UnkTakeover_l9e947c, field_0x2304) == 0x2304, "layout");
static_assert(__builtin_offsetof(UnkTakeover_l9e947c, field_0x2cdc) == 0x2cdc, "layout");
static_assert(__builtin_offsetof(UnkTakeover_l9e947c, field_0x2d08) == 0x2d08, "layout");
static_assert(__builtin_offsetof(UnkTakeover_l9e947c, field_0x51a4) == 0x51a4, "layout");

void fn_71009e947c(UnkTakeover_l9e947c* self, UnkTakeoverOther_l9e947c* other) {
    fn_71010da468(reinterpret_cast<UnkMoveCopy_b0204*>(self), reinterpret_cast<UnkMoveSource_b0204*>(other));
    const UnkTakeoverSettings_l9e947c& settings =
        *reinterpret_cast<const UnkTakeoverSettings_l9e947c*>(&g_unk_7172740de0);
    if (other->morph) {
        UnkTakeoverState_l9e947c* state = self->field_0x2d08;
        if (state && state->field_0x10 == 3) {
            if (state->field_0x1b4 >= 0.0f && state->field_0x1f4)
                fn_7100947298(other->morph, true, settings.field_0x60);
        } else if (!self->field_0x51a5) {
            bool active = self->vtable->vf_0x3b8(self);
            if (settings.field_0x5c && active)
                fn_7100947298(other->morph, true, settings.field_0x64);
        }
    }
    UnkTakeoverParams_l9e947c params;
    u64 key;
    fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->air_target),
                  reinterpret_cast<UnkAirRequest_B71a6*>(&params));
    self->field_0x2cdc = UnkVec3_l9e947c();
    self->field_0x2ce8 = UnkVec3_l9e947c();
    UnkActor* owner = self->owner;
    key = fn_7100001570("stop_fx_notransition", 20);
    fn_7100d7c828(owner, &key, false);
    owner = self->owner;
    key = fn_7100001570("stop_materialtransition_notransition", 36);
    fn_7100d7c828(owner, &key, false);
    self->field_0x51a4 = false;
    self->field_0x51a5 = false;
    self->field_0x2304 = 0;
}
