// Points the record at a new target: when the weak reference at +0x12a60 still refers to something, drops the
// two links at +0x12988 and +0x129a0 (each registered in a list of the previous target). Then it
// takes the weak handle of `target` for that reference, registers the links in the lists at +0x90 and
// +0x110 of the object the reference points at, and takes the weak handle of `target` again for
// the reference at +0x11978.
#include "game_s2/UnkWeakHandle.h"

struct UnkLinkList;
void fn_71002ae87c(UnkLinkList* list, UnkLinkList** slot);
void fn_71002aeebc(UnkLinkList* list, UnkLinkList** slot);

struct UnkLinkOwner : UnkWeakTarget {
    u8 pad_0x8[0x88];
    u8 field_0x90[0x80];
    u8 field_0x110[0x8];
};

struct UnkLinks {
    UnkLinkList* field_0x0;
    u8 pad_0x8[0x10];
    UnkLinkList* field_0x18;
    u8 pad_0x20[0xb8];
    UnkWeakHandle* field_0xd8;
};

struct UnkRecord {
    u8 pad_0x0[0x11978];
    UnkWeakHandle* field_0x11978;
    u8 pad_0x11980[0x12988 - 0x11980];
    UnkLinks field_0x12988;
};

static inline void Unlink(UnkLinkList** slot) {
    if (*slot) {
        fn_71002ae87c(*slot, slot);
        *slot = nullptr;
    }
}

static inline UnkLinkOwner* OwnerOf(UnkWeakHandle* handle) {
    return static_cast<UnkLinkOwner*>(handle->target);
}

void fn_71002ad688(UnkRecord* self, UnkWeakTarget* target) {
    UnkLinks* links = &self->field_0x12988;
    if (links->field_0xd8 && links->field_0xd8->target) {
        Unlink(&links->field_0x0);
        Unlink(&links->field_0x18);
    }
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(&links->field_0xd8, handle);
    if (handle) {
        if (UnkLinkOwner* owner = OwnerOf(handle)) {
            Unlink(&links->field_0x0);
            links->field_0x0 = reinterpret_cast<UnkLinkList*>(owner->field_0x90);
            fn_71002aeebc(links->field_0x0, &links->field_0x0);
            UnkLinkList** slot = &links->field_0x18;
            UnkWeakHandle* d8 = links->field_0xd8;
            UnkLinkOwner* o2 = d8 ? OwnerOf(d8) : nullptr;
            Unlink(slot);
            *slot = reinterpret_cast<UnkLinkList*>(o2->field_0x110);
            fn_71002aeebc(*slot, slot);
        }
    }
    AssignRef(&self->field_0x11978, target ? GetWeakHandle(target) : nullptr);
}
