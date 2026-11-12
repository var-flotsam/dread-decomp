// CCentralUnitAIComponent's unit (tools/tu_map.py unit 614): the start (fn_7100403b30: the base update,
// the owner's position kept four times from +0x146fc, and the owner turned to the angles g_unk_7101a337c8
// unless its own are all within 0.001 of them) and the message handler (fn_7100406880: kind 2 goes to
// fn_7101199818, kind 0 sends the owner's +0x220 object two mode-2 requests and runs fn_7100406ab0). Names
// and layouts are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkBehaviorTree.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "game/UnkXformNode.h"
#include "types.h"

extern UnkVec3f g_unk_7101a337c8;

struct UnkCentralUnitOwner_ovb0125 {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkXform xform;  // 0x38

    void vf_0xd8(const UnkVec3f* angle, bool flag) {
        reinterpret_cast<void (*)(UnkCentralUnitOwner_ovb0125*, const UnkVec3f*, bool)>(vtable[0xd8 / 8])(this, angle, flag);
    }
};

struct UnkCentralUnitAI_ovb0125 {
    u8 pad_0x0[0x30];
    UnkCentralUnitOwner_ovb0125* owner;
    u8 pad_0x38[0x146c4];
    UnkVec3f field_0x146fc;
    UnkVec3f field_0x14708;
    UnkVec3f field_0x14714;
    UnkVec3f field_0x14720;
    u8 pad_0x1472c[0xa4];
    bool field_0x147d0;
};
static_assert(__builtin_offsetof(UnkCentralUnitAI_ovb0125, field_0x147d0) == 0x147d0, "layout");

static inline bool Near(f32 a, f32 b) {
    return __builtin_fabsf(a - b) < 0.001f;
}

void fn_7100403b30(UnkCentralUnitAI_ovb0125* self) {
    fn_7100369650(reinterpret_cast<char*>(self));
    UnkCentralUnitOwner_ovb0125* owner = self->owner;
    UnkVec3f pos = owner->xform.GetTranslation();
    self->field_0x146fc = pos;
    self->field_0x14708 = pos;
    self->field_0x14714 = pos;
    self->field_0x14720 = pos;
    self->field_0x147d0 = false;
    UnkVec3f angle = owner->xform.GetAngle();
    if (!(Near(g_unk_7101a337c8.x, angle.x) && Near(g_unk_7101a337c8.y, angle.y) && Near(g_unk_7101a337c8.z, angle.z)))
        self->owner->vf_0xd8(&g_unk_7101a337c8, true);
}

extern UnkDesc_SpitSend g_unk_710164b968;  // reached through the GOT

struct UnkCentralSendOwner_ovb0125 {
    u8 pad_0x0[0x220];
    void* field_0x220;
};

struct UnkCentralUnitAI3_ovb0125 {
    u8 pad_0x0[0x30];
    UnkCentralSendOwner_ovb0125* owner;
};

void fn_7100406ab0(UnkCentralUnitAI3_ovb0125* self);

void fn_7100406880(UnkCentralUnitAI3_ovb0125* self, void* unused, s32 kind) {
    switch (kind) {
    case 0:
        if (void* target = self->owner->field_0x220) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 2;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            fn_7101176690(target, 0xbdd72151748e5a82, 0xf4061ff25a2163d6, &mode, &g_unk_710164b968,
                          &g_unk_7172769740, &mask);
            void* target2 = self->owner->field_0x220;
            base::reflection::UnkReflTypeIdArg mode2;
            mode2.id = 2;
            base::reflection::UnkReflTypeIdArg mask2;
            mask2.id = UnkAll_SpitSend().value;
            fn_7101176690(target2, 0xbdd72151748e5a82, 0xcf7107181622cb23, &mode2,
                          &g_unk_710164b968, &g_unk_7172769740, &mask2);
        }
        fn_7100406ab0(self);
        break;
    case 2:
        fn_7101199818(g_unk_7101d47330, self);
        break;
    }
}
