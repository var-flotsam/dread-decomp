// Unit 42: two more passes of the event dispatch of src/base/UnkEventDispatch.cpp (fn_7100068bec),
// each over one of the set's listener lists: fn_7100068b40 over the list at +0x50 (pass flag 0x20),
// fn_7100068e08 over the list at +0xa0 (pass flag 0x10), and fn_7100068a40 over the list at +0x28 (pass
// flag 4; with `exclusive`, listeners with 0x200 are skipped). The types repeat that file's layouts under
// this file's own names. Names are placeholders.
#include "types.h"

struct UnkEventDataA1 {
    u64 field_0x0;
    u64 field_0x8;

    UnkEventDataA1(const UnkEventDataA1& other) : field_0x0(other.field_0x0), field_0x8(other.field_0x8) {}
};

struct UnkEventListenerA1 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18(UnkEventDataA1 data);
    union {
        u32 flags;  // 0x200: skipped by exclusive passes
        u8 kinds;
    };
};

struct UnkEventListA1 {
    UnkEventListenerA1** items;
    u32 count;
};

struct UnkEventSetA1 {
    u8 pad_0x0[0x28];
    UnkEventListA1 list_0x28;  // +0x28
    u8 pad_0x38[0x18];
    UnkEventListA1 list_0x50;  // +0x50
    u8 pad_0x60[0x40];
    UnkEventListA1 list_0xa0;  // +0xa0
};

struct UnkEventDispatchA1 {
    UnkEventSetA1* set;
    UnkEventDataA1 data;
    UnkEventListenerA1** first;
    u8 flags;
    u8 pad_0x21[0x3];
    u8 mask;
    u8 pad_0x25[0x3];
    bool enabled;
};

static inline void SendA1(UnkEventDispatchA1* self, UnkEventListenerA1* listener, u8 pass) {
    if (self->enabled && (self->flags & pass) && (listener->kinds & (u8)~self->mask) == 0)
        listener->vf_0x18(self->data);
}

static inline void SendAllA1(UnkEventDispatchA1* self, UnkEventListA1 UnkEventSetA1::*list, u8 pass) {
    UnkEventSetA1* set = self->set;
    if (!set)
        return;
    UnkEventListA1& l = set->*list;
    for (UnkEventListenerA1 **it = l.items, **end = l.items + l.count; it != end; ++it)
        SendA1(self, *it, pass);
}

void fn_7100068b40(UnkEventDispatchA1* self) {
    SendAllA1(self, &UnkEventSetA1::list_0x50, 0x20);
}

void fn_7100068e08(UnkEventDispatchA1* self) {
    SendAllA1(self, &UnkEventSetA1::list_0xa0, 0x10);
}

void fn_7100068a40(UnkEventDispatchA1* self, bool exclusive) {
    UnkEventSetA1* set = self->set;
    if (!set)
        return;
    UnkEventListA1& l = set->list_0x28;
    for (UnkEventListenerA1 **it = l.items, **end = l.items + l.count; it != end; ++it) {
        if (exclusive && ((*it)->flags & 0x200))
            continue;
        SendA1(self, *it, 4);
    }
}
