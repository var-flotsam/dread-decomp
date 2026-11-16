// CCameraCtrl's unit (tools/tu_map.py unit 2693): the camera controller's base step on taking a scene
// camera: applies its description to the camera component, saves the camera's state, then either restores
// it (when a reset is pending) or pushes the saved state back, registers itself and calls the script
// callback its id names. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CCameraCtrl.h"
#include "game/UnkCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU131.h"
#include "game/UnkTU2656.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CStrId;

struct UnkCamComp6_ovb5;
struct UnkCamCtrl_ovb5;
struct UnkCamComp8_ovb5;
struct UnkCamTarget2_ovb5;
struct UnkCameraCtrl_ovb0049;
struct UnkScreenFadeState_b48a3;
void fn_71010e5da4(UnkCamComp6_ovb5* self, UnkCamCtrl_ovb5* ctrl, bool flag, bool a, bool b);
void fn_71010e6278(UnkCamComp8_ovb5* self, UnkCamTarget2_ovb5* target, f32 a, f32 b, f32 time);
void fn_71010eea84(void* list, UnkCameraCtrl_ovb0049* const* ctrl);
bool fn_7101053a1c(UnkScreenFadeState_b48a3* self, f32 t);
void fn_7100145388(void* scene, void* value);
void fn_71001b7d80(void* params, f32 value);
void fn_71001bab1c(void* params, f32 value);

// The scene camera's parameters.
struct UnkCamParams_l0ed98c {
    u8 pad_0x0[0x74];
    f32 field_0x74;
    u8 pad_0x78[0x8];
    f32 field_0x80;  // 0x80
    u8 pad_0x84[0xc];
    f32 field_0x90;  // 0x90
    u8 pad_0x94[0x20];
    UnkVec3f field_0xb4;  // 0xb4
    u8 pad_0xc0[0x5c];
    UnkVec3f field_0x11c;  // 0x11c
};
static_assert(__builtin_offsetof(UnkCamParams_l0ed98c, field_0x11c) == 0x11c, "layout");

// The scene camera the controller drives (its node's +0xfb marks it changed).
struct UnkArg_fc {
    u8 pad_0x0[0x20];
    UnkXformNode xform;            // 0x20
    UnkCamParams_l0ed98c* params;  // 0x130

    void SetFov(f32 value) {
        fn_71001b7d80(params, value);
        xform.flag_0xfb = true;
    }
    void SetField0x90(f32 value) {
        params->field_0x90 = value;
        xform.flag_0xfb = true;
    }
    void SetField0x74(f32 value) {
        fn_71001bab1c(params, value);
        xform.flag_0xfb = true;
    }
};
static_assert(__builtin_offsetof(UnkArg_fc, params) == 0x130, "layout");

// What the controller saves of the scene camera.
struct UnkCamState_l0ed98c {
    UnkVec3f position;  // 0x0
    UnkVec3f target;    // 0xc
    UnkVec3f up;        // 0x18
    f32 fov;            // 0x24
    f32 field_0x28;
    f32 field_0x2c;
};

struct UnkCamDesc_l0ed98c {
    u8 pad_0x0[0x3c];
    f32 fov;  // 0x3c
};

struct UnkCamDescRef_l0ed98c {
    u8 pad_0x0[0x10];
    UnkCamDesc_l0ed98c* desc;  // 0x10
};

struct UnkCamCompRef_l0ed98c {
    u8 pad_0x0[0x10];
    void* component;  // 0x10
};

struct UnkFixedCamera_fc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(bool flag, f32 time);
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68();
    virtual void vf_0x70();
    virtual void vf_0x78();
    virtual void vf_0x80();
    virtual void vf_0x88();
    virtual void vf_0x90();
    virtual void vf_0x98();
    virtual void vf_0xa0();
    virtual void vf_0xa8();
    virtual void vf_0xb0();
    virtual void vf_0xb8();
    virtual void vf_0xc0();
    virtual void vf_0xc8();
    virtual void vf_0xd0();
    virtual void vf_0xd8();
    virtual void vf_0xe0();
    virtual void vf_0xe8();
    virtual void vf_0xf0();
    virtual void vf_0xf8(f32 time);

    u8 pad_0x8[0x10];
    UnkArg_fc* scene;  // 0x18
    u8 pad_0x20[0xc];
    u32 field_0x2c;
    u8 pad_0x30[0x4];
    bool field_0x34;
    u8 pad_0x35[0x3];
    UnkCamState_l0ed98c saved;  // 0x38
    u8 pad_0x68[0x24];
    f32 fov;  // 0x8c
    u8 pad_0x90[0x8];
    bool restore;  // 0x98
    u8 pad_0x99[0x17];
    UnkCamCompRef_l0ed98c* component;  // 0xb0
    UnkCamDescRef_l0ed98c* desc;       // 0xb8
    u8 pad_0xc0[0x8];
    CStrId script;  // 0xc8
    u8 pad_0xd0[0x8];
    u8 field_0xd8[0x8];  // 0xd8

    void* Component() const { return component ? component->component : nullptr; }

    void Save(UnkArg_fc* camera) {
        UnkVec3f position = fn_7100206154(&camera->xform);
        saved.position = position;
        saved.target = camera->params->field_0xb4;
        saved.up = camera->params->field_0x11c;
        saved.fov = camera->params->field_0x80;
        saved.field_0x28 = camera->params->field_0x90;
        saved.field_0x2c = camera->params->field_0x74;
    }

    void Restore() {
        if (UnkArg_fc* camera = scene) {
            fn_7100145388(camera, &saved.position);
            fn_71001453c4(reinterpret_cast<UnkSceneObj_71010e2278*>(scene),
                          reinterpret_cast<UnkArg2_71010e2278*>(&saved.target));
            scene->params->field_0x11c = saved.up;
            scene->SetFov(saved.fov);
            scene->SetField0x90(saved.field_0x28);
            scene->SetField0x74(saved.field_0x2c);
        }
    }
};
static_assert(__builtin_offsetof(UnkFixedCamera_fc, fov) == 0x8c, "layout");
static_assert(__builtin_offsetof(UnkFixedCamera_fc, field_0xd8) == 0xd8, "layout");

void fn_71010ed98c(UnkFixedCamera_fc* self, UnkArg_fc* arg, u32 flag) {
    self->vf_0x90();
    UnkCamDescRef_l0ed98c* desc = self->desc;
    if (desc && desc->desc) {
        self->fov = desc->desc->fov;
        if (void* component = self->Component())
            fn_71010e5da4(static_cast<UnkCamComp6_ovb5*>(component),
                          reinterpret_cast<UnkCamCtrl_ovb5*>(desc->desc), false, true, true);
    } else {
        self->fov = 45.0f;
    }
    self->field_0x34 = true;
    self->scene = arg;
    self->field_0x2c = 0;
    self->Save(arg);
    if (self->restore) {
        if (fn_71010e2080(self->Component()) == reinterpret_cast<UnkCameraCtrl*>(self))
            fn_71010e6278(static_cast<UnkCamComp8_ovb5*>(self->Component()),
                          reinterpret_cast<UnkCamTarget2_ovb5*>(self), 1.0f, -1.0f, 0.1f);
        self->vf_0x48(false, 0.0f);
        self->Save(arg);
    } else {
        if (flag & 1)
            self->vf_0x88();
        // Pushed back while the screen fade runs or is in its window.
        if (fn_7101053a0c(g_unk_7101d47310->field_0x1040) ||
            fn_7101053a1c(reinterpret_cast<UnkScreenFadeState_b48a3*>(g_unk_7101d47310->field_0x1040), 0.0f))
            self->Restore();
    }
    self->restore = false;
    self->vf_0xf8(0.0f);
    UnkCameraCtrl_ovb0049* me = reinterpret_cast<UnkCameraCtrl_ovb0049*>(self);
    fn_71010eea84(self->field_0xd8, &me);
    if (self->script.data != &g_unk_7101d47108->empty) {
        fn_7100000790(self->script.Text(), "o", self);
        self->script = g_unk_7101d470e0;
    }
}
