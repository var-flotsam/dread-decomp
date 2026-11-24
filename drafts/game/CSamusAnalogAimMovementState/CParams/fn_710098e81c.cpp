// CSamusAnalogAimMovementState::CParams's unit (tools/tu_map.py unit 1456): the analog aim state's update.
// After the base update, when the movement allows it and the camera's controller is a
// CAutofocusAimCameraCtrl it asks for another movement state (the movement's track request, or a fixed
// one); then, while the movement moves, it runs four steps on its state of kind 1 as long as the target
// stays in state 15. Names and layouts are placeholders.
#include "../../../../src/game_s2/UnkSpitSend.h"
#include "base/core/CBaseObject.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "game/CAbilityGhostAura.h"
#include "game/CCameraComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "game/UnkTU1455.h"
#include "game/UnkTU1501.h"
#include "game/UnkTU1730.h"
#include "types.h"

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

struct UnkDesc16_ovb0004 {
    u64 a, b;
};

// A movement transition: an id (-1 for none), the two descriptors of UnkSpitSend.h and a mask.
struct UnkMoveTransition_ovb0004 {
    s64 id;
    UnkDesc16_ovb0004 desc;
    UnkTrackBlend blend;
    u32 field_0x24;
    u32 mask;

    UnkMoveTransition_ovb0004(UnkDesc16_ovb0004 d, UnkTrackBlend b, u32 m)
        : id(-1), desc(d), blend(b), field_0x24(0), mask(m) {}
    UnkMoveTransition_ovb0004()
        : UnkMoveTransition_ovb0004(*reinterpret_cast<const UnkDesc16_ovb0004*>(&g_unk_710164b958),
                                    *reinterpret_cast<const UnkTrackBlend*>(&g_unk_7172769740),
                                    UnkAll_SpitSend().value) {}
};
static_assert(sizeof(UnkMoveTransition_ovb0004) == 0x30, "layout");

// A 64-bit word at a 4-byte aligned offset.
typedef u64 UnkU64Align4_ovb0004 __attribute__((aligned(4)));

// CSamusMovementState::CParams (vtable g_unk_7101a22cb8), given the vtable of the state it asks for.
struct UnkAimParams_l98e81c {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    explicit UnkAimParams_l98e81c(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkAimParams_l98e81c) == 0x48, "layout");

// fn_71009767f4's result (src/game/CCharacterMovement/CCharacterMovement_Movement_ovb0021.cpp).
struct UnkTrackTime_ovb0021 {
    u64 key;
    u64 time;
};
struct UnkTrackBlend_ovb0021 {
    u32 a, b, c;
};

struct UnkTrackParams_ovb0021 {
    s64 id;
    UnkTrackTime_ovb0021 time;
    UnkTrackBlend_ovb0021 blend;
    u32 flags;
    u32 mask;

    __attribute__((always_inline)) UnkTrackParams_ovb0021(UnkTrackTime_ovb0021 time_, UnkTrackBlend_ovb0021 blend_, u32 mask_)
        : id(-1), time(time_), blend(blend_), flags(0), mask(mask_) {}
    __attribute__((always_inline)) UnkTrackParams_ovb0021()
        : UnkTrackParams_ovb0021(reinterpret_cast<const UnkTrackTime_ovb0021&>(g_unk_710164b958),
                                 reinterpret_cast<const UnkTrackBlend_ovb0021&>(g_unk_7172769740),
                                 base::reflection::refl_GetEnumNot8<0x7101d57cb8, 0x7101d57cc8>()) {}
};

struct UnkTrackMovement_ovb0021;
UnkTrackParams_ovb0021 fn_71009767f4(UnkTrackMovement_ovb0021* self, bool forced);

// fn_71009b0580's parameter block (src/game/CSamusMovementState/CSamusMovementState_b42.cpp).
struct UnkParamHead_b42 {
    u32 id[2];
    u32 words[7];
};

struct UnkParamBlock_b42 {
    UnkParamHead_b42 head;
    u32 field_0x24;
    u32 field_0x28;
};

struct UnkParamHolder_b42 {
    u8 pad_0x0[0x8];
    UnkParamBlock_b42 block;  // 0x8
};

void CSamusMovementState_CopyBlockUnlessIdUnset(UnkParamHolder_b42* self, const UnkParamBlock_b42* block);

struct UnkAimMoveState_l98e81c {
    void* vtable;
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
};

#pragma pack(push, 4)
struct UnkAimMovement_l98e81c {
    u8 pad_0x0[0xf9];
    bool moving;  // 0xf9
    u8 pad_0xfa[0x2d18 - 0xfa];
    u32 state_count;
    UnkAimMoveState_l98e81c* states[1];  // 0x2d1c, 4-byte aligned
};
#pragma pack(pop)

struct UnkAimTarget_l98e81c {
    u8 pad_0x0[0xa4];
    s32 state;  // 0xa4
};

struct UnkAimOwner_l98e81c {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkAimState_l98e81c {
    void* vtable;
    UnkAimTarget_l98e81c* target;  // 0x8
    u8 pad_0x10[0x20];
    UnkAimOwner_l98e81c* owner;  // 0x30
    UnkAimMovement_l98e81c* movement;  // 0x38
};

void fn_71009b0d24(void* object, void* arg);
void fn_710098996c(UnkAimMoveState_l98e81c* state, void* arg);
void fn_710098eb0c(UnkAimState_l98e81c* self);
void fn_7100989ba4(UnkAimMoveState_l98e81c* state, void* arg);
void fn_710098a660(UnkAimMoveState_l98e81c* state, void* arg);

static inline UnkAimMoveState_l98e81c* FindState(UnkAimMovement_l98e81c* movement, s32 kind) {
    u64 size = (u64)movement->state_count * sizeof(UnkAimMoveState_l98e81c*);
    UnkAimMoveState_l98e81c** it = movement->states;
    for (; size != 0; ++it, size -= sizeof(*it)) {
        if ((*it)->kind == kind)
            return *it;
    }
    return nullptr;
}

void fn_710098e81c(UnkAimState_l98e81c* self, void* arg) {
    fn_71009b0d24(self, arg);
    if (fn_71009e84e4(reinterpret_cast<char*>(self->movement)) && self->owner && self->owner->field_0x220) {
        base::reflection::UnkReflObject* ctrl = fn_71010e37f4(fn_7100defcec(g_unk_7101d47310));
        if (ctrl && fn_7100023d00(reinterpret_cast<const UnkCameraCtrl*>(ctrl), fn_7100720464())) {
            UnkAimParams_l98e81c params(reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10);
            UnkTrackParams_ovb0021 track =
                fn_71009767f4(reinterpret_cast<UnkTrackMovement_ovb0021*>(self->movement), false);
            if (track.id != -1) {
                CSamusMovementState_CopyBlockUnlessIdUnset(reinterpret_cast<UnkParamHolder_b42*>(&params),
                              reinterpret_cast<const UnkParamBlock_b42*>(&track));
            } else {
                // The id table's string id at +0xc98, which UnkGameManagerIdTable does not name yet.
                const base::global::CStrId& id =
                    *reinterpret_cast<const base::global::CStrId*>(GameManagerIdTable().pad_0x0 + 0xc98);
                params.transition.id = id.data->Key();
                params.transition.desc.a = -1;
                params.transition.desc.b = 0x13df5c28full;
            }
            fn_710098ae04(reinterpret_cast<UnkAirTarget_B71a6*>(self->target),
                          reinterpret_cast<UnkAirRequest_B71a6*>(&params));
        }
    }
    UnkAimMovement_l98e81c* movement = self->movement;
    if (!movement->moving)
        return;
    UnkAimMoveState_l98e81c* state = FindState(movement, 1);
    if (self->target->state != 15)
        return;
    fn_710098996c(state, arg);
    if (self->target->state != 15)
        return;
    fn_710098eb0c(self);
    if (self->target->state != 15)
        return;
    fn_7100989ba4(state, arg);
    if (self->target->state != 15)
        return;
    fn_710098a660(state, arg);
}
