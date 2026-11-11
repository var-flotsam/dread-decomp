// CSamusAirMovementState unit (unit 1455 of tools/tu_map.py): fills a transition description. When
// the owner's (+0x30) animation component (+0x220) plays a CCharClassInstantMorphballConversionTrack,
// the target is a fixed state hash with the global blend setting g_unk_7172769770 and the flag at
// +0x30 set; otherwise the target is the hash of the reflection singleton's "fall2mb" string id
// (+0xfd8). Returns true either way. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflStatics.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkTU1730.h"
#include "types.h"

// As src/game/CCharClassToStateTrack/CCharClassToStateTrack_Fill.cpp declares it.
struct __attribute__((packed, aligned(4))) UnkTrackBlend {
    u64 kind;
    f32 value;
};

struct UnkAirOwner_b52 {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;
};

struct UnkAirState_b52 {
    u8 pad_0x0[0x30];
    UnkAirOwner_b52* owner;
};

struct UnkAirTransition_b52 {
    u64 state;
    u8 unk_0x8[0x10];
    UnkTrackBlend blend;
    u8 unk_0x24[0xc];
    bool flag_0x30;
};

struct UnkAirStateName_b52 {
    u8 unk_0x0[0x30];
    u64 hash;
};

bool fn_710098d790(UnkAirState_b52* self, UnkAirTransition_b52* out) {
    UnkAnimationComponent* animation = self->owner->animation;
    if (animation && fn_710117b348(animation, fn_7100d14d38())) {
        out->blend = g_unk_7172769770;
        out->state = 0x75960bf29f3f6c1dull;
        out->flag_0x30 = true;
        return true;
    }
    out->state = (*reinterpret_cast<UnkAirStateName_b52**>(
                      reinterpret_cast<char*>(&base::reflection::refl_GetSingleton()) + 0xfd8))
                     ->hash;
    return true;
}
