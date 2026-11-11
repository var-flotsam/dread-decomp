// CCameraCtrl (tools/tu_map.py unit 2693; the name from reflection): the camera controller's destructor
// body (fn_71010ed260), its set-up from a target, a name and a pose (fn_71010ed5d4), the actor it follows
// (fn_71010ed7e8), the reset that calls the script function named at +0xd0 (fn_71010edd64), the pose
// snapshot (fn_71010ee0f8) and a distance factor towards the player (fn_71010ee560). Names and layouts
// are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCameraCtrl.h"
#include "game/CGameObject.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager*);

struct UnkCameraVec3_ovb0049 {
    f32 x, y, z;
};

struct UnkCameraVec4_ovb0049 {
    f32 x, y, z, w;
};

// A camera pose: position, target, orientation and two scalars.
struct UnkCameraPose_ovb0049 {
    UnkCameraVec3_ovb0049 position;
    UnkCameraVec3_ovb0049 target;
    UnkCameraVec4_ovb0049 rotation;  // w: the field of view (45 by default)
    f32 field_0x28;
    f32 field_0x2c;
};

struct UnkCameraTarget_ovb0049 {
    u8 pad_0x0[0x3c];
    f32 field_0x3c;
    u8 pad_0x40[0x21c];
    f32 scale;  // 0x25c
    f32 time;  // 0x260
    u8 pad_0x264[0x9dc];
    f32 field_0xc40;
};
static_assert(__builtin_offsetof(UnkCameraTarget_ovb0049, field_0xc40) == 0xc40, "layout");

// A string id's entry as its text is read: the text at +0x10 and a count at +0x44 that each read
// bumps (a union member: the original reloads the id's data after it).
struct UnkStringEntry_ovb0049 {
    u8 pad_0x0[0x10];
    const char* text;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        s32 readers;  // 0x44
    };
};

// Three arrays (growth step 10, allocator flags 0xf) and a flag.
struct UnkCameraLists_ovb0049 {
    base::UnkVector<void*> field_0x0;
    base::UnkVector<void*> field_0x28;
    base::UnkVector<void*> field_0x50;
    bool field_0x78;
};
static_assert(sizeof(UnkCameraLists_ovb0049) == 0x80, "lists layout");

struct UnkCameraCtrl_ovb0049 {
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
    UnkCameraPose_ovb0049 previous;  // 0x38
    UnkCameraPose_ovb0049 current;   // 0x68
    bool field_0x98;
    u8 pad_0x99[0x3];
    s32 mode;  // 0x9c
    f32 blend;  // 0xa0
    f32 period;  // 0xa4
    f32 field_0xa8;
    bool field_0xac;
    u8 pad_0xad[0x3];
    UnkWeakHandle* handle;  // 0xb0
    UnkWeakHandle* follow;  // 0xb8
    f32 field_0xc0;
    f32 field_0xc4;
    CStrId field_0xc8;
    CStrId on_reset;  // 0xd0, the script function called with the controller
    UnkCameraLists_ovb0049 field_0xd8;
    UnkCameraLists_ovb0049 field_0x158;  // the controller is told to it (fn_71010eea84)

    void vf_0x90() { reinterpret_cast<void (*)(UnkCameraCtrl_ovb0049*)>(vtable[0x90 / 8])(this); }

    UnkCameraTarget_ovb0049* Target() {
        return reinterpret_cast<UnkCameraTarget_ovb0049*>(handle ? handle->target : nullptr);
    }

    void vf_0x48(f32 time, bool flag) {
        reinterpret_cast<void (*)(UnkCameraCtrl_ovb0049*, f32, bool)>(vtable[0x48 / 8])(this, time, flag);
    }
    void vf_0xf0(f32 time) {
        reinterpret_cast<void (*)(UnkCameraCtrl_ovb0049*, f32)>(vtable[0xf0 / 8])(this, time);
    }
};
static_assert(__builtin_offsetof(UnkCameraCtrl_ovb0049, handle) == 0xb0, "layout");
static_assert(__builtin_offsetof(UnkCameraCtrl_ovb0049, on_reset) == 0xd0, "layout");
static_assert(__builtin_offsetof(UnkCameraCtrl_ovb0049, field_0x158) == 0x158, "layout");

void fn_71010eea84(void* list, UnkCameraCtrl_ovb0049* const* ctrl);
void fn_71010e6470(UnkCameraTarget_ovb0049* target, UnkCameraCtrl_ovb0049* ctrl, bool flag, f32 value, f32 fallback);

void fn_71010edd64(UnkCameraCtrl_ovb0049* self) {
    self->field_0xac = false;
    self->field_0x34 = false;
    self->field_0x2c = 0.0f;
    self->field_0x18 = 0;
    self->vf_0x90();
    UnkCameraCtrl_ovb0049* ctrl = self;
    fn_71010eea84(&self->field_0x158, &ctrl);
    if (self->on_reset.data != &g_unk_7101d47108->empty) {
        reinterpret_cast<UnkStringEntry_ovb0049*>(self->on_reset.data)->readers++;
        const char* name = reinterpret_cast<UnkStringEntry_ovb0049*>(self->on_reset.data)->text;
        fn_7100000790(name ? name : g_unk_710162e2ed, "o", self);
        self->on_reset = g_unk_7101d470e0;
    }
    if (reinterpret_cast<UnkCameraTarget_ovb0049*>(self->handle->target)->field_0xc40 > 0.0f)
        fn_71010e6470(self->Target(), self, true, self->field_0xc0, -1.0f);
}

f32 fn_71010e49c8(UnkCameraTarget_ovb0049* target, UnkPlayer* player, UnkCameraPose_ovb0049* pose, f32 fallback);

f32 fn_71010ee560(UnkCameraCtrl_ovb0049* self) {
    if (!self->Target())
        return 0.0f;
    f32 value;
    if (UnkPlayer* player = fn_7100dec788(g_unk_7101d47310))
        value = fn_71010e49c8(self->Target(), player, &self->current, -1.0f);
    else
        value = -1.0f;
    return value < 1.0f ? 1.0f : value;
}

void fn_71010ee0f8(UnkCameraCtrl_ovb0049* self) {
    self->field_0x2c = 0.0f;
    self->vf_0x48(0.0f, true);
    self->vf_0xf0(0.0f);
    self->previous.position = self->current.position;
    self->previous.target = self->current.target;
    self->previous.rotation = self->current.rotation;
    self->previous.field_0x28 = self->current.field_0x28;
    self->previous.field_0x2c = self->current.field_0x2c;
}

void fn_71010ed7e8(UnkCameraCtrl_ovb0049* self, UnkWeakTarget* target) {
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(&self->follow, handle);
    if (handle && handle->target)
        self->current.rotation.w = reinterpret_cast<UnkCameraTarget_ovb0049*>(handle->target)->field_0x3c;
    else
        self->current.rotation.w = 45.0f;
}

static inline void CopyVec(UnkCameraVec3_ovb0049* dst, const UnkCameraVec3_ovb0049* src) {
    dst->x = src->x;
    dst->y = src->y;
    dst->z = src->z;
}

void fn_71010ed5d4(UnkCameraCtrl_ovb0049* self, UnkWeakTarget* target, const CStrId* name,
                   const UnkCameraVec3_ovb0049* position, const UnkCameraVec3_ovb0049* look, f32 fov, f32 a, f32 b) {
    AssignRef(&self->handle, target ? GetWeakHandle(target) : nullptr);
    self->name = *name;
    CopyVec(&self->current.position, position);
    CopyVec(&self->previous.position, &self->current.position);
    CopyVec(&self->current.target, look);
    CopyVec(&self->previous.target, &self->current.target);
    self->current.rotation.w = fov;
    self->previous.rotation.w = fov;
    self->current.field_0x28 = a;
    self->previous.field_0x28 = a;
    self->current.field_0x2c = b;
    self->previous.field_0x2c = b;
    self->field_0x30 = fov;
}

// Releases a string id in two steps (the count is reloaded after the decrement) and leaves the
// pool's empty id.
static inline void ReleaseId(CStrId* id) {
    if (UnkStringPool* pool = g_unk_7101d47108) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                fn_71000803a4(pool, id->data);
                pool = g_unk_7101d47108;
            }
        }
        id->data = &pool->empty;
    }
}

static inline void ReleaseRef(UnkWeakHandle* ref) {
    if (ref) {
        if (--ref->ref_count == 0)
            ref->vf_0x10();
    }
}

void fn_71010ed3b4(void* member);
extern char g_unk_7101cb1720[];

void fn_71010ed260(UnkCameraCtrl_ovb0049* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101cb1720 + 0x10);
    fn_71010ed3b4(&self->field_0xd8);
    ReleaseId(&self->on_reset);
    ReleaseId(&self->field_0xc8);
    ReleaseRef(self->follow);
    ReleaseRef(self->handle);
    ReleaseId(&self->name);
    fn_7101043934(self);
}
