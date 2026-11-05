// The CHydrogigaAIComponent unit. fn_71005c2b2c sets +0x149c8 to -1; when the owner (+0x30) has its
// collision container (+0x2a0), its sub-object at +0x220 and the collider named g_unk_71726eb440, sends
// the sub-object a one under one of two ids (picked by the collider's flag at +0x6a) for two kinds (the
// all-flags mask, then the empty one) and turns the collider off; finally clears +0x149c4.
// Names are placeholders.
#include "../../game_s2/UnkSpitSend.h"
#include "game/UnkBroadphaseObject.h"
#include "game/UnkCollisionRecord.h"
#include "types.h"

extern base::global::CStrId g_unk_71726eb440 __attribute__((visibility("hidden")));

struct UnkHydrogigaOwner3_b59a3 {
    u8 pad_0x0[0x220];
    void* field_0x220;
    u8 pad_0x228[0x2a0 - 0x228];
    UnkCollisionContainer* field_0x2a0;
};

struct UnkHydrogigaAI3_b59a3 {
    u8 pad_0x0[0x30];
    UnkHydrogigaOwner3_b59a3* owner;
    u8 pad_0x38[0x149c4 - 0x38];
    u32 field_0x149c4;
    f32 field_0x149c8;
};

void fn_71005c2b2c(UnkHydrogigaAI3_b59a3* self) {
    self->field_0x149c8 = -1.0f;
    UnkCollisionContainer* container = self->owner->field_0x2a0;
    void* target;
    UnkCollisionObject* object;
    // The gotos skip the declarations below, so they get no lifetime markers and keep slots of their own.
    if (!container)
        goto done;
    target = self->owner->field_0x220;
    object = fn_71008640a4(container, &g_unk_71726eb440);
    if (!target || !object)
        goto done;
    base::reflection::UnkReflTypeIdArg mode1;
    base::reflection::UnkReflTypeIdArg mask1;
    base::reflection::UnkReflTypeIdArg mode2;
    base::reflection::UnkReflTypeIdArg mask2;
    base::reflection::UnkReflTypeIdArg mode3;
    base::reflection::UnkReflTypeIdArg mask3;
    base::reflection::UnkReflTypeIdArg mode4;
    base::reflection::UnkReflTypeIdArg mask4;
    if (reinterpret_cast<bool*>(object)[0x6a]) {
        mode1.id = 1;
        mask1.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x7620adeca58e32f3, 0xf4061ff25a2163d6, &mode1, &g_unk_710164b958, &g_unk_7172769740,
                      &mask1);
        mode2.id = 1;
        mask2.id = UnkNone_SpitSend().value;
        fn_7101176690(target, 0x7620adeca58e32f3, 0xbd794f4bf6d1d598, &mode2, &g_unk_710164b958, &g_unk_7172769740,
                      &mask2);
    } else {
        mode3.id = 1;
        mask3.id = UnkAll_SpitSend().value;
        fn_7101176690(target, 0x9e7051642e151650, 0xf4061ff25a2163d6, &mode3, &g_unk_710164b958, &g_unk_7172769740,
                      &mask3);
        mode4.id = 1;
        mask4.id = UnkNone_SpitSend().value;
        fn_7101176690(target, 0x9e7051642e151650, 0xbd794f4bf6d1d598, &mode4, &g_unk_710164b958, &g_unk_7172769740,
                      &mask4);
    }
    fn_7100d35a98(reinterpret_cast<UnkEntry*>(object), false);
done:
    self->field_0x149c4 = 0;
}
