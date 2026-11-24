// CCharClassSoundEvent's unit (tools/tu_map.py unit 2297): the event plays its sound (fn_7100cdc2f0) with
// its parameters and the given target, unless the GUI (g_unk_7101d47330) answers fn_7101198688. Names
// and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "types.h"

using base::global::CStrId;

struct UnkCharClassSoundEvent_b0080 {
    u8 pad_0x0[0x10];
    u8 sSoundID[8];
    u8 sAlternativeSoundID1[8];
    u8 sAlternativeSoundID2[8];
    u8 sAlternativeSoundID3[8];
    u8 sAlternativeSoundID4[8];
    u8 sAlternativeSoundID5[8];
    u8 sAlternativeSoundID6[8];
    u8 sAlternativeSoundID7[8];
    u8 pad_0x50[8];
    f32 fVolume;
    f32 fRadiusMinAttenuation;
    f32 fRadiusMaxAttenuation;
    f32 fPitch;
    f32 fPitchMax;
    u8 bStopOnChangeAnim;
    f32 fFadeInTime;
    f32 fFadeOutTime;
    f32 fStopInTime;
    f32 fRumbleGainOverride;
    u8 bStopOnEntityDead;
    u8 bRumbleSync;
    u8 pad_0x82[6];
    u8 sSoundGroupId[8];
    u8 sType[8];
    u32 eSoundType;
};
static_assert(__builtin_offsetof(UnkCharClassSoundEvent_b0080, eSoundType) == 0x98, "layout");

void fn_7100cdc2f0(UnkCharClassSoundEvent_b0080* self, const void* a10, const void* a18, const void* a20,
                   const void* a28, const void* a30, const void* a38, const void* a40, f32 f58, f32 f5c,
                   f32 f60, f32 f64, f32 f68, f32 f70, f32 f74, f32 f7c, const void* a48, const void* a90,
                   const CStrId* id, const void* a88, u8 b6c, u32 u98, u8 b80, u8 b81, f32 f78, void* arg);

void fn_7100cdebcc(UnkCharClassSoundEvent_b0080* self, void* arg) {
    if (fn_7101198688(g_unk_7101d47330))
        return;
    fn_7100cdc2f0(self, self->sSoundID, self->sAlternativeSoundID1, self->sAlternativeSoundID2, self->sAlternativeSoundID3, self->sAlternativeSoundID4,
                  self->sAlternativeSoundID5, self->sAlternativeSoundID6, self->fVolume, self->fRadiusMinAttenuation, self->fRadiusMaxAttenuation,
                  self->fPitch, self->fPitchMax, self->fFadeInTime, self->fFadeOutTime, self->fRumbleGainOverride,
                  self->sAlternativeSoundID7, self->sType, &base::global::CStrId::Empty(), self->sSoundGroupId, self->bStopOnChangeAnim,
                  self->eSoundType, self->bStopOnEntityDead, self->bRumbleSync, self->fStopInTime, arg);
}
