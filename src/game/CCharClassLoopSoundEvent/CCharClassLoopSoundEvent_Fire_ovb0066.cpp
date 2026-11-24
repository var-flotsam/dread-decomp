// CCharClassLoopSoundEvent's unit (tools/tu_map.py unit 2262): the event's fire (starts the loop
// sound through fn_7100cdc910 unless the GUI (g_unk_7101d47330) answers fn_7101198688 or the game
// manager answers fn_7100deabe4) and its copy-assignment. Names and layouts are placeholders.
#include "base/UnkDestructors.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2730.h"
#include "types.h"

using base::UnkStrIdMember;
using base::UnkStrIdMemberOf;
using base::global::CStrId;

struct UnkLoopSoundEvent_ovb0066 {
    virtual void vf_0x0();

    u32 field_0x8;
    UnkStrIdMemberOf<g_unk_7101d47200> path;  // 0x10
    UnkStrIdMember field_0x18;
    f32 fVolume;
    f32 fRadiusMinAttenuation;
    f32 fRadiusMaxAttenuation;
    f32 fPitch;
    bool bStopOnChangeAnim;
    bool bStopOnEntityDead;
    f32 fFadeInTime;
    f32 fFadeOutTime;
    f32 fStopInTime;
    f32 fRumbleGainOverride;
    bool bRumbleSync;
    UnkStrIdMember sound;  // 0x48
};

void fn_7100cdc910(UnkLoopSoundEvent_ovb0066* self, UnkStrIdMemberOf<g_unk_7101d47200>* path,
                   UnkStrIdMember* sound, const CStrId* a, const CStrId* b, f32 f0, f32 f1, f32 f2, f32 f3,
                   bool c, bool d, f32 f4, f32 f5, f32 f6, f32 f7, bool e, bool g, f32 f8, void* arg);

void CCharClassLoopSoundEvent_PlayLoopSound(UnkLoopSoundEvent_ovb0066* self, void* arg) {
    if (fn_7101198688(g_unk_7101d47330))
        return;
    if (fn_7100deabe4(g_unk_7101d47310))
        return;
    fn_7100cdc910(self, &self->path, &self->sound, &base::global::CStrId::Empty(), &base::global::CStrId::Empty(), self->fVolume,
                  self->fRadiusMinAttenuation, self->fRadiusMaxAttenuation, self->fPitch, self->bStopOnChangeAnim, true, -1.0f,
                  self->fFadeInTime, self->fFadeOutTime, self->fStopInTime, self->bStopOnEntityDead, self->bRumbleSync,
                  self->fRumbleGainOverride, arg);
}

__attribute__((flatten)) UnkLoopSoundEvent_ovb0066* fn_7100cc7e18(UnkLoopSoundEvent_ovb0066* self, const UnkLoopSoundEvent_ovb0066* other) {
    *self = *other;
    return self;
}
