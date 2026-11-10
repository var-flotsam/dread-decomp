// CPhotoModeCameraCtrl's unit (tools/tu_map.py unit 1490): fn_71009cb260 is the photo mode camera's
// deactivation, the shape of CFreeCameraCtrl's (fn_71010ef41c in
// src/game/CFreeCameraCtrl/CFreeCameraCtrl_Ctor_ovb0058.cpp): the scene object (+0x18) gets the saved
// values back (+0xbc8, +0xbd4), then the base step (fn_71010edd64), the target's angle is taken, the
// input member (+0x210) is unregistered, the callbacks of the array at +0x1d8 run (dropping empty
// slots), the camera is marked done (+0xbe0), the render manager is told (fn_7100219d4c) and the
// camera key is removed from the world. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "types.h"

using base::global::CStrId;

struct UnkFreeCameraCallback_b0058;
struct UnkSceneObj_71010e2278;
struct UnkArg2_71010e2278;
void fn_7100145388(void* scene, void* value);
void fn_71001453c4(UnkSceneObj_71010e2278*, UnkArg2_71010e2278*);
struct UnkGrabCameraCtrl;
void fn_71010edd64(UnkGrabCameraCtrl*);
bool fn_7100decc5c(UnkGameManager*);
void fn_7100decc74(UnkGameManager*);
extern void* g_unk_7172729910;
void fn_710119aee8(void*);
void fn_710119ae24(void*, void*, bool);
UnkFreeCameraCallback_b0058** fn_71009cb3e0(base::UnkVector<UnkFreeCameraCallback_b0058*>*, u32);
void fn_7100e8f9a0(char*, bool, base::global::CStrId*);
extern CStrId g_unk_717273f970 __attribute__((visibility("hidden")));

struct UnkPhotoCallback_ovb0134 {
    void* context;
    void (*fn)(void* context);
};

struct UnkPhotoTarget_ovb0134 {
    u8 pad_0x0[0xe4];
    u32 angle;  // 0xe4
};

struct UnkPhotoOwner_ovb0134 {
    u8 pad_0x0[0x10];
    UnkPhotoTarget_ovb0134* target;  // 0x10
};

struct UnkPhotoModeCameraCtrl_ovb0134 {
    u8 pad_0x0[0x18];
    void* scene;  // 0x18
    u8 pad_0x20[0x90];
    UnkPhotoOwner_ovb0134* owner;  // 0xb0
    u8 pad_0xb8[0x120];
    base::UnkVector<UnkPhotoCallback_ovb0134*> list;  // 0x1d8
    u8 pad_0x200[0x10];
    u8 member[0x9b8];   // 0x210
    u8 saved_a[0xc];    // 0xbc8
    u8 saved_b[0xc];    // 0xbd4
    bool done;          // 0xbe0
    u8 pad_0xbe1[0x3];
    u32 angle;          // 0xbe4
};
static_assert(__builtin_offsetof(UnkPhotoModeCameraCtrl_ovb0134, angle) == 0xbe4, "layout");

static inline UnkPhotoCallback_ovb0134** At(UnkPhotoModeCameraCtrl_ovb0134* self, u32 i) {
    return reinterpret_cast<UnkPhotoCallback_ovb0134**>(fn_71009cb3e0(
        reinterpret_cast<base::UnkVector<UnkFreeCameraCallback_b0058*>*>(&self->list), i));
}

void fn_71009cb260(UnkPhotoModeCameraCtrl_ovb0134* self) {
    if (self->scene) {
        fn_7100145388(self->scene, self->saved_a);
        fn_71001453c4(static_cast<UnkSceneObj_71010e2278*>(self->scene),
                      reinterpret_cast<UnkArg2_71010e2278*>(self->saved_b));
    }
    fn_71010edd64(reinterpret_cast<UnkGrabCameraCtrl*>(self));
    self->angle = self->owner->target->angle;
    if (fn_7100decc5c(g_unk_7101d47310))
        fn_7100decc74(g_unk_7101d47310);
    if (!g_unk_7172729910) {
        void* p = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(p);
        g_unk_7172729910 = p;
    }
    fn_710119ae24(g_unk_7172729910, self->member, true);
    u32 count = self->list.count;
    for (u32 i = 0; i < count; ++i) {
        if (*At(self, i)) {
            UnkPhotoCallback_ovb0134* callback = *At(self, i);
            callback->fn(callback->context);
        } else {
            if (i < self->list.count) {
                UnkPhotoCallback_ovb0134** at = &self->list.data[i];
                __builtin_memcpy(at, at + 1, (self->list.count - i - 1) * sizeof(*at));
                --self->list.count;
            }
            ++i;
        }
    }
    self->done = true;
    fn_7100219d4c(g_unk_7101d47300, true, 2);
    fn_7100e8f9a0(static_cast<char*>(g_unk_7101d47310->field_0x2228), self->done, &g_unk_717273f970);
}
