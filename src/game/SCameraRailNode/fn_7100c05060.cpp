// Resets of a variant holding a 0x58-byte record (state 1) or a plain value (state 2): the
// record's destructor is inlined. It releases the refcounted pointer of its member at 0x18,
// then destroys the member's base, an array of callback slots freed through its allocator.
// The record sits at `Record` with the state after it (fn_7100c05060 and its copies).
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkRefPtrMember;

extern char unk_7101aa2888[];  // the record's vtable
extern char unk_7101a20950[];  // the member's vtable
extern char unk_7101a20998[];  // the member's base's vtable

struct UnkCallbackSlot;
struct UnkCallbackOps {
    u8 unk_0x0[0x40];
    void (*destroy)(UnkCallbackSlot* slot);
};
struct UnkCallbackSlot {
    u64 storage;
    UnkCallbackOps* ops;
};

struct UnkVariantRecord {
    char* vtable;
    u8 unk_0x8[0x10];
    char* member_vtable;
    UnkCallbackSlot* slots;
    u32 count;
    u32 field_0x2c;
    u8 unk_0x30[0x10];
    UnkAllocator* allocator;
    u8 unk_0x48[0x8];
    UnkRefPtrMember ref;
    u32 state;
};

// Alt reads the state at its byte offset: another access path, so the two resets of
// fn_7100c051e8 are not merged (the original keeps them apart).
template <u32 Record, bool Alt = false>
__attribute__((always_inline)) static inline void ResetVariant(char* self) {
    UnkVariantRecord* r = reinterpret_cast<UnkVariantRecord*>(self + Record);
    u32 state = Alt ? *reinterpret_cast<u32*>(self + Record + 0x58) : r->state;
    switch (state) {
    case 1:
        r->vtable = unk_7101aa2888 + 0x10;
        r->member_vtable = unk_7101a20950 + 0x10;
        r->ref.~UnkRefPtrMember();
        r->member_vtable = unk_7101a20998 + 0x10;
        if (r->slots) {
            for (u64 i = 0; i < r->count; ++i) {
                UnkCallbackSlot* slot = &r->slots[i];
                if (slot->ops) {
                    slot->ops->destroy(slot);
                    slot->ops = nullptr;
                }
            }
            UnkCallbackSlot* slots = r->slots;
            r->count = 0;
            r->allocator->vf_0x48(slots);
            r->field_0x2c = 0;
        }
        r->state = 0;
        break;
    case 2:
        r->state = 0;
        break;
    }
}

void fn_7100c05060(char* self) { ResetVariant<0x18>(self); }
void fn_7100c0562c(char* self) { ResetVariant<0x18>(self); }
void fn_7100c08274(char* self) { ResetVariant<0x8>(self); }
void fn_7100c088f8(char* self) { ResetVariant<0x8>(self); }

// Sets the variant at 0x10 from the pointer at 8: a pair of this and the pointer (state 2), or
// empty without one (fn_7100c051e8 and its copy).
struct UnkVariantPair {
    char* owner;
    void* value;
};

__attribute__((always_inline)) static inline void SetVariant(char* self) {
    void* value = *reinterpret_cast<void**>(self + 8);
    if (value) {
        ResetVariant<0x18>(self);
        *reinterpret_cast<u32*>(self + 0x70) = 2;
        UnkVariantPair* pair = reinterpret_cast<UnkVariantPair*>(self + 0x10);
        pair->owner = self;
        pair->value = value;
    } else {
        ResetVariant<0x18, true>(self);
    }
}

void fn_7100c051e8(char* self) { SetVariant(self); }
void fn_7100c057b4(char* self) { SetVariant(self); }
