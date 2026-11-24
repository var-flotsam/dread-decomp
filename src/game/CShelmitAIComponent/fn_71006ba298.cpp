// The CShelmitAIComponent unit: after the base update, mirrors the mode at +0x14780 in the owner's
// two model flags, resets the mode's timers from the target at +0x146c8, and in mode 1 sends the
// owner's sub-object a request and gives the owner's object at +0x1b8 the global tunable's +0x1ec;
// in mode 0, unless the flag at +0x15d0 is set, calls fn_71006ba4fc. Names and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/CCharClassLifeComponent.h"
#include "game/CModelUpdaterComponent.h"
#include "game/CShelmitAIComponent.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkShelmitObject_b56a3.h"
#include "types.h"

// Declared with this type in src/game/CCharClassCentralUnitAIComponent/fn_7100408770.cpp.
struct UnkGroupIds;

struct UnkShelmitOwner_l6ba298 {
    u8 pad_0x0[0x1b8];
    UnkShelmitObject_b56a3* field_0x1b8;
    u8 pad_0x1c0[0x60];
    void* field_0x220;
    UnkFlagTarget* flags;  // 0x228
};

struct UnkShelmitTarget_l6ba298 {
    u8 pad_0x0[0x638];
    f32 field_0x638;
};

// The mode block at +0x146c8.
struct UnkShelmitMode_l6ba298 {
    UnkShelmitTarget_l6ba298* target;
    u8 pad_0x8[0xb8 - 0x8];
    u32 mode;  // 0xb8
    u32 previous_mode;  // 0xbc
    u8 pad_0xc0[0xd4 - 0xc0];
    f32 field_0xd4;
    f32 field_0xd8;
    f32 field_0xdc;
    f32 field_0xe0;
};

struct UnkShelmitAI_l6ba298 {
    u8 pad_0x0[0x30];
    UnkShelmitOwner_l6ba298* owner;
    u8 pad_0x38[0x15d0 - 0x38];
    bool field_0x15d0;
    u8 pad_0x15d1[0x146c8 - 0x15d1];
    UnkShelmitMode_l6ba298 mode;
};

void fn_71006ba298(UnkShelmitAI_l6ba298* self) {
    UnkShelmitMode_l6ba298* mode = &self->mode;
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkShelmitObject_b56a3* object = nullptr;
    void* target = nullptr;
    if (self->owner) {
        target = self->owner->field_0x220;
        object = self->owner->field_0x1b8;
    }
    const char* on = mode->mode == 0 ? g_unk_710164a730 : g_unk_710164a738;
    const char* off = mode->mode == 0 ? g_unk_710164a738 : g_unk_710164a730;
    fn_7101172764(self->owner->flags, on);
    fn_7101172780(self->owner->flags, off);
    mode->previous_mode = mode->mode;
    mode->field_0xe0 = 1.0f;
    if (mode->target) {
        f32 value = mode->target->field_0x638;
        mode->field_0xd8 = value;
        mode->field_0xd4 = value;
        mode->field_0xdc = 0.0f;
        CShelmitAIComponent_ApplyCrystalEmissiveFactor(reinterpret_cast<UnkShelmitAI*>(self), value);
    }
    fn_71006ba194(reinterpret_cast<UnkShelmitAI*>(self));
    u32 current = mode->mode;
    if (target && current == 1) {
        base::reflection::UnkReflTypeIdArg request;
        request.id = current;
        base::reflection::UnkReflTypeIdArg mask;
        mask.id = UnkAll_SpitSend().value;
        CAnimationComponent_SendActionToTrack(target, 0xfb70eec0a98665b3, 0xf4061ff25a2163d6, &request, &g_unk_710164b958, &g_unk_7172769740,
                      &mask);
    }
    current = mode->mode;
    if (object && current == 1) {
        object->vf_0x170(g_unk_717272fd38.field_0x1ec);
        object->vf_0x168(g_unk_717272fd38.field_0x1ec, true);
    }
    current = mode->mode;
    if (current == 0 && !self->field_0x15d0)
        fn_71006ba4fc(self);
}
