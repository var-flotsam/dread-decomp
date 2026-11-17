// CRedenkiSwarmControllerComponent's unit: the component's constructor. The vtable is stored with the
// base's constructor; then a zeroed group holding one engine array, two more arrays (flags 0xf) and the
// remaining fields. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/CRedenkiSwarmControllerComponent.h"
#include "types.h"

struct UnkRedenkiSwarm_b46a2;
void fn_7100737d58(UnkRedenkiSwarm_b46a2* object);

struct UnkSwarmCtrlBase_l64effc {
    void** vtable;
    u8 pad_0x8[0x468 - 0x8];

    UnkSwarmCtrlBase_l64effc() {
        fn_7100737d58(reinterpret_cast<UnkRedenkiSwarm_b46a2*>(this));
        vtable = reinterpret_cast<void**>(g_unk_7101a5af00 + 0x10);
    }
};

struct UnkSwarmGroup_l64effc {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    base::global::CRntVector<void*> list = base::global::CRntVector<void*>(0xf);
    u64 field_0x48 = 0;
    u64 field_0x50 = 0;
    u64 field_0x58 = 0;
    u64 field_0x60 = 0;
    u32 field_0x68 = 0;
};

struct UnkSwarmCtrl_l64effc : UnkSwarmCtrlBase_l64effc {
    UnkSwarmGroup_l64effc group;
    base::global::CRntVector<void*> lists[2];
    bool field_0x528 = false;
    u8 pad_0x529[7];
    u32 field_0x530 = 0;
    f32 field_0x534 = -1.0f;
    u64 field_0x538 = 0;
    u64 field_0x540 = 0;
    u64 field_0x548 = 0;
    u32 field_0x550 = 0;
    u8 field_0x554 = 0;
    f32 field_0x558 = -1.0f;
    u64 field_0x560 = 0;
    f32 field_0x568 = -1.0f;
    u32 field_0x56c = 0;

    UnkSwarmCtrl_l64effc()
        : group(), lists{base::global::CRntVector<void*>(0xf), base::global::CRntVector<void*>(0xf)} {}
};

static_assert(__builtin_offsetof(UnkSwarmCtrl_l64effc, group) == 0x468, "group");
static_assert(__builtin_offsetof(UnkSwarmCtrl_l64effc, lists) == 0x4d8, "lists");
static_assert(__builtin_offsetof(UnkSwarmCtrl_l64effc, field_0x528) == 0x528, "flag");
static_assert(__builtin_offsetof(UnkSwarmCtrl_l64effc, field_0x568) == 0x568, "tail");
static_assert(__builtin_offsetof(UnkSwarmCtrl_l64effc, field_0x530) == 0x530, "pair");

void fn_710064effc(UnkSwarmCtrl_l64effc* self) {
    new (self) UnkSwarmCtrl_l64effc;
}
