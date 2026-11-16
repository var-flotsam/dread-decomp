// Unit 1501 (CSamusMovement area): fn_71009ebbd8 erases one entry of a fixed-capacity array of 0x48-byte
// entries (two counted slots and a record): the later ones move down by one and the last is destroyed.
// Names and layouts are placeholders.
#include <new>

#include "game/UnkAssignedSlot.h"
#include "types.h"

// A counted object: its third virtual function destroys it once the count drops to zero.
struct UnkCounted_l9ebbd8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Destroy();
    s32 refs;  // 0x8
};

static inline void Release(void* ptr) {
    UnkCounted_l9ebbd8* obj = static_cast<UnkCounted_l9ebbd8*>(ptr);
    if (obj && --obj->refs == 0)
        obj->Destroy();
}

struct UnkIdTriple_l9ebbd8 {
    s32 id = -1;
    u32 a = 0;
    u32 b = 0;
};

struct UnkVec3_l9ebbd8 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
};

struct UnkVec4_l9ebbd8 {
    f32 x = 0.0f;
    f32 y = 0.0f;
    f32 z = 0.0f;
    f32 w = 0.0f;
};

#pragma pack(push, 4)
struct UnkEntry_l9ebbd8 {
    UnkAssignedSlot slot = {nullptr};  // 0x0
    UnkAssignedSlot slot_0x8 = {nullptr};  // 0x8
    UnkIdTriple_l9ebbd8 field_0x10;
    UnkVec3_l9ebbd8 field_0x1c;
    UnkVec4_l9ebbd8 field_0x28;
    f32 field_0x38 = 0.0f;
    f32 field_0x3c = 0.0f;
    u32 field_0x40 = 0;
    u32 unk_0x44;

    UnkEntry_l9ebbd8() {}
    ~UnkEntry_l9ebbd8() {
        Release(slot_0x8.ptr);
        Release(slot.ptr);
    }
    UnkEntry_l9ebbd8& operator=(const UnkEntry_l9ebbd8& other) {
        slot = other.slot;
        slot_0x8 = other.slot_0x8;
        field_0x10 = other.field_0x10;
        field_0x1c = other.field_0x1c;
        field_0x28 = other.field_0x28;
        field_0x38 = other.field_0x38;
        field_0x3c = other.field_0x3c;
        field_0x40 = other.field_0x40;
        return *this;
    }
};

struct UnkEntryArray_l9ebbd8 {
    u32 count;  // 0x0
    UnkEntry_l9ebbd8 entries[1];  // 0x4, capacity unknown

    UnkEntry_l9ebbd8* begin() { return entries; }
    UnkEntry_l9ebbd8* end() { return begin() + count; }

    void Resize(u32 n) {
        if (n < count) {
            for (u64 i = n; i < count; i++)
                entries[i].~UnkEntry_l9ebbd8();
        } else {
            for (u64 i = count; i < n; i++)
                new (&entries[i]) UnkEntry_l9ebbd8();
        }
        count = n;
    }
};
#pragma pack(pop)
static_assert(sizeof(UnkEntry_l9ebbd8) == 0x48, "UnkEntry_l9ebbd8 layout");

UnkEntry_l9ebbd8* fn_71009ebbd8(UnkEntryArray_l9ebbd8* self, UnkEntry_l9ebbd8* pos) {
    UnkEntry_l9ebbd8* entries = self->entries;
    for (UnkEntry_l9ebbd8* next = pos + 1; next != entries + self->count; ++next)
        next[-1] = *next;
    self->Resize(self->count - 1);
    return pos;
}
