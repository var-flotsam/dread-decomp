// CAbilityShinespark's unit (tools/tu_map.py): the shinespark's start step. After CAbility's, hands the
// player's movement the actor's collision state, hands it the movement state for the sub-state (+0x6b0) when
// that is 4 or 5, resets the model colour of the player and of the camera's actor, runs the cool shinespark
// request, restarts the fade and clears the owner's name, and switches the state machine at +0x6a0 to state 1.
// Names and layouts are placeholders.
#include "../../../src/game_s2/UnkSpitSend.h"
#include "../../../src/game/UnkSmRequest_A5_b73.h"
#include "base/global/CStrId.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbility.h"
#include "game/CAbilityGhostAura.h"
#include "game/CSamusModelUpdaterComponent.h"
#include "game/UnkCollisionRecord.h"
#include "game/UnkDesc16_ovb0004.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU1730.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

struct UnkAccessTarget_la70e48;
struct UnkCoolShinesparkMask_ovb0137;
void fn_710032a3b4(UnkReflObject* ability, const UnkCoolShinesparkMask_ovb0137* allowed_a, const UnkCoolShinesparkMask_ovb0137* allowed_b, bool flag);

extern CStrId g_unk_71726b09a8 __attribute__((visibility("hidden")));
extern const u8 g_unk_7101648f08[] __attribute__((visibility("hidden")));
extern const u8 g_unk_7101648f18[] __attribute__((visibility("hidden")));

// A flag set as the ability takes it: the bits, 8-byte aligned.
struct alignas(8) UnkCoolShinesparkMask_ovb0137 {
    u32 bits;
    u8 pad_0x4[0x4];
};

struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};
static_assert(sizeof(UnkTrackBlend) == 0xc, "UnkTrackBlend layout");

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
struct UnkLeaveParams_l9968ec {
    void* vtable;
    UnkMoveTransition_ovb0004 transition;  // 0x8
    u32 field_0x38;
    UnkU64Align4_ovb0004 field_0x3c;
    u32 field_0x44;

    explicit UnkLeaveParams_l9968ec(void* state_vtable)
        : vtable(reinterpret_cast<char*>(g_unk_7101a22cb8) + 0x10), transition() {
        field_0x38 = 3;
        field_0x3c = 0;
        field_0x44 = 0;
        vtable = state_vtable;
    }
};
static_assert(sizeof(UnkLeaveParams_l9968ec) == 0x48, "layout");

struct UnkShinesparkMovement_l32995c;
struct UnkShinesparkMovementVtable_l32995c {
    void* pad_0x0[0x2a8 / 8];
    void* (*vf_0x2a8)(UnkShinesparkMovement_l32995c* self);
    void (*vf_0x2b0)(UnkShinesparkMovement_l32995c* self, void* value);
};

struct UnkShinesparkMovement_l32995c {
    UnkShinesparkMovementVtable_l32995c* vtable;
    u8 pad_0x8[0xf9 - 0x8];
    bool field_0xf9;
};

struct UnkShinesparkBody_l32995c;
struct UnkShinesparkBodyVtable_l32995c {
    void* pad_0x0[0x200 / 8];
    bool (*vf_0x200)(UnkShinesparkBody_l32995c* self, const f32* pos, void* a, u32 b, void* c, u32 d);
};

struct UnkShinesparkBody_l32995c {
    UnkShinesparkBodyVtable_l32995c* vtable;
    u8 pad_0x8[0x3cc - 0x8];
    u32 field_0x3cc;
};

struct UnkShinesparkActor_l32995c;
struct UnkShinesparkActorVtable_l32995c {
    void* pad_0x0[0xd0 / 8];
    void (*vf_0xd0)(UnkShinesparkActor_l32995c* self, const f32* pos, bool flag);
};

struct UnkShinesparkActor_l32995c {
    UnkShinesparkActorVtable_l32995c* vtable;
    u8 pad_0x8[0x44 - 0x8];
    f32 field_0x44;
    u8 pad_0x48[0x54 - 0x48];
    f32 field_0x54;
    u8 pad_0x58[0x64 - 0x58];
    f32 field_0x64;
    u8 pad_0x68[0x1f8 - 0x68];
    UnkShinesparkBody_l32995c* body;  // 0x1f8
    u8 pad_0x200[0x208 - 0x200];
    UnkShinesparkMovement_l32995c* movement;  // 0x208
    u8 pad_0x210[0x2a0 - 0x210];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkShinesparkLink_l32995c {
    u8 pad_0x0[0x30];
    UnkShinesparkActor_l32995c* actor;  // 0x30
    u8 pad_0x38[0xd2 - 0x38];
    bool field_0xd2;
};

struct UnkShinesparkSelf_l32995c;
struct UnkShinesparkVtable_l32995c {
    void* pad_0x0[0xa0 / 8];
    void (*vf_0xa0)(UnkShinesparkSelf_l32995c* self, bool flag);
};
void fn_7100329f74(UnkShinesparkSelf_l32995c* self);

struct UnkShinesparkSelf_l32995c {
    UnkShinesparkVtable_l32995c* vtable;
    u8 pad_0x8[0xc0 - 0x8];
    UnkShinesparkLink_l32995c* link;  // 0xc0
    u8 pad_0xc8[0xdc - 0xc8];
    bool field_0xdc;
    bool field_0xdd;
    u8 pad_0xde[0x410 - 0xde];
    u32 field_0x410;
    u8 pad_0x414[0x441 - 0x414];
    bool field_0x441;
    u8 pad_0x442[0x448 - 0x442];
    CStrId field_0x448;
    u8 pad_0x450[0x6a0 - 0x450];
    UnkSmMachine_A5_b73 machine;  // 0x6a0
};
static_assert(__builtin_offsetof(UnkShinesparkSelf_l32995c, machine) == 0x6a0, "layout");
static_assert(__builtin_offsetof(UnkShinesparkSelf_l32995c, field_0x448) == 0x448, "layout");

struct UnkModelHolder_l32995c {
    u8 pad_0x0[0x10];
    struct UnkModelActor_l32995c* actor;  // 0x10
};

struct UnkModelActor_l32995c {
    u8 pad_0x0[0x228];
    UnkReflObject* model_updater;  // 0x228
};

struct UnkModelManager_l32995c {
    u8 pad_0x0[0x29a0];
    UnkModelHolder_l32995c* holder;  // 0x29a0
};

static inline void ResetModelColour(UnkReflObject* comp) {
    if (comp && fn_710009d50c(comp->vf_0x0(), fn_7100939af4()))
        fn_710093adc0(comp, g_unk_7101648f08, g_unk_7101648f18, 0.2f);
}

void fn_710032995c(UnkShinesparkSelf_l32995c* self) {
    fn_71002fa068(reinterpret_cast<UnkSmLockon_A1_b64*>(self));
    UnkShinesparkMovement_l32995c* movement = self->link->actor->movement;
    if (movement) {
        void* value = movement->vtable->vf_0x2a8(movement);
        movement->vtable->vf_0x2b0(movement, value);
        if (!self->machine.current || self->machine.current->id != 5) {
            UnkShinesparkActor_l32995c* actor = self->link->actor;
            UnkShinesparkBody_l32995c* body = actor->body;
            f32 x = actor->field_0x44;
            f32 y = actor->field_0x54;
            f32 z = actor->field_0x64;
            f32 position[3] = {x, y, z};
            UnkShinesparkBodyVtable_l32995c* body_vtable = body->vtable;
            u32 body_id = body->field_0x3cc;
            if (body_vtable->vf_0x200(body, position, nullptr, body_id, nullptr, 3)) {
                UnkShinesparkActor_l32995c* target = self->link->actor;
                target->vtable->vf_0xd0(target, position, true);
            }
        }
    }
    if (self->machine.current) {
        switch (self->machine.current->id) {
        case 5:
        case 4:
            if (movement) {
                if (movement->field_0xf9) {
                    if (UnkCollisionObject* object = fn_71008640a4(self->link->actor->collision, &g_unk_71726b09a8))
                        movement->vtable->vf_0x2b0(movement, object);
                    UnkLeaveParams_l9968ec params(g_unk_7101a22ce8 + 0x10);
                    fn_710031433c(reinterpret_cast<UnkAccessTarget_la70e48*>(movement), &params);
                } else {
                    UnkLeaveParams_l9968ec params(reinterpret_cast<char*>(g_unk_7101a22d18) + 0x10);
                    fn_710031433c(reinterpret_cast<UnkAccessTarget_la70e48*>(movement), &params);
                }
            }
            break;
        }
    }
    if (self->field_0xdd && (!self->field_0xdc || !self->link->field_0xd2)) {
        fn_7100329f74(self);
        self->vtable->vf_0xa0(self, true);
    }
    RequestState_A5_b73<15, 6>(&self->machine, 1);
    ResetModelColour(reinterpret_cast<UnkModelActor_l32995c*>(fn_7100defcd4(g_unk_7101d47310))->model_updater);
    ResetModelColour(reinterpret_cast<UnkModelManager_l32995c*>(g_unk_7101d47310)->holder->actor->model_updater);
    if (self->field_0x441) {
        UnkCoolShinesparkMask_ovb0137 mask_a;
        mask_a.bits = base::reflection::refl_GetEnumZero<0x71726b0940>();
        UnkCoolShinesparkMask_ovb0137 mask_b;
        mask_b.bits = base::reflection::refl_GetEnumZero<0x71726b0950>();
        fn_710032a3b4(reinterpret_cast<UnkReflObject*>(self), &mask_a, &mask_b, true);
        self->field_0x410 = 0;
        fn_7100df6c78(g_unk_7101d47310, 1.0f, 0.2f);
        self->field_0x441 = false;
        self->field_0x448 = CStrId::Empty();
    }
}
