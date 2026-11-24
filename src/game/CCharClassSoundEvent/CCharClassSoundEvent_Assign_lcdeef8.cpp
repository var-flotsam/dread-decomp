// CCharClassSoundEvent unit (tools/tu_map.py unit 2297): the event's copy-assignment: the base's word, the
// eight file-path ids, the string id, the block of settings and the two last string ids and word.
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

// The event base: polymorphic, with one word.
struct UnkSoundEventBase_lcdeef8 {
    virtual void vf_0x0();
    u32 field_0x8;
};

struct UnkSoundEvent_lcdeef8 : UnkSoundEventBase_lcdeef8 {
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

    // The implicit copy-assignment, emitted out of line (fn_7100cdeef8).
    UnkSoundEvent_lcdeef8& operator=(const UnkSoundEvent_lcdeef8& other) asm("fn_7100cdeef8");
};
static_assert(__builtin_offsetof(UnkSoundEvent_lcdeef8, sSoundGroupId) == 0x88, "layout");

UnkSoundEvent_lcdeef8& UnkSoundEvent_lcdeef8::operator=(const UnkSoundEvent_lcdeef8& other) = default;
