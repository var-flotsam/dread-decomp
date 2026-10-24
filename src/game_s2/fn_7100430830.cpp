// Limits the box [min xyz, max xyz] at `box` (six floats) so that it covers the region
// the Chozo Commander's component at +0x240 of the owner stores (at 0x15d1c: two floats and a third
// that replaces the first when the owner's direction is positive), when the global manager's
// camera controller is a boss camera: its fixed 3.0 is written to the controller's 0x344. The z extent
// is clamped to include 0.
#include "base/UnkMatrix44.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "types.h"

using base::reflection::CClass;

struct UnkObject {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkObject*)>(vtable[0])(this); }
};

struct UnkCamera : UnkObject {
    u8 pad_0x8[0x33c];
    f32 field_0x344;
};

struct UnkOwner {
    u8 pad_0x0[0x38];
    u8 field_0x38[1];
    u8 pad_0x39[0x207];
    UnkObject* field_0x240;
};

struct UnkSelf {
    u8 pad_0x0[8];
    UnkOwner* field_0x8;
};

struct UnkBox {
    f32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
    f32 field_0xc;
    f32 field_0x10;
    f32 field_0x14;
};

void* fn_7100defcec(UnkGameManager* manager);
UnkCamera* fn_71010e2080();
const CClass* fn_710074eca8();  // CBossCameraCtrl::refl_GetClass
const CClass* fn_710044e708();  // CChozoCommanderAIComponent::refl_GetClass

// NON_MATCHING(minor): the last z clamp (fmaxnm) and its store are scheduled before the paired store
// of the max x/y pair (scheduling only)
void fn_7100430830(UnkSelf* self, UnkBox* box) {
    if (!fn_7100defcec(g_unk_7101d47310))
        return;
    UnkCamera* camera = fn_71010e2080();
    if (!camera)
        return;
    if (!fn_710009d50c(camera->vf_0x0(), fn_710074eca8()))
        return;
    UnkObject* component = self->field_0x8->field_0x240;
    if (!component)
        return;
    if (!fn_710009d50c(component->vf_0x0(), fn_710044e708()))
        return;
    f32* region = reinterpret_cast<f32*>(reinterpret_cast<u64>(component) + 0x15d1c);
    camera->field_0x344 = 3.0f;
    f32 low = region[0];
    f32 high = region[1];
    if (fn_710002614c(reinterpret_cast<const UnkMatrix44*>(self->field_0x8->field_0x38)).m[2] > 0.0f)
        low = region[2];
    f32 min_x = box->field_0x0 < low ? box->field_0x0 : low;
    f32 min_y = box->field_0x4 < high ? box->field_0x4 : high;
    box->field_0x0 = min_x;
    box->field_0x4 = min_y;
    box->field_0x8 = __builtin_fminf(box->field_0x8, 0.0f);
    f32 max_x = box->field_0xc > low ? box->field_0xc : low;
    f32 max_y = box->field_0x10 > high ? box->field_0x10 : high;
    box->field_0xc = max_x;
    box->field_0x10 = max_y;
    box->field_0x14 = __builtin_fmaxf(box->field_0x14, 0.0f);
}
