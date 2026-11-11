// CChozoWarriorAIComponent's unit (tools/tu_map.py): when the owner's (+0x30) sub-object at +0x220 and its
// actor (fn_7100d7c990) exist, runs fn_710049d094 and the facing update (fn_7100498db8), sends the
// sub-object a nine under the hash at +0x14b40 (refl_EmitU32 written out, for its stack slots), turns the owner a quarter turn away from
// the side it faces (vtable slot 0xd8 with the angles (0, +-pi/2, 0)) and sets the counter at +0x149cc
// to 3. Names are placeholders.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/CChozoWarriorAIComponent.h"
#include "game/UnkActor.h"
#include "../../game_s2/UnkSpitSend.h"
#include "types.h"

struct UnkSelf;

struct UnkWarriorTurnAngles_b58 {
    f32 x;
    f32 y;
    f32 z;
};

struct UnkWarriorTurnOwner_b58 {
    void** vtable;
    u8 pad_0x8[0x218];
    void* field_0x220;
};

struct UnkWarriorTurn_b58 {
    u8 pad_0x0[0x30];
    UnkWarriorTurnOwner_b58* owner;
    u8 pad_0x38[0x149cc - 0x38];
    u32 field_0x149cc;
    u8 pad_0x149d0[0x14b40 - 0x149d0];
    u64 field_0x14b40;
};

void fn_71004994f4(UnkWarriorTurn_b58* self) {
    UnkWarriorTurnOwner_b58* owner = self->owner;
    void* target = owner->field_0x220;
    UnkActor* actor = fn_7100d7c990(reinterpret_cast<UnkActor*>(owner));
    if (target && actor) {
        base::reflection::UnkReflTypeIdArg value;
        base::reflection::UnkReflTypeIdArg arg;
        UnkWarriorTurnAngles_b58 angles;
        fn_710049d094(self);
        fn_7100498db8(reinterpret_cast<UnkSelf*>(self));
        u64 hash = self->field_0x14b40;
        value.id = 9;
        arg.id = base::reflection::UnkReflTypeId<base::reflection::UnkReflTypeIdTag>::id();
        fn_7101176690(target, hash, 0xf4061ff25a2163d6, &value, &g_unk_710164b958, &g_unk_7172769740, &arg);
        UnkWarriorTurnOwner_b58* o = self->owner;
        const UnkMatrix44& m = fn_710002614c(reinterpret_cast<const UnkMatrix44*>(actor->field_0x38));
        f32 y = m.m[2] >= 0.0f ? -1.5707964f : 1.5707964f;
        angles.x = 0.0f;
        angles.z = 0.0f;
        angles.y = y;
        reinterpret_cast<void (*)(UnkWarriorTurnOwner_b58*, UnkWarriorTurnAngles_b58*, bool)>(o->vtable[0xd8 / 8])(
            o, &angles, true);
        self->field_0x149cc = 3;
    }
}
