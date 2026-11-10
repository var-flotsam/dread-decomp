// CQuarentineDoorComponent's unit (tools/tu_map.py): fn_71007df218 does nothing while the flag at
// +0x7a or +0x2a0 is set; it sends the owner's (+0x30) sub-object at +0x220, if any, the hash of
// "close" with mode 0x200 and a local block {-1, 3.0f, 1} (as fn_71007defe0 in
// CQuarentineDoorComponent_Send_b54.cpp sends "opened"), refreshes the owner's collision (+0x2a0,
// fn_71008615b4), then fn_71007df12c(0). Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/global/CName.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

struct UnkQuarentineDoor_b54;
void fn_71007df12c(UnkQuarentineDoor_b54* self, bool flag);
void fn_71008615b4(UnkCollisionContainer*);

struct UnkQuarentineOwner_ovb0127 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

// The owner as its collision is read (a second view: through the first, the compiler would drop the
// owner check before the send).
struct UnkQuarentineBody_ovb0127 {
    u8 pad_0x0[0x2a0];
    UnkCollisionContainer* collision;  // 0x2a0
};

struct UnkQuarentineDoor_ovb0127 {
    u8 pad_0x0[0x30];
    union {
        UnkQuarentineOwner_ovb0127* owner;
        UnkQuarentineBody_ovb0127* body;
    };
    u8 pad_0x38[0x42];
    bool field_0x7a;
    u8 pad_0x7b[0x2a0 - 0x7b];
    bool field_0x2a0;
    bool field_0x2a1;
};

struct UnkQuarentineSendParams_ovb0127 {
    s64 field_0x0;
    f32 field_0x8;
    u32 field_0xc;
};

void fn_71007df218(UnkQuarentineDoor_ovb0127* self) {
    if (self->field_0x7a || self->field_0x2a0)
        return;
    if (self->owner) {
        if (void* target = self->owner->field_0x220) {
            u64 hash = base::global::CName("close").hash;
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0x200;
            UnkQuarentineSendParams_ovb0127 params;
            params.field_0x0 = -1;
            params.field_0x8 = 3.0f;
            params.field_0xc = 1;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, hash, 0xf4061ff25a2163d6, &mode, &params, &g_unk_7172769740, &mask);
        }
    }
    if (auto* collision = self->body->collision)
        fn_71008615b4(collision);
    fn_71007df12c(reinterpret_cast<UnkQuarentineDoor_b54*>(self), false);
}
