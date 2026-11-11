// CQuarentineDoorComponent's unit (tools/tu_map.py): the door's update. While the flag at +0x2a1 is set
// and the owner's animation reaches the event 0xf4061ff25a2163d6 (fn_71011782ec), runs fn_71007df12c(1)
// and clears the flag. Then, when the player's shape is outside the box at +0x7c and inside the one
// at +0x8c (fn_710012b330, as the sensor door of src/game/UnkStateMachineSetA5_b61.cpp tests it), asks the
// state machine (+0xa0) for state 2 and clears the flag at +0x2a0. Names and layouts are placeholders.
#include "../CSensorDoorComponent/UnkDoorStateMachine_b58.h"
#include "game/CCharacterMovement.h"
#include "game/CQuarentineDoorComponent.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkGameManager.h"
#include "types.h"

struct UnkQuarentineDoor_b54;
bool fn_71011782ec(UnkAnimationComponent*, u64);

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);
struct UnkFlagged;
struct UnkShape;
struct UnkBox2D;
struct UnkContactPointA5_b61 {
    u8 data[0x2c];
};
template <u32 N>
struct UnkContactBuffer {
    u32 count;
    UnkContactPointA5_b61 points[N];
};
bool fn_710012b330(const UnkBox2D* a, const void* b, UnkContactBuffer<4>* out, bool flag);

struct UnkQuarentineShape_ovb0127 {
    virtual void vf_0x0();
    u8 pad_0x8[0x40 - 0x8];
    u8 geometry[0x20];
    bool dirty;
    void Update() {
        typedef void (*Fn)(UnkQuarentineShape_ovb0127*);
        (*reinterpret_cast<Fn**>(this))[0x98 / 8](this);
    }
};

struct UnkQuarentinePlayer_ovb0127 {
    u8 pad_0x0[0x200];
    UnkFlagged* field_0x200;
};

struct UnkQuarentineUpdateOwner_ovb0127 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;  // 0x220
};

struct UnkQuarentineUpdateDoor_ovb0127 {
    u8 pad_0x0[0x30];
    UnkQuarentineUpdateOwner_ovb0127* owner;  // 0x30
    u8 pad_0x38[0x44];
    u8 box_outer[0x10];  // 0x7c
    u8 box_inner[0x10];  // 0x8c
    u8 pad_0x9c[0x4];
    UnkDoorStateMachine_b58 state_machine;  // 0xa0
    u8 pad_0x150[0x2a0 - 0x150];
    bool field_0x2a0;
    bool field_0x2a1;
};

static inline bool Touches(const u8* box, UnkQuarentineShape_ovb0127* shape, UnkContactBuffer<4>& contacts) {
    contacts.count = 0;
    if (shape->dirty)
        shape->Update();
    return fn_710012b330(reinterpret_cast<const UnkBox2D*>(box), shape->geometry, &contacts, true);
}

// Taken by reference: the flag is loaded ahead of the animation test, as in the original.
static inline bool Waiting(UnkQuarentineUpdateDoor_ovb0127& door, UnkAnimationComponent* animation) {
    return animation && door.field_0x2a1;
}

void fn_71007df4e4(UnkQuarentineUpdateDoor_ovb0127* self) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (Waiting(*self, animation) && fn_71011782ec(animation, 0xf4061ff25a2163d6)) {
        fn_71007df12c(reinterpret_cast<UnkQuarentineDoor_b54*>(self), true);
        self->field_0x2a1 = false;
    }
    auto* player = reinterpret_cast<UnkQuarentinePlayer_ovb0127*>(fn_7100dec788(g_unk_7101d47310));
    if (!player || !player->field_0x200)
        return;
    auto* shape = reinterpret_cast<UnkQuarentineShape_ovb0127*>(fn_7100976588(player->field_0x200));
    if (!shape)
        return;
    UnkContactBuffer<4> outer_contacts;
    bool outer = Touches(self->box_outer, shape, outer_contacts);
    UnkContactBuffer<4> inner_contacts;
    bool inner = Touches(self->box_inner, shape, inner_contacts);
    if (outer || !inner)
        return;
    self->field_0x2a0 = false;
    RequestState_b58(&self->state_machine, 2);
}
