// Unit 2508: erase from a fixed-capacity vector of 0x48-byte entries held in raw storage after its count:
// later entries move down one place, then the vector is resized to one fewer. Returns the erase position.
// Names and layouts are placeholders.
#include <new>

#include "game/UnkAssignedSlot.h"
#include "types.h"

struct UnkRefCounted_ld4db00 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();  // 0x10
    u32 refs;  // 0x8
};

static inline void Release_ld4db00(UnkAssignedSlot& slot) {
    UnkRefCounted_ld4db00* obj = static_cast<UnkRefCounted_ld4db00*>(slot.ptr);
    if (obj && --obj->refs == 0) {
        obj->Destroy();
    }
}

struct UnkKeyed_ld4db00 {
    s32 id = -1;
    u32 field_0x4 = 0;
    u32 field_0x8 = 0;
};

struct UnkTriple_ld4db00 {
    u32 field_0x0 = 0;
    u32 field_0x4 = 0;
    u32 field_0x8 = 0;
};

struct UnkQuad_ld4db00 {
    u64 field_0x0 = 0;
    u64 field_0x8 = 0;
};

struct UnkEntry_ld4db00 {
    UnkAssignedSlot first = {nullptr};  // 0x0
    UnkAssignedSlot second = {nullptr};  // 0x8
    UnkKeyed_ld4db00 field_0x10;
    UnkTriple_ld4db00 field_0x1c;
    UnkQuad_ld4db00 field_0x28;
    f32 field_0x38 = 0;
    f32 field_0x3c = 0;
    f32 field_0x40 = 0;

    UnkEntry_ld4db00() = default;
    UnkEntry_ld4db00& operator=(const UnkEntry_ld4db00& other) {
        first = other.first;
        second = other.second;
        field_0x10 = other.field_0x10;
        field_0x1c = other.field_0x1c;
        field_0x28 = other.field_0x28;

        field_0x40 = other.field_0x40;
        field_0x3c = other.field_0x3c;
        field_0x38 = other.field_0x38;
        return *this;
    }
    ~UnkEntry_ld4db00() {
        Release_ld4db00(second);
        Release_ld4db00(first);
    }
};
static_assert(sizeof(UnkEntry_ld4db00) == 0x48, "layout");

struct UnkEntryVector_ld4db00 {
    u32 count;
    u8 storage[0x48 * 8];  // capacity unknown

    UnkEntry_ld4db00* Data() { return reinterpret_cast<UnkEntry_ld4db00*>(storage); }
    UnkEntry_ld4db00* End() { return Data() + count; }

    void Resize(u32 n) {
        if (n < count) {
            for (u32 i = n; i < count; ++i) {
                Data()[i].~UnkEntry_ld4db00();
            }
        } else {
            for (u32 i = count; i < n; ++i) {
                new (&Data()[i]) UnkEntry_ld4db00();
            }
        }
        count = n;
    }

    UnkEntry_ld4db00* Erase(UnkEntry_ld4db00* pos) {
        for (UnkEntry_ld4db00* src = pos + 1; src != End(); ++src) {
            UnkEntry_ld4db00* dst = src - 1;
            *dst = *src;
        }
        Resize(count - 1);
        return pos;
    }
};

UnkEntry_ld4db00* fn_7100d4db00(UnkEntryVector_ld4db00* self, UnkEntry_ld4db00* pos) {
    return self->Erase(pos);
}
