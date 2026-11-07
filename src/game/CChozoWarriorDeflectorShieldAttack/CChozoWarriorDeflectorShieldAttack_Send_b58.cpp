// CChozoWarriorDeflectorShieldAttack's unit (tools/tu_map.py): after the base update, once (flag at +0x48),
// when the owner's (+0x8) component at +0x240 is a CChozoWarriorAIComponent and the owner's sub-object at
// +0x220 exists: reads a distance from the component (fn_71010fc554 when the owner faces +x, else
// fn_71010fc5fc) and, unless it exceeds the limit at +0x4c, sends the sub-object a two (refl_EmitU32
// written out, for its stack slots) and sets the flag. Names are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflStatics.h"
#include "../../game_s2/UnkSpitSend.h"
#include "game_s2/UnkSpitAttack.h"
#include "types.h"

void fn_71010fc554(base::reflection::UnkReflObject* component, f32* out);
void fn_71010fc5fc(base::reflection::UnkReflObject* component, f32* out);

struct UnkDeflectorOwner_b58 {
    u8 pad_0x0[0x38];
    u8 field_0x38[0x1e8];
    void* field_0x220;
    u8 pad_0x228[0x18];
    base::reflection::UnkReflObject* field_0x240;
};

struct UnkDeflector_b58 {
    u8 pad_0x0[0x8];
    UnkDeflectorOwner_b58* owner;
    u8 pad_0x10[0x38];
    bool field_0x48;
    u8 pad_0x49[3];
    f32 field_0x4c;
};

// `dt` (s0) reaches the base update untouched in the original (tools/forwarded_args.py).
void fn_7100494210(UnkDeflector_b58* self, f32 dt) {
    fn_71003609a8(reinterpret_cast<UnkSpitAttack*>(self), dt);
    base::reflection::UnkReflObject* component = self->owner->field_0x240;
    if (!component || !fn_710009d50c(component->vf_0x0(), fn_7100497f2c()))
        return;
    void* target = self->owner->field_0x220;
    if (!target || self->field_0x48)
        return;
    f32 distance = 0.0f;
    if (fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->owner->field_0x38)).m[2] >= 0.0f) {
        fn_71010fc554(component, &distance);
        if (!(distance <= self->field_0x4c))
            return;
    } else {
        fn_71010fc5fc(component, &distance);
        if (!(distance <= self->field_0x4c))
            return;
    }
    u32 mode = 2;
    base::reflection::UnkReflTypeIdArg arg;
    arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
    fn_7101176690(target, 0x04dc4c0c7b23af17, 0xf4061ff25a2163d6, reinterpret_cast<base::reflection::UnkReflTypeIdArg*>(&mode),
                  &g_unk_710164b958, &g_unk_7172769740, &arg);
    self->field_0x48 = true;
}
