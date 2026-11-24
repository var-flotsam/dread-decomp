// A constructor in CCharClassFXFadeEvent's unit: the base constructor (fn_7100067ea4), the
// vtable, a string id built from "" and the fade's fields (two floats of 1 and two cleared flags).
// Names and the layout are placeholders.
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU0.h"
#include "types.h"

struct UnkFXFadeEvent {
    void** vtable;
    u8 unk_0x8[0x8];
    u64 id_0x10;
    f32 fTime;
    f32 fIntensitySrc;
    f32 fIntensityDst;
    bool bEnableBefore;
    bool bDisableAfter;
};

extern void* g_unk_7101aa5aa0[];

void fn_7100cbe13c(UnkFXFadeEvent* self) {
    fn_7100067ea4(self);
    self->vtable = g_unk_7101aa5aa0 + 2;
    CStrId_Construct(&self->id_0x10, "", true);
    self->fTime = 1.0f;
    self->fIntensitySrc = 0.0f;
    self->fIntensityDst = 1.0f;
    self->bEnableBefore = false;
    self->bDisableAfter = false;
}
