// Refreshes a list of child items: counts the items of kind 6 or 5 (looking through a
// kind-7 wrapper to its target), then, if any were found or `all` is set, sets a flag on every
// item (or only on those that accept it) that is true when fewer than two were found and
// `force_off` is clear; with two or more, also runs a follow-up. Finally updates two attached
// components of the owner.
#include "types.h"

struct UnkTarget_71002fda50 {
    u8 unk_0x0[0x10];
    s32 kind;
};

struct UnkItem_71002fda50 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
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
    virtual void vf_0xa0(bool value);
    virtual void vf_0xa8();
    virtual bool vf_0xb0();

    u8 unk_0x8[0x18];
    UnkTarget_71002fda50* target;
    u8 unk_0x28[0x8c];
    s32 kind;  // 0xb4

    // Whether this item (or, for a kind-7 wrapper, its target) has the given kind.
    bool IsKind(s32 k) const {
        s32 effective = kind;
        if (kind == 7) {
            if (!target)
                return false;
            effective = target->kind;
        }
        return effective == k;
    }
};

struct UnkComponentA_71002fda50;
struct UnkComponentB_71002fda50;
void fn_710113f2f4(UnkComponentA_71002fda50* self);
void fn_710094f974(UnkComponentB_71002fda50* self);

struct UnkOwner_71002fda50 {
    u8 unk_0x0[0x210];
    UnkComponentB_71002fda50* field_0x210;
    u8 unk_0x218[0x48];
    UnkComponentA_71002fda50* field_0x260;
};

struct UnkStruct_71002fda50 {
    u8 unk_0x0[0x30];
    UnkOwner_71002fda50* owner;
    u8 unk_0x38[8];
    UnkItem_71002fda50** items;
    u32 count;
};

void fn_71003014e0(UnkStruct_71002fda50* self, void* arg);

void fn_71002fda50(UnkStruct_71002fda50* self, bool all, bool force_off) {
    UnkItem_71002fda50** begin = self->items;
    UnkItem_71002fda50** end = begin + self->count;
    u32 found = 0;
    for (UnkItem_71002fda50** it = begin; it != end; ++it) {
        if ((*it)->IsKind(6) || (*it)->IsKind(5))
            ++found;
    }
    if (found != 0 || all) {
        bool single = found < 2;
        bool value = single && !force_off;
        for (UnkItem_71002fda50** it = begin; it != end; ++it) {
            UnkItem_71002fda50* item = *it;
            if (all || item->vf_0xb0())
                item->vf_0xa0(value);
        }
        if (!single)
            fn_71003014e0(self, nullptr);
    }
    if (UnkComponentA_71002fda50* a = self->owner->field_0x260)
        fn_710113f2f4(a);
    if (UnkComponentB_71002fda50* b = self->owner->field_0x210)
        fn_710094f974(b);
}
