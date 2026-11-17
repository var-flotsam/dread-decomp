// CCameraCtrl (tools/tu_map.py unit 2693): the camera controller's constructor, on the game object
// base: its defaults (two poses looking along y with a 45 degree field of view), copies of the default
// string id and two sets of three empty arrays (flags 0xf). Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCameraCtrl.h"
#include "game/CGameObject.h"
#include "types.h"

using base::global::CStrId;

extern char g_unk_7101cb1720[];

// A camera pose: position, target, orientation (w: the field of view) and two scalars.
struct UnkCameraPose_l0ed044 {
    f32 position[3];
    f32 target[3];
    f32 rotation[4];
    f32 field_0x28;
    f32 field_0x2c;
};

// Three arrays (growth step 10, allocator flags 0xf) and a flag.
struct UnkCameraLists_l0ed044 {
    base::global::CRntVector<void*> field_0x0;
    base::global::CRntVector<void*> field_0x28;
    base::global::CRntVector<void*> field_0x50;
    bool field_0x78;
};
static_assert(sizeof(UnkCameraLists_l0ed044) == 0x80, "lists layout");

struct UnkCameraCtrl_l0ed044 {
    void** vtable;
    u8 pad_0x8[0x8];
    bool field_0x10;
    u8 pad_0x11[0x7];
    u64 field_0x18;
    CStrId name;  // 0x20
    s32 field_0x28;
    f32 field_0x2c;
    f32 field_0x30;
    bool field_0x34;
    bool field_0x35;
    u8 pad_0x36[0x2];
    UnkCameraPose_l0ed044 previous;  // 0x38
    UnkCameraPose_l0ed044 current;   // 0x68
    bool field_0x98;
    u8 pad_0x99[0x3];
    s32 mode;     // 0x9c
    f32 blend;    // 0xa0
    f32 period;   // 0xa4
    f32 field_0xa8;
    bool field_0xac;
    u8 pad_0xad[0x3];
    void* handle;  // 0xb0
    void* follow;  // 0xb8
    f32 field_0xc0;
    f32 field_0xc4;
    CStrId field_0xc8;
    CStrId on_reset;  // 0xd0
    UnkCameraLists_l0ed044 field_0xd8;
    UnkCameraLists_l0ed044 field_0x158;
};
static_assert(__builtin_offsetof(UnkCameraCtrl_l0ed044, field_0x158) == 0x158, "layout");

// A pose at the origin, looking along y with a 45 degree field of view.
static inline void ResetPose(UnkCameraPose_l0ed044* pose) {
    pose->position[0] = 0.0f;
    pose->position[1] = 0.0f;
    pose->position[2] = 0.0f;
    pose->target[0] = 0.0f;
    pose->target[1] = 0.0f;
    pose->target[2] = 0.0f;
    pose->rotation[0] = 0.0f;
    pose->rotation[1] = 1.0f;
    pose->rotation[2] = 0.0f;
    pose->rotation[3] = 45.0f;
    pose->field_0x28 = 0.0f;
    pose->field_0x2c = 0.0f;
}

static inline void InitLists(UnkCameraLists_l0ed044* lists) {
    new (&lists->field_0x0) base::global::CRntVector<void*>(0xf);
    new (&lists->field_0x28) base::global::CRntVector<void*>(0xf);
    new (&lists->field_0x50) base::global::CRntVector<void*>(0xf);
    lists->field_0x78 = false;
}

void fn_71010ed044(UnkBase_7100763ed4* base) {
    fn_7101043904(base);
    UnkCameraCtrl_l0ed044* self = reinterpret_cast<UnkCameraCtrl_l0ed044*>(base);
    self->vtable = reinterpret_cast<void**>(g_unk_7101cb1720 + 0x10);
    self->field_0x10 = true;
    self->field_0x18 = 0;
    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
    ResetPose(&self->previous);
    ResetPose(&self->current);
    self->field_0x28 = 0;
    self->field_0x2c = 0.0f;
    self->field_0x30 = 0.0f;
    self->field_0x34 = false;
    self->field_0x35 = false;
    self->field_0x98 = true;
    self->mode = 1;
    self->blend = 1.0f;
    self->period = 0.15f;
    self->field_0xa8 = 0.0f;
    self->field_0xac = false;
    self->handle = nullptr;
    self->follow = nullptr;
    self->field_0xc0 = -1.0f;
    self->field_0xc4 = -1.0f;
    new (&self->field_0xc8) CStrId(g_unk_7101d470e0);
    new (&self->on_reset) CStrId(g_unk_7101d470e0);
    InitLists(&self->field_0xd8);
    InitLists(&self->field_0x158);
}
