// CPoisonFlyDiveAttack unit (tools/tu_map.py, the name from reflection): fn_71006331ec sets up the attack's
// damage request by the mode (+0x608) of the owner's (+0x8) AI component (+0x240, a CPoisonFlyAIComponent)
// character class (+0x1610, a CCharClassPoisonFlyAIComponent): mode 0 takes the unit's first static id and
// the tunable's value at +0x2b0, mode 1 the second id and the value at +0x6c (id at +0xb8, value through
// fn_71008c1538); then the word at +0xc8 is set to 1. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::global::CStrId;
using base::reflection::UnkReflObject;

extern base::reflection::UnkTunable g_unk_7172716160;
extern CStrId g_unk_71726f98b8 __attribute__((visibility("hidden")));
extern CStrId g_unk_71726f98c0 __attribute__((visibility("hidden")));

struct UnkRequest;
void fn_71008c1538(UnkRequest* request, f32 value);

struct UnkPoisonFlyClass_A3_b72 {
    u8 pad_0x0[0x608];
    s32 mode;  // 0x608
};

struct UnkPoisonFlyAI_A3_b72 {
    u8 pad_0x0[0x1610];
    UnkReflObject* char_class;  // 0x1610
};

struct UnkPoisonFlyOwner_A3_b72 {
    u8 pad_0x0[0x240];
    UnkReflObject* ai;  // 0x240
};

struct UnkPoisonFlyDive_A3_b72 {
    u8 pad_0x0[0x8];
    UnkPoisonFlyOwner_A3_b72* owner;  // 0x8
};

struct UnkPoisonFlyRequest_A3_b72 {
    u8 pad_0x0[0xb8];
    CStrId id;  // 0xb8
    u8 pad_0xc0[0x8];
    u32 field_0xc8;
};

static inline const f32* TunableValue_A3_b72(u32 offset) {
    return reinterpret_cast<const f32*>(reinterpret_cast<const char*>(&g_unk_7172716160) + offset);
}

void fn_71006331ec(UnkPoisonFlyDive_A3_b72* self, void* unused, UnkPoisonFlyRequest_A3_b72* request) {
    UnkReflObject* ai = self->owner->ai;
    UnkPoisonFlyAI_A3_b72* cast =
        fn_710009d50c(ai->vf_0x0(), fn_7100638be0()) ? reinterpret_cast<UnkPoisonFlyAI_A3_b72*>(ai) : nullptr;
    UnkReflObject* klass = cast->char_class;
    UnkPoisonFlyClass_A3_b72* data = fn_710009d50c(klass->vf_0x0(), fn_7100638214())
                                         ? reinterpret_cast<UnkPoisonFlyClass_A3_b72*>(klass)
                                         : nullptr;
    const f32* value;
    switch (data->mode) {
    case 0:
        request->id = g_unk_71726f98b8;
        value = TunableValue_A3_b72(0x2b0);
        break;
    case 1:
        request->id = g_unk_71726f98c0;
        value = TunableValue_A3_b72(0x6c);
        break;
    default:
        goto done;
    }
    fn_71008c1538(reinterpret_cast<UnkRequest*>(request), *value);
done:
    request->field_0xc8 = 1;
}
