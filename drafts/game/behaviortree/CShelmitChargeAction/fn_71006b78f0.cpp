// behaviortree::CShelmitChargeAction (tools/tu_map.py): its event handler. For a kind-1 event whose id is
// not empty: in stage 2 an id hash of 0x884c8e254ba39908 sends the owner's (+0x18) sub-object (+0x220) a
// request and, when it is taken, moves to stage 3 and starts sampling the character class's value; in stage 4
// an id hash of 0x6836cdff1d937e28 moves to stage 5 and resets the AI's charge state. Always returns true.
// Names and layouts are placeholders.
#include "../../../game_s2/UnkSpitSend.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CShelmitAIComponent.h"
#include "types.h"

struct UnkShelmitAI_b35;
void fn_71006ba4a8(UnkShelmitAI_b35* self, f32 dt);

struct UnkShelmitChargeCharClass_l6b78f0 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x618 - 0x8];
    f32 field_0x618;
    u8 pad_0x61c[0x640 - 0x61c];
    f32 field_0x640;
};

struct UnkShelmitChargeState_l6b78f0 {
    bool active;  // 0x0
    u8 pad_0x1[3];
    f32 value;  // 0x4
    u8 pad_0x8[0xc];
    f32 scale;  // 0x14
};

struct UnkShelmitChargeAI_l6b78f0 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1610 - 0x8];
    base::reflection::UnkReflObject* charClass;
    u8 pad_0x1618[0x14794 - 0x1618];
    UnkShelmitChargeState_l6b78f0 charge;
};
static_assert(__builtin_offsetof(UnkShelmitChargeAI_l6b78f0, charge) == 0x14794, "layout");

struct UnkShelmitChargeOwner_l6b78f0 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x240 - 0x228];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkShelmitChargeIdHolder_l6b78f0 {
    u8 pad_0x0[0x38];
    base::global::CStrId id;
};

struct UnkShelmitChargeIdRef_l6b78f0 {
    u8 pad_0x0[0x10];
    UnkShelmitChargeIdHolder_l6b78f0* holder;
};

struct UnkShelmitChargeEvent_l6b78f0 {
    s32 kind;
    u8 pad_0x4[4];
    UnkShelmitChargeIdRef_l6b78f0* ref;

    const base::global::CStrId& Id() const {
        if (ref && ref->holder)
            return ref->holder->id;
        return g_unk_7101d470e0;
    }
};

struct UnkShelmitChargeAction_l6b78f0 {
    u8 pad_0x0[0x18];
    UnkShelmitChargeOwner_l6b78f0* owner;
    u8 pad_0x20[0x30];
    s32 stage;  // 0x50
    f32 field_0x54;
};

bool fn_71006b78f0(UnkShelmitChargeAction_l6b78f0* self, UnkShelmitChargeEvent_l6b78f0* event) {
    const base::global::CStrId& id = event->Id();

    UnkShelmitChargeAI_l6b78f0* ai = nullptr;
    UnkShelmitChargeCharClass_l6b78f0* charClass = nullptr;
    bool isShelmit = false;
    if (self->owner) {
        base::reflection::UnkReflObject* component = self->owner->field_0x240;
        if (component && fn_710009d50c(component->vf_0x0(), fn_71006b98c0())) {
            ai = static_cast<UnkShelmitChargeAI_l6b78f0*>(component);
            base::reflection::UnkReflObject* cc = ai->charClass;
            if (cc && fn_710009d50c(cc->vf_0x0(), fn_71006b8744()))
                charClass = static_cast<UnkShelmitChargeCharClass_l6b78f0*>(cc);
            isShelmit = true;
        }
    }
    void* target = self->owner ? self->owner->field_0x220 : nullptr;

    base::global::UnkStringData* data = id.data;
    if (data == &g_unk_7101d47108->empty)
        return true;

    switch (self->stage) {
    case 2:
        if (data->Key() != 0x884c8e254ba39908)
            break;
        if (target && event->kind == 1) {
            base::reflection::UnkReflTypeIdArg mode;
            mode.id = 0;
            base::reflection::UnkReflTypeIdArg mask;
            mask.id = UnkAll_SpitSend().value;
            if (fn_7101176690(target, 0xce6cb58bea34b4f7, 0xf4061ff25a2163d6, &mode, &g_unk_710164b958,
                              &g_unk_7172769740, &mask)) {
                self->stage = 3;
                if ((charClass != nullptr) & isShelmit) {
                    self->field_0x54 = charClass->field_0x640;
                    fn_71006ba4a8(reinterpret_cast<UnkShelmitAI_b35*>(ai), self->field_0x54);
                }
            }
        }
        break;
    case 4:
        if (data->Key() == 0x6836cdff1d937e28 && event->kind == 1) {
            self->stage = 5;
            UnkShelmitChargeState_l6b78f0& charge = ai->charge;
            charge.scale = 1.0f;
            if (charge.active) {
                charge.value = charClass->field_0x618;
                fn_71006ba4fc(ai);
            }
        }
        break;
    }
    return true;
}
