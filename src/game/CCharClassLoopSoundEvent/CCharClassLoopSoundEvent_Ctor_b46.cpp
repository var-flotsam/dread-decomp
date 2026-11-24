// CCharClassLoopSoundEvent unit (tools/tu_map.py unit 2262): a constructor: the event base
// constructor (fn_7100cdc2b8), the vtable, a file path id built from " ", a copy of the empty
// string id, the defaults (1, 500, 3000 and 1, two flags set, -1 twice) and a string id built
// from "N". Names and the layout are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCharClassSoundBaseEvent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkLoopSoundEvent_b46a3 {
    void** vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId sSoundID;
    CStrId sNode;
    f32 fVolume;
    f32 fRadiusMinAttenuation;
    f32 fRadiusMaxAttenuation;
    f32 fPitch;
    bool bStopOnChangeAnim;
    bool bStopOnEntityDead;
    u8 unk_0x32[2];
    u32 field_0x34;
    u32 field_0x38;
    f32 fStopInTime;
    f32 fRumbleGainOverride;
    bool bRumbleSync;
    CStrId sType;
};

extern void* g_unk_7101aa5d48[];

void fn_7100cc7b60(UnkLoopSoundEvent_b46a3* self) {
    fn_7100cdc2b8(self);
    self->vtable = &g_unk_7101aa5d48[2];
    new (&self->sSoundID) CFilePathStrId(" ", true);
    new (&self->sNode) CStrId(base::global::CStrId::Empty());
    self->fVolume = 1.0f;
    self->fRadiusMinAttenuation = 500.0f;
    self->fRadiusMaxAttenuation = 3000.0f;
    self->fPitch = 1.0f;
    self->bStopOnChangeAnim = true;
    self->bStopOnEntityDead = true;
    self->field_0x34 = 0;
    self->field_0x38 = 0;
    self->fStopInTime = -1.0f;
    self->fRumbleGainOverride = -1.0f;
    self->bRumbleSync = false;
    new (&self->sType) CStrId("N", true);
}
