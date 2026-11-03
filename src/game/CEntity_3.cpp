// CEntity: starting a rumble effect through the entity's rumble component (+0x2c0), shaped as
// fn_7100d7c68c (src/game/CEntity_2.cpp) with another CRumbleComponent entry point; the null handle
// when the entity has none. The signature is the one src/game/CHyperBeamGun.cpp calls it with.
// The three flags are bools (only bit 0 is passed on). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkActor;
struct UnkHyperBeamEffectDesc;
struct UnkHyperBeamEffectRef;
struct UnkHyperBeamEffectHandle {
    UnkHyperBeamEffectRef* ref;
    s64 serial;
};
struct UnkHandleTarget;
struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};
extern UnkHandle g_unk_7101d586c0;  // the null handle

struct CRumbleComponent;
struct UnkEntityWithRumble {
    u8 pad_0x0[0x2c0];
    CRumbleComponent* rumble;
};

UnkHyperBeamEffectHandle fn_71009da8f4(CRumbleComponent* self, const UnkHyperBeamEffectDesc* desc, const CStrId* name,
                                       u32 a, u32 b, u32 c);

UnkHyperBeamEffectHandle fn_7100d7c614(UnkActor* owner, const UnkHyperBeamEffectDesc* desc, const CStrId* name,
                                       u32 a, u32 b, u32 c) {
    UnkEntityWithRumble* self = reinterpret_cast<UnkEntityWithRumble*>(owner);
    if (self->rumble) return fn_71009da8f4(self->rumble, desc, name, a & 1, b & 1, c & 1);  // the flags are bools
    return reinterpret_cast<const UnkHyperBeamEffectHandle&>(g_unk_7101d586c0);
}
