// CFreeCameraCtrl (the name from reflection): the plain constructor (fn_71010ef0ec) and the in-place one the
// reflection descriptor points to (fn_71010f0234, the same stores and then the post-construction virtual call,
// slot 3). After the camera controller base constructor (fn_71010ed044) they set the vtable, an engine array
// (flags 0xf) at +0x1d8, a flag and a pair of distances (1500, 2630), an empty id at +0x210, a list (flags 0x13)
// at +0x218, the input member at +0x250 (fn_71009ca9fc, with the class name and a flags word), clear the state at
// +0xbf0, set an angle of 45 and register with the render manager (fn_7100219dbc). Names and layouts are
// placeholders.
#include <new>

#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCameraCtrl.h"
#include "game/CInputComponent.h"
#include "game/CPhotoModeCameraCtrl.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2577.h"
#include "game/UnkTU2732.h"
#include "game/UnkTU315.h"
#include "types.h"

using base::global::CStrId;

struct UnkBase_7100763ed4;
struct UnkFreeCameraCtrl_b0058;
extern void* g_unk_7101cb1860[];
extern CStrId g_unk_7172a04a30 __attribute__((visibility("hidden")));
void fn_7100219dbc(UnkRenderManager* manager, const CStrId* key, bool on);
void fn_71010ed260(UnkFreeCameraCtrl_b0058* self);

// What the array at +0x1d8 holds: callbacks run (and dropped when empty) on deactivation.
struct UnkFreeCameraCallback_b0058 {
    void* context;
    void (*fn)(void* context);
};

struct UnkFreeCameraTarget_b0058 {
    u8 pad_0x0[0xe4];
    f32 field_0xe4;
    u8 pad_0xe8[0x270 - 0xe8];
    bool field_0x270;
};

struct UnkFreeCameraOwner_b0058 {
    u8 pad_0x0[0x10];
    UnkFreeCameraTarget_b0058* target;  // 0x10
};

struct UnkFreeCameraEntry_b0058 {
    UnkStrIdPathPair ids;
    UnkFreeCameraEntry_b0058* next;  // 0x10
};

struct UnkFreeCameraList_b0058 {
    void** vtable;
    u32 count;
    UnkFreeCameraEntry_b0058* head;
    UnkFreeCameraEntry_b0058* tail;
    base::UnkAllocator* allocator;
    u32 flags;
};

// The input state the frame's inputs are folded into (+0xbf0), cleared by the constructor as one block.
struct UnkFreeCameraState_b0058 {
    u8 field_0xbf0;
    u8 field_0xbf1;
    bool field_0xbf2;
    u8 field_0xbf3;
    u32 field_0xbf4;
    u32 field_0xbf8;
    u32 field_0xbfc;
    u32 field_0xc00;
    u8 unk_0xc04[0x9];
    u8 pad_0xc0d[3];
};

// One input of the frame: the key (bit), its kind (1 pressed, 2 released) and its value.
struct UnkFreeCameraInput_b0058 {
    u32 key;
    u32 kind;
    u32 field_0x8;
    u32 value;  // 0xc
    u32 field_0x10;
};

struct UnkFreeCameraCtrl_b0058 {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[0x34 - 0x2c];
    bool active;  // 0x34
    u8 pad_0x35[0xb0 - 0x35];
    UnkFreeCameraOwner_b0058* owner;  // 0xb0
    u8 pad_0xb8[0x1d8 - 0xb8];
    base::UnkVector<UnkFreeCameraCallback_b0058*> list;  // 0x1d8
    bool field_0x200;
    f32 field_0x204;
    f32 field_0x208;
    u32 field_0x20c;
    CStrId name;  // 0x210
    UnkFreeCameraList_b0058 entries;  // 0x218
    s32 field_0x248;
    u8 pad_0x24c[4];
    u8 member[0x34];  // 0x250
    bool field_0x284;
    u8 pad_0x285[3];
    UnkFreeCameraInput_b0058 inputs[120];  // 0x288
    u32 input_count;  // 0xbe8
    u8 pad_0xbec[4];
    UnkFreeCameraState_b0058 state;  // 0xbf0
    f32 field_0xc10;
};
struct UnkFreeCameraCtrlRaw_b0058 {
    void** vtable;
    u8 pad_0x8[0x20];
    u32 field_0x28;
    u8 pad_0x2c[0x34 - 0x2c];
    bool active;  // 0x34
    u8 pad_0x35[0xb0 - 0x35];
    UnkFreeCameraOwner_b0058* owner;  // 0xb0
    u8 pad_0xb8[0x1d8 - 0xb8];
    // The engine array at +0x1d8 as plain fields: the in-place constructor stores them one by one.
    void* list_data;  // 0x1d8
    u64 list_count;  // count and capacity
    u32 list_step;
    bool list_grow;
    u32 list_flags;
    base::UnkAllocator* list_allocator;
    bool field_0x200;
    f32 field_0x204;
    f32 field_0x208;
    u32 field_0x20c;
    CStrId name;  // 0x210
    UnkFreeCameraList_b0058 entries;  // 0x218
    s32 field_0x248;
    u8 pad_0x24c[4];
    u8 member[0x34];  // 0x250
    bool field_0x284;
    u8 pad_0x285[0xbf0 - 0x285];
    UnkFreeCameraState_b0058 state;  // 0xbf0
    f32 field_0xc10;
};
static_assert(__builtin_offsetof(UnkFreeCameraCtrl_b0058, field_0xc10) == 0xc10, "layout");

// The stores after the array, shared by both constructors.
template <class T>
static inline void ConstructRest(T* self) {
    self->field_0x200 = true;
    self->field_0x204 = 1500.0f;
    self->field_0x208 = 2630.0f;
    self->field_0x20c = 0;
    new (&self->name) CStrId(g_unk_710162e2ed, true);
    self->entries.vtable = &g_unk_7101a202e0[2];
    self->entries.count = 0;
    self->entries.head = nullptr;
    self->entries.tail = nullptr;
    self->entries.allocator = fn_7100083990();
    self->entries.flags = 0x13;
    self->field_0x248 = -1;
    fn_71009ca9fc(self->member, "FreeCameraCtrl", 0x03f00f33);
    __builtin_memset(&self->state, 0, 0x1d);  // up to +0xc0d
    self->field_0xc10 = 45.0f;
    self->field_0x28 = 6;
    self->field_0x284 = true;
    fn_7100219dbc(g_unk_7101d47300, &g_unk_7172a04a30, true);
}

void fn_71010ef0ec(UnkFreeCameraCtrl_b0058* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->vtable = &g_unk_7101cb1860[2];
    new (&self->list) base::UnkVector<UnkFreeCameraCallback_b0058*>(0xf);
    ConstructRest(self);
}

void fn_71010f0234(UnkFreeCameraCtrlRaw_b0058* self) {
    fn_71010ed044(reinterpret_cast<UnkBase_7100763ed4*>(self));
    self->vtable = &g_unk_7101cb1860[2];
    self->list_data = nullptr;
    self->list_count = 0;
    self->list_step = 10;
    self->list_grow = true;
    self->list_flags = 0xf;
    self->list_allocator = fn_7100083990();
    ConstructRest(self);
    reinterpret_cast<void (*)(UnkFreeCameraCtrlRaw_b0058*)>(self->vtable[3])(self);
}

// The destructor: the members in reverse order, then the base destructor (fn_71010ed260).
void fn_71010ef204(UnkFreeCameraCtrl_b0058* self) {
    self->vtable = &g_unk_7101cb1860[2];
    fn_710119ad3c(self->member);
    UnkFreeCameraList_b0058& entries = self->entries;
    entries.vtable = &g_unk_7101a202e0[2];
    for (UnkFreeCameraEntry_b0058* e = entries.head; e;) {
        UnkFreeCameraEntry_b0058* next = e->next;
        fn_710008d8e8(&e->ids);
        entries.allocator->vf_0x48(e);
        e = next;
    }
    entries.count = 0;
    entries.head = nullptr;
    entries.tail = nullptr;
    self->name.Reset();
    if (UnkFreeCameraCallback_b0058** data = self->list.data) {
        self->list.count = 0;
        self->list.allocator->vf_0x48(data);
        self->list.capacity = 0;
    }
    fn_71010ed260(self);
}

struct UnkManager;
struct UnkCameraEvent_b0058 {
    s32 kind;
    u32 unk_0x4;
};
void fn_7100eecf7c(void* manager, bool on, const UnkCameraEvent_b0058* event);
struct UnkGrabCameraCtrl;
extern CStrId g_unk_7172a04a38 __attribute__((visibility("hidden")));

// The sub-area manager, created on first use.
static inline void* SubAreaManager() {
    void* manager = g_unk_7101d47360;
    if (!manager) {
        manager = fn_71000839a0(0xae8, 0x3f);
        fn_7100ee1f34(manager);
        g_unk_7101d47360 = static_cast<UnkManager*>(manager);
    }
    return manager;
}

// Switches the flag at +0x200; when it changed while the camera is active (+0x34), the sub-area manager
// is told (fn_7100eecf7c, event 8).
void fn_71010f014c(UnkFreeCameraCtrl_b0058* self, bool on) {
    if (self->field_0x200 == on)
        return;
    self->field_0x200 = on;
    if (self->active) {
        void* manager = SubAreaManager();
        bool value = self->field_0x200;
        UnkCameraEvent_b0058 event;
        event.kind = 8;
        fn_7100eecf7c(manager, value, &event);
    }
}

// Deactivation: after the base step (fn_71010edd64), switches the sub-area manager's flag off again,
// resets +0x248, takes the target's angle, unregisters the input member, runs the callbacks of the
// array at +0x1d8 (dropping empty slots) and removes the camera key from the world.
void fn_71010ef41c(UnkFreeCameraCtrl_b0058* self) {
    fn_71010edd64(reinterpret_cast<UnkGrabCameraCtrl*>(self));
    if (self->field_0x200) {
        void* manager = SubAreaManager();
        UnkCameraEvent_b0058 event;
        event.kind = 8;
        fn_7100eecf7c(manager, false, &event);
    }
    self->field_0x248 = -1;
    self->field_0xc10 = self->owner->target->field_0xe4;
    fn_7100decc74(g_unk_7101d47310);
    if (!g_unk_7172729910) {
        void* p = fn_71000839a0(0x2208, 0x3f);
        fn_710119aee8(p);
        g_unk_7172729910 = p;
    }
    fn_710119ae24(g_unk_7172729910, self->member, true);
    u32 count = self->list.count;
    for (u32 i = 0; i < count; ++i) {
        if (*fn_71009cb3e0(&self->list, i)) {
            UnkFreeCameraCallback_b0058* callback = *fn_71009cb3e0(&self->list, i);
            callback->fn(callback->context);
        } else {
            if (i < self->list.count) {
                UnkFreeCameraCallback_b0058** at = &self->list.data[i];
                __builtin_memcpy(at, at + 1, (self->list.count - i - 1) * sizeof(*at));
                --self->list.count;
            }
            ++i;
        }
    }
    fn_7100e8f9a0(static_cast<char*>(g_unk_7101d47310->field_0x2228), true, &g_unk_7172a04a38);
}
