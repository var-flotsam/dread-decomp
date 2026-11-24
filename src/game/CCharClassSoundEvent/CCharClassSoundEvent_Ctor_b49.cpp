// CCharClassSoundEvent unit (tools/tu_map.py unit 2297): a constructor: the event base constructor
// (fn_7100cdc2b8), the vtable, eight file path ids built from "", an empty string id (its constructor
// inlined), the defaults (1, 500, 3000 and 1, then -1; -1 twice after two cleared words), string ids
// built from "DEFAULT" and "N", and a cleared word. As CCharClassLoopSoundEvent's
// (src/game/CCharClassLoopSoundEvent/CCharClassLoopSoundEvent_Ctor_b46.cpp). Names and the layout are
// placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCharClassSoundBaseEvent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSoundEvent_b49 {
    void** vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId field_0x10[8];
    CStrId sNode;
    f32 fVolume;
    f32 fRadiusMinAttenuation;
    f32 fRadiusMaxAttenuation;
    f32 fPitch;
    f32 fPitchMax;
    bool bStopOnChangeAnim;
    u32 field_0x70;
    u32 field_0x74;
    f32 fStopInTime;
    f32 fRumbleGainOverride;
    bool bStopOnEntityDead;
    bool bRumbleSync;
    CStrId sSoundGroupId;
    CStrId sType;
    u32 eSoundType;
};
static_assert(__builtin_offsetof(UnkSoundEvent_b49, eSoundType) == 0x98, "layout");

extern void* g_unk_7101aa63e0[];

void fn_7100cde6ac(UnkSoundEvent_b49* self) {
    fn_7100cdc2b8(self);
    self->vtable = &g_unk_7101aa63e0[2];
    for (CFilePathStrId& id : self->field_0x10)
        new (&id) CFilePathStrId("", true);
    self->sNode.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->sNode.data = &g_unk_7101d47108->empty;
    self->fVolume = 1.0f;
    self->fRadiusMinAttenuation = 500.0f;
    self->fRadiusMaxAttenuation = 3000.0f;
    self->fPitch = 1.0f;
    self->fPitchMax = -1.0f;
    self->bStopOnChangeAnim = false;
    self->field_0x70 = 0;
    self->field_0x74 = 0;
    self->fStopInTime = -1.0f;
    self->fRumbleGainOverride = -1.0f;
    self->bStopOnEntityDead = false;
    self->bRumbleSync = false;
    new (&self->sSoundGroupId) CStrId("DEFAULT", true);
    new (&self->sType) CStrId("N", true);
    self->eSoundType = 0;
}
