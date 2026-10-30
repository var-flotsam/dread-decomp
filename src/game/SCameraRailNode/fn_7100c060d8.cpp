// Resets of a variant holding an array of 0x58-byte polymorphic records (state 1) or a plain
// value (state 2): the array's destructor is inlined. It destroys each record through its
// vtable, then frees the storage through its allocator. The array sits at `Array` with the
// state after it (fn_7100c060d8 and its copies), as in fn_7100c05060.
#include "base/UnkDestructors.h"
#include "types.h"

using base::UnkAllocator;

extern char unk_7101aa28d8[];  // the array's vtable (at 0x18, or at 0x8 in fn_7100c09b38's pair)

struct UnkRecord58 {
    virtual void vf_0x0();
    virtual ~UnkRecord58();
    u8 unk_0x8[0x50];
};

template <class Record>
struct UnkRecordArrayVariant {
    char* vtable;
    Record* data;
    u32 count;
    u32 capacity;
    u8 unk_0x18[0x10];
    UnkAllocator* allocator;
    u8 unk_0x30[0x20];
    u32 state;
};

// Alt reads the state at its byte offset, so the two resets of fn_7100c0621c are not merged.
template <u32 Array, char* Vtable, bool Alt = false, class Record = UnkRecord58>
__attribute__((always_inline)) static inline void ResetArrayVariant(char* self) {
    auto* v = reinterpret_cast<UnkRecordArrayVariant<Record>*>(self + Array);
    u32 state = Alt ? *reinterpret_cast<u32*>(self + Array + 0x50) : v->state;
    switch (state) {
    case 1:
        v->vtable = Vtable + 0x10;
        if (v->data) {
            for (u64 i = 0; i < v->count; ++i)
                v->data[i].~Record();
            Record* data = v->data;
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
void fn_7100c09b38(char* self) { ResetArrayVariant<0x8, unk_7101aa28d8>(self); }
void fn_7100c09ea4(char* self) { ResetArrayVariant<0x8, unk_7101aa28d8>(self); }

// Sets the variant at 0x10 from the pointer at 8: a pair of this and the pointer (state 2), or
// empty without one (fn_7100c0621c and its copy).
struct UnkVariantPair {
    char* owner;
    void* value;
};

template <char* Vtable = unk_7101aa28d8, class Record = UnkRecord58>
__attribute__((always_inline)) static inline void SetArrayVariant(char* self) {
    void* value = *reinterpret_cast<void**>(self + 8);
    if (value) {
        ResetArrayVariant<0x18, Vtable, false, Record>(self);
        *reinterpret_cast<u32*>(self + 0x68) = 2;
        UnkVariantPair* pair = reinterpret_cast<UnkVariantPair*>(self + 0x10);
        pair->owner = self;
        pair->value = value;
    } else {
        ResetArrayVariant<0x18, Vtable, true, Record>(self);
    }
}

void fn_7100c0621c(char* self) { SetArrayVariant(self); }
void fn_7100c06720(char* self) { SetArrayVariant(self); }

// The same for variants holding an array of 0x20-byte records, with their own vtable: the sets
// fn_710105b904 and fn_710105be08, the resets fn_710105e6ec and fn_710105ebac (the array at 0x8).
extern char unk_7101cac938[];  // the array's vtable (game_s2/UnkItem50.h names it g_unk_7101cac938)

struct UnkRecord20 {
    virtual void vf_0x0();
    virtual ~UnkRecord20();
    u8 unk_0x8[0x18];
};

void fn_710105b904(char* self) { SetArrayVariant<unk_7101cac938, UnkRecord20>(self); }
void fn_710105be08(char* self) { SetArrayVariant<unk_7101cac938, UnkRecord20>(self); }
void fn_710105e6ec(char* self) { ResetArrayVariant<0x8, unk_7101cac938, false, UnkRecord20>(self); }
void fn_710105ebac(char* self) { ResetArrayVariant<0x8, unk_7101cac938, false, UnkRecord20>(self); }
// Copies next door, with the array at 0x18.
void fn_710105b7c0(char* self) { ResetArrayVariant<0x18, unk_7101cac938, false, UnkRecord20>(self); }
void fn_710105bcc4(char* self) { ResetArrayVariant<0x18, unk_7101cac938, false, UnkRecord20>(self); }
