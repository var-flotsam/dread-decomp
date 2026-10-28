// Variant of fn_71002ad688 for the links at +0x129e8 (three of them at +0x0, +0x18 and +0x30, and the
// weak reference at +0x80): the lists are those at +0x1828, +0x18a8 and +0x1928 of the object at
// +0x240 of the target.
#include "game_s2/UnkWeakHandle.h"

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);
void fn_71002af070(UnkLinkList* list, UnkLinkList** slot);

struct UnkLinkSource {
    u8 pad_0x0[0x1828];
    u8 field_0x1828[0x80];
    u8 field_0x18a8[0x80];
    u8 field_0x1928[0x8];
};

struct UnkLinkOwner : UnkWeakTarget {
    u8 pad_0x8[0x238];
    UnkLinkSource* field_0x240;
};

struct UnkLinks {
    UnkLinkList* field_0x0;
    u8 pad_0x8[0x10];
    UnkLinkList* field_0x18;
    u8 pad_0x20[0x10];
    UnkLinkList* field_0x30;
    u8 pad_0x38[0x48];
    UnkWeakHandle* field_0x80;
};

struct UnkRecord {
    u8 pad_0x0[0x129e8];
    UnkLinks field_0x129e8;
};

static inline void Unlink(UnkLinkList** slot) {
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
}

static inline UnkLinkOwner* OwnerOf(UnkWeakHandle* handle) {
    return static_cast<UnkLinkOwner*>(handle->target);
}

void fn_71002ad978(UnkRecord* self, UnkWeakTarget* target) {
    UnkLinks* links = &self->field_0x129e8;
    if (links->field_0x80 && links->field_0x80->target) {
        Unlink(&links->field_0x0);
        Unlink(&links->field_0x18);
        Unlink(&links->field_0x30);
    }
    UnkWeakHandle* handle = target ? GetWeakHandle(target) : nullptr;
    AssignRef(&links->field_0x80, handle);
    if (handle) {
        if (UnkLinkOwner* owner = OwnerOf(handle)) {
            if (UnkLinkSource* source = owner->field_0x240) {
                Unlink(&links->field_0x0);
                links->field_0x0 = reinterpret_cast<UnkLinkList*>(source->field_0x1828);
                fn_71002af070(links->field_0x0, &links->field_0x0);
                UnkLinkList** slot_b = &links->field_0x18;
                UnkLinkSource* source_b = OwnerOf(links->field_0x80)->field_0x240;
                Unlink(slot_b);
                *slot_b = reinterpret_cast<UnkLinkList*>(source_b->field_0x18a8);
                fn_71002af070(*slot_b, slot_b);
                UnkLinkList** slot_c = &links->field_0x30;
                UnkLinkSource* source_c = OwnerOf(links->field_0x80)->field_0x240;
                Unlink(slot_c);
                *slot_c = reinterpret_cast<UnkLinkList*>(source_c->field_0x1928);
                fn_71002af070(*slot_c, slot_c);
            }
        }
    }
}
