// CGooshockerAIComponent's unit (tools/tu_map.py unit 854): the update (fn_710059e384). After the base
// update, the shock timer (+0x148c8) runs out by waking the owner's part, fading the shock sound and, with
// no charges left, stopping (fn_710059aa2c); the second timer (+0x148e4) sets the "on" timeline unless the
// flag at +0x148e8 defers it; a pending move (+0x148e1) goes to the owner's AI (fn_71010d08d4). Names and
// layouts are placeholders.
#include "base/snd/UnkSound.h"
#include "game/CElectrifyingAreaComponent.h"
#include "game/CGooplotAIComponent.h"
#include "game/CGooshockerAIComponent.h"
#include "game/CMovementComponent.h"
#include "game/UnkActor.h"
#include "types.h"

struct UnkGooshockerState;
struct UnkGooplotAI_b55;
struct UnkGooplotOwner_b55;
struct UnkMovement_71010d08d4;
struct UnkMovementVec_71010d08d4;
char* fn_71003be154(UnkActor*);

// A sound slot the handle refers to while its id still matches.
struct UnkGooshockerSoundSlot_ovb0125 {
    UnkSound* sound;
    u64 id;
};

struct UnkGooshockerSoundHandle_ovb0125 {
    UnkGooshockerSoundSlot_ovb0125* slot;
    u64 id;

    UnkSound* Get() const {
        if (slot && slot->id == id && id != (u64)-1)
            return slot->sound;
        return nullptr;
    }
};

struct UnkGooshockerAI2_ovb0125 {
    u8 pad_0x0[0x30];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x14694];
    u8 field_0x146cc[0xf];
    bool field_0x146db;
    u8 pad_0x146dc[0x11c];
    s32 field_0x147f8;
    u8 pad_0x147fc[0xb4];
    UnkGooshockerSoundHandle_ovb0125 sound;  // 0x148b0
    u8 pad_0x148c0[0x8];
    f32 field_0x148c8;
    u8 pad_0x148cc[0x15];
    bool field_0x148e1;
    u8 pad_0x148e2[0x2];
    f32 field_0x148e4;
    bool field_0x148e8;
    bool field_0x148e9;
};
static_assert(__builtin_offsetof(UnkGooshockerAI2_ovb0125, field_0x148e9) == 0x148e9, "layout");

void fn_710059ab2c(UnkGooshockerAI2_ovb0125* self, f32 dt);

void fn_710059e384(UnkGooshockerAI2_ovb0125* self, f32 dt) {
    fn_710059ab2c(self, dt);
    if (self->field_0x148c8 > 0.0f) {
        self->field_0x148c8 -= dt;
        if (self->field_0x148c8 <= 0.0f) {
            self->field_0x148c8 = 0.0f;
            if (char* part = fn_71003be154(self->owner))
                fn_71007d29d4(part);
            UnkSound* sound = self->sound.Get();
            if (sound && sound->field_0x118)
                fn_7100105a80(sound, 0.2f, 0.0f);
            if (self->field_0x147f8 <= 0 && !self->field_0x148e8) {
                // fn_710059aa2c reads only its first argument; the second is left as it is.
                UnkGooplotOwner_b55* unused;
                fn_710059aa2c(reinterpret_cast<UnkGooplotAI_b55*>(self), unused);
            }
        }
    }
    if (self->field_0x148e4 > 0.0f) {
        self->field_0x148e4 -= dt;
        if (self->field_0x148e4 <= 0.0f) {
            bool flag = self->field_0x148e8;
            self->field_0x148e4 = 0.0f;
            if (flag) {
                self->field_0x148e9 = true;
            } else {
                fn_710059e4f0(reinterpret_cast<UnkGooshockerState*>(self), 1);
                self->field_0x146db = true;
            }
        }
    }
    if (self->field_0x148e1) {
        self->field_0x148e1 = false;
        if (void* ai = self->owner->field_0x218)
            fn_71010d08d4(static_cast<UnkMovement_71010d08d4*>(ai),
                          reinterpret_cast<const UnkMovementVec_71010d08d4*>(self->field_0x146cc), true);
    }
}
