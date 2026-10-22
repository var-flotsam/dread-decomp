// Resets of a variant holding an array of 0x58-byte polymorphic records (state 1) or a plain
// value (state 2): the array's destructor is inlined. It destroys each record through its
// vtable, then frees the storage through its allocator. The array sits at `Array` with the
// state after it (fn_7100c060d8 and its copies), as in fn_7100c05060.
#include "base/UnkDestructors.h"
#include "types.h"

using base::UnkAllocator;

extern char unk_7101aa28d8[];  // the array's vtable (at 0x18)
extern char unk_7101a9d4b0[];  // the array's vtable (at 0x8)

struct UnkRecord58 {
    virtual void vf_0x0();
    virtual ~UnkRecord58();
    u8 unk_0x8[0x50];
};

struct UnkRecordArrayVariant {
    char* vtable;
    UnkRecord58* data;
    u32 count;
    u32 capacity;
    u8 unk_0x18[0x10];
    UnkAllocator* allocator;
    u8 unk_0x30[0x20];
    u32 state;
};

// Alt reads the state at its byte offset, so the two resets of fn_7100c0621c are not merged.
template <u32 Array, char* Vtable, bool Alt = false>
__attribute__((always_inline)) static inline void ResetArrayVariant(char* self) {
    UnkRecordArrayVariant* v = reinterpret_cast<UnkRecordArrayVariant*>(self + Array);
    u32 state = Alt ? *reinterpret_cast<u32*>(self + Array + 0x50) : v->state;
    switch (state) {
    case 1:
        v->vtable = Vtable + 0x10;
        if (v->data) {
            for (u64 i = 0; i < v->count; ++i)
                v->data[i].~UnkRecord58();
            UnkRecord58* data = v->data;
            v->count = 0;
            v->allocator->vf_0x48(data);
            v->capacity = 0;
        }
        v->state = 0;
        break;
    case 2:
        v->state = 0;
        break;
    }
}

void fn_7100c060d8(char* self) { ResetArrayVariant<0x18, unk_7101aa28d8>(self); }
void fn_7100c065dc(char* self) { ResetArrayVariant<0x18, unk_7101aa28d8>(self); }
void fn_7100c09b38(char* self) { ResetArrayVariant<0x8, unk_7101a9d4b0>(self); }
void fn_7100c09ea4(char* self) { ResetArrayVariant<0x8, unk_7101a9d4b0>(self); }

// Sets the variant at 0x10 from the pointer at 8: a pair of this and the pointer (state 2), or
// empty without one (fn_7100c0621c and its copy).
struct UnkVariantPair {
    char* owner;
    void* value;
};

__attribute__((always_inline)) static inline void SetArrayVariant(char* self) {
    void* value = *reinterpret_cast<void**>(self + 8);
    if (value) {
        ResetArrayVariant<0x18, unk_7101aa28d8>(self);
        *reinterpret_cast<u32*>(self + 0x68) = 2;
        UnkVariantPair* pair = reinterpret_cast<UnkVariantPair*>(self + 0x10);
        pair->owner = self;
        pair->value = value;
    } else {
        ResetArrayVariant<0x18, unk_7101aa28d8, true>(self);
    }
}

void fn_7100c0621c(char* self) { SetArrayVariant(self); }
void fn_7100c06720(char* self) { SetArrayVariant(self); }
