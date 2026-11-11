// CSamusFloorSlideMovementState unit (tools/tu_map.py): picks a hash by the player's (+0x38) state
// (fn_71010d1598, then fn_71010d15b4), sends it to the owner's (+0x30) sub-object at +0x220 with mode 0
// (UnkSpitSend, src/game_s2/UnkSpitSend.h, written out: its locals keep their own slots), resets the
// fields at +0x198..+0x1c4, calls virtual slot 3 of the object at +0x158 of the collision object named
// g_unk_717273b7c0 (fn_71008640a4 on the owner's +0x2a0) and requests state 3 from the transitions at
// +0x48 (fn_7100997128). Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "game/CSamusFloorSlideMovementState/CParams.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkGroundPlayer_b52;
bool fn_71010d1598(UnkGroundPlayer_b52*);
bool fn_71010d15b4(UnkGroundPlayer_b52*);
struct UnkMovementTransitions;

extern const base::global::CStrId g_unk_717273b7c0 __attribute__((visibility("hidden")));

struct UnkFloorSlideFx_b57 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};

struct UnkFloorSlideObject_b57 {
    u8 pad_0x0[0x158];
    UnkFloorSlideFx_b57* field_0x158;
};

struct UnkFloorSlideOwner_b57 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* collision;
};

struct UnkFloorSlidePair_b57 {
    u32 a;
    u32 b;
};

struct UnkFloorSlideState_b57 {
    u8 pad_0x0[0x30];
    UnkFloorSlideOwner_b57* owner;
    UnkGroundPlayer_b52* player;
    u8 pad_0x40[0x8];
    u8 transitions[0x198 - 0x48];
    u32 field_0x198;
    UnkFloorSlidePair_b57 field_0x19c;
    u8 pad_0x1a4[0x1b4 - 0x1a4];
    f32 field_0x1b4;
    u8 pad_0x1b8[0x1c4 - 0x1b8];
    bool field_0x1c4;
};

void fn_7100996c10(UnkFloorSlideState_b57* self) {
    u64 hash;
    if (fn_71010d1598(self->player))
        hash = 0x20ee4036297fa79b;
    else if (fn_71010d15b4(self->player))
        hash = 0x135b897efe508968;
    else
        hash = 0xa2829c68ac092d1c;
    void* target = self->owner->field_0x220;
    base::reflection::UnkReflTypeIdArg mode;
    mode.id = 0;
    base::reflection::UnkReflTypeIdArg mask;
    mask.id = UnkAll_SpitSend().value;
    fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    self->field_0x198 = 1;
    self->field_0x19c = UnkFloorSlidePair_b57();
    self->field_0x1b4 = -1.0f;
    self->field_0x1c4 = false;
    auto* object = reinterpret_cast<UnkFloorSlideObject_b57*>(fn_71008640a4(self->owner->collision, &g_unk_717273b7c0));
    if (object && object->field_0x158)
        object->field_0x158->vf_0x18();
    u32 next = 3;
    fn_7100997128(reinterpret_cast<UnkMovementTransitions*>(self->transitions), &next);
}
