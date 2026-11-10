// CHearingSensor unit (unit 2580 of tools/tu_map.py): growing the sensor's array of 0x18-byte holders (a
// word and a counted shared object, as fn_7100f03818 destroys them): each holder is copied into the new
// storage (base vtable g_unk_7101caa330, then the holder's g_unk_7101caa308) and the old one destroyed through
// its vtable. Names and layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101caa330[];  // the holder's base vtable
extern char g_unk_7101caa308[];  // the holder's vtable

struct UnkHearingShared_b0129 {
    virtual ~UnkHearingShared_b0129();
    u8 pad_0x8[0x28];
    // A union member: the holder's pointer is reloaded after a count update.
    union {
        s32 ref_count;
    };
};

// The holder (as fn_7100f03818 destroys it): a word and a counted shared object.
struct UnkHearingHolder_b0129 {
    char* vtable;
    u32 value;
    UnkHearingShared_b0129* shared;

    __attribute__((always_inline)) UnkHearingHolder_b0129(const UnkHearingHolder_b0129& other) {
        vtable = g_unk_7101caa330 + 0x10;
        __builtin_memcpy(&value, &other.value, sizeof(value));
        vtable = g_unk_7101caa308 + 0x10;
        shared = nullptr;
        if (UnkHearingShared_b0129* p = other.shared) {
            ++p->ref_count;
            if (UnkHearingShared_b0129* old = shared) {
                if (--old->ref_count == 0)
                    delete old;
            }
            shared = p;
        }
    }
    // The virtual destructor (slot 0).
    void Destroy() { reinterpret_cast<void (*)(UnkHearingHolder_b0129*)>(*reinterpret_cast<void**>(vtable))(this); }
};

struct UnkHearingHolderArray_b0129 {
    UnkHearingHolder_b0129* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool growable;
    u32 flags;
    base::UnkAllocator* allocator;
};

// Grows the array to `n` holders, copying each over and destroying the old one.
void fn_7100f038f8(UnkHearingHolderArray_b0129* self, u32 n) {
    if (self->capacity < n) {
        auto* p = static_cast<UnkHearingHolder_b0129*>(
            self->allocator->vf_0x40((u64)n * sizeof(UnkHearingHolder_b0129), 8, self->flags));
        if (UnkHearingHolder_b0129* old = self->data) {
            UnkHearingHolder_b0129* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++old, ++dst) {
                new (dst) UnkHearingHolder_b0129(*old);
                old->Destroy();
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}
