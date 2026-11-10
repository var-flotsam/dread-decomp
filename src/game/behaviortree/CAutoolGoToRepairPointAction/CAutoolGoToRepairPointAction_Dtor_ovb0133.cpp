// behaviortree::CAutoolGoToRepairPointAction's unit (tools/tu_map.py): the action's destructor (vtable
// g_unk_7101a2cf58). The listener at +0x380 (vtable g_unk_7101a22d48) leaves its target's list
// (fn_710117b3f0) and drops its link (+0x398); the object at +0x2d0 (vtable g_unk_7101a2d008, built by
// fn_71003c2468) is cleared; the six value holders from +0x260 down to +0x30 (vtable g_unk_7101a2d0a0,
// as fn_71003c2484) are destroyed; then the base destructor fn_7100393ea0 runs. Names and layouts are
// placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/reflection/UnkReflCast.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern void* g_unk_7101a2cf58[];
extern void* g_unk_7101a22d48[];
extern void* g_unk_7101a2d008[];
extern char g_unk_7101a2d0a0[];

struct UnkAutoolValueTag_ovb0133;
template <>
struct base::UnkValueHolderTraits<UnkAutoolValueTag_ovb0133> {
    static char* vtable() { return g_unk_7101a2d0a0; }
};
using UnkAutoolValue_ovb0133 = base::UnkValueHolder<UnkAutoolValueTag_ovb0133>;

struct UnkAutoolLink_ovb0133 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Release();  // 0x10
    u32 ref_count;  // 0x8
    void* owner;  // 0x10
};

struct UnkAutoolListener_ovb0133 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkAutoolLink_ovb0133* link;  // 0x18

    ~UnkAutoolListener_ovb0133() {
        vtable = &g_unk_7101a22d48[2];
        UnkAutoolLink_ovb0133* l = link;
        if (!l)
            return;
        if (void* owner = l->owner) {
            fn_710117b3f0(static_cast<UnkWeakTarget*>(owner), this);
            l = link;
            if (!l)
                return;
        }
        if (--l->ref_count == 0)
            l->Release();
    }
};

struct UnkAutoolCleared_ovb0133 {
    void** vtable;
    u8 pad_0x8[8];
    u64 field_0x10;
    u64 field_0x18;
    u32 field_0x20;

    ~UnkAutoolCleared_ovb0133() {
        vtable = &g_unk_7101a2d008[2];
        field_0x10 = 0;
        field_0x18 = 0;
        field_0x20 = 0;
    }
};

struct UnkAutoolGoToRepairPoint_ovb0133 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkAutoolValue_ovb0133 values[6];  // 0x30
    UnkAutoolCleared_ovb0133 cleared;  // 0x2d0
    u8 pad_0x2f8[0x88];
    UnkAutoolListener_ovb0133 listener;  // 0x380
};
static_assert(__builtin_offsetof(UnkAutoolGoToRepairPoint_ovb0133, listener) == 0x380, "layout");

void fn_7100393ea0(void*);

void fn_71003c22d4(UnkAutoolGoToRepairPoint_ovb0133* self) {
    self->vtable = &g_unk_7101a2cf58[2];
    self->listener.~UnkAutoolListener_ovb0133();
    self->cleared.~UnkAutoolCleared_ovb0133();
    for (int i = 5; i >= 0; --i)
        base::refl_ValueHolderDtor(&self->values[i]);
    fn_7100393ea0(self);
}
