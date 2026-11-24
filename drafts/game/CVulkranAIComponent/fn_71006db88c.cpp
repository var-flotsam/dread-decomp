// CVulkranAIComponent's unit: resets the AI's state after the base update, then (unless the AI's data says
// not to) plays its current animation on the character class and aims the owner along its forward axis.
// Names and layouts are placeholders.
#include <stdlib.h>

#include "../../../src/game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkActor.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU1730.h"
#include "game_s2/UnkParams.h"
#include "types.h"

struct UnkVulkranAnim_l6db88c {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

// What fn_71006daba8 answers: the animation the AI plays now.
struct UnkVulkranCurrent_l6db88c {
    UnkVulkranAnim_l6db88c* anim;
};

struct UnkVulkranData_l6db88c {
    u8 pad_0x0[0x6e0];
    u32 field_0x6e0;
};

struct UnkVulkranTracked_l6db88c {
    u8 pad_0x0[0xa0];
    UnkVulkranAnim_l6db88c* anim;  // 0xa0
};

struct UnkVulkranSlot_l6db88c {
    u8 pad_0x0[0x10];
    UnkVulkranTracked_l6db88c* tracked;  // 0x10
};

struct UnkVulkranOwner_l6db88c {
    u8 pad_0x0[0x38];
    u8 xform[0xc];  // 0x38; fn_710002614c returns its matrix
    u8 pad_0x44[0x220 - 0x44];
    UnkAnimationComponent* cls;  // 0x220
    u8 pad_0x228[0x70];
    void* sink;  // 0x298
    u8 pad_0x2a0[0xf8];
    UnkVulkranSlot_l6db88c* slot;  // 0x398
};
static_assert(__builtin_offsetof(UnkVulkranOwner_l6db88c, slot) == 0x398, "UnkVulkranOwner_l6db88c layout");

struct UnkVulkranAI_l6db88c {
    u8 pad_0x0[0x30];
    UnkVulkranOwner_l6db88c* owner;  // 0x30
    u8 pad_0x38[0x146c8 - 0x38];
    UnkVulkranData_l6db88c* data;  // 0x146c8
    bool field_0x146d0;
    f32 field_0x146d4;
    f32 field_0x146d8;
    f32 field_0x146dc;
    u32 field_0x146e0;
    bool field_0x146e4;
    u8 pad_0x146e5[0x4b];
    bool field_0x14730;
    bool field_0x14731;
    bool field_0x14732;
    bool field_0x14733;
};
static_assert(__builtin_offsetof(UnkVulkranAI_l6db88c, field_0x14730) == 0x14730, "UnkVulkranAI_l6db88c layout");

// The direction a kind-5 parameter block carries, and whether it is set.
struct UnkAimDir_l6db88c {
    f32 x;
    f32 y;
    f32 z;
    bool set;
};

// A kind-5 parameter block (game_s2/UnkParams.h) seen as an aim: its +0x8 holds a direction.
struct UnkAimParams_l6db88c {
    void** vtable;
    UnkAimDir_l6db88c dir;  // 0x8
    u32 field_0x18;
    u8 field_0x1c;
    u8 pad_0x1d[3];
    u8 field_0x20[16];
    u8 field_0x30;
    u8 pad_0x31[3];
    u32 field_0x34;
    u8 pad_0x38;
    u8 field_0x39;
    u8 pad_0x3a;
    u8 field_0x3b;
    u8 pad_0x3c;
    u8 field_0x3d;
};
static_assert(__builtin_offsetof(UnkAimParams_l6db88c, field_0x34) == 0x34, "UnkAimParams_l6db88c layout");

UnkVulkranCurrent_l6db88c* fn_71006daba8(UnkVulkranData_l6db88c* data);
void* fn_7101103a44(UnkVulkranAI_l6db88c* self);

static inline f32 Random01() { return (f32)((rand() & 0x7fff) * (1.0 / 32767.0)); }

static inline f32 RandomRange(f32 lo, f32 hi) { return Random01() * (hi - lo) + lo; }

void fn_71006db88c(UnkVulkranAI_l6db88c* self) {
    base::reflection::UnkReflTypeIdArg mode;
    base::reflection::UnkReflTypeIdArg mask;
    fn_7100369650(reinterpret_cast<char*>(self));
    self->field_0x146d0 = false;
    self->field_0x146d4 = -1.0f;
    self->field_0x146d8 = -1.0f;
    self->field_0x146e0 = 0;
    self->field_0x146e4 = false;
    self->field_0x14730 = false;
    self->field_0x146dc = RandomRange(-1.0f, -1.0f);
    self->field_0x14733 = true;
    self->field_0x14731 = false;
    self->field_0x14732 = true;

    UnkVulkranOwner_l6db88c* owner = self->owner;
    UnkAnimationComponent* cls = owner->cls;
    void* sink = owner->sink;
    UnkVulkranTracked_l6db88c* tracked = owner->slot ? owner->slot->tracked : nullptr;
    if (self->data->field_0x6e0 != 0)
        return;

    void* state = fn_7101103a44(self);
    if (cls && state) {
        u64 id = fn_71006daba8(self->data)->anim->id;
        mode.id = 2;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(cls, id, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958, &g_unk_7172769740, &mask);
    }
    bool playing = false;
    if (cls)
        playing = fn_710117ae38(cls, fn_71006daba8(self->data)->anim->id, false);
    if (tracked && !playing) {
        UnkVulkranCurrent_l6db88c* current = fn_71006daba8(self->data);
        playing = tracked->anim == current->anim;
    }

    if (sink && cls && playing) {
        UnkAimParams_l6db88c params;
        params.field_0x1c = 0;
        __builtin_memset(params.field_0x20, 0, 16);
        __builtin_memset(&params.dir, 0, 13);
        params.vtable = g_unk_7101a25b10 + 2;
        params.field_0x30 = 0;
        params.field_0x39 = 0;
        params.field_0x3b = 0;
        params.field_0x3d = 0;
        params.field_0x34 = 5;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->xform));
        f32 x = m.m[2];
        f32 y = m.m[6];
        f32 z = m.m[10];
        params.dir.x = x;
        params.dir.y = y;
        params.dir.z = z;
        params.dir.set = true;
        fn_71006f4b0c(sink, &params, 0);
    }
}
