// CYamplotXStepAttack's unit (tools/tu_map.py): the start. After the base start (fn_71003608ac) the
// owner's (+0x8) AI component gets fn_710111094c with 1; the owner's character class is then sent, with
// mode 0 (as src/game_s2/UnkSpitSend.h), one of two tracks: the first when the actor it acts against
// (fn_7100d7c990) lies in front of it (along the third column of its matrix), else the second.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkActor.h"
#include "game/UnkAttack.h"
#include "types.h"

using base::reflection::UnkReflObject;

void fn_71003608ac(UnkAttackBase* self);
void fn_710111094c(UnkReflObject* ai, u32 value);

void fn_71006ed934(UnkAttackBase* self) {
    // Declared past a goto (no lifetime markers), so each keeps its own slot.
    goto start;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
start:
    fn_71003608ac(self);
    fn_710111094c(static_cast<UnkReflObject*>(self->field_0x8->field_0x240), 1);
    void* cls = self->field_0x8->field_0x220;
    UnkActor* target = fn_7100d7c990(self->field_0x8);
    f32 tx = target->field_0x44;
    f32 ty = target->field_0x54;
    f32 tz = target->field_0x64;
    UnkActor* owner = self->field_0x8;
    f32 ox = owner->field_0x44;
    f32 oy = owner->field_0x54;
    f32 oz = owner->field_0x64;
    f32 dx = tx - ox;
    f32 dy = ty - oy;
    f32 dz = tz - oz;
    const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(owner->field_0x38));
    f32 fx = m.m[2];
    f32 fy = m.m[6];
    f32 fz = m.m[10];
    if (dx * fx + dy * fy + dz * fz > 0.0f) {
        mode1.id = 0;
        mask1.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, 0xf142127271e8d0e3, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                      &mask1);
    } else {
        mode2.id = 0;
        mask2.id = UnkAll_SpitSend().value;
        fn_7101176690(cls, 0x4ae549454537c757, 0xf4061ff25a2163d6, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                      &mask2);
    }
}
