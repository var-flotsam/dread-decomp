// CCooldownXBossFireBallDef's unit (tools/tu_map.py): the reflected-array helpers of its array of
// 0x18-byte polymorphic elements (base/UnkVector.h), as CCooldownXBossLavaDropsMovementDef's
// (../CCooldownXBossLavaDropsMovementDef/CCooldownXBossLavaDropsMovementDef_Array_ovb0072.cpp): the
// begin iterator, erase, grow-to-index and copy constructor. The element is a vtable (unk_7101a3e2b0),
// a flag, a word and a flag; the default constructor sets them to true, 0 and false. Names and layouts
// are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101a3e2b0[];



// A flag, a word and a flag, copied as one 9-byte block (packed: the copy skips the tail padding).
struct __attribute__((packed)) UnkFireBallDefData_ovb0072 {
    bool field_0x0;
    u8 pad_0x1[3];
    f32 field_0x4;
    bool field_0x8;
};

struct UnkFireBallDef_ovb0072;
struct UnkFireBallDefVtable_ovb0072 {
    void* vf_0x0;
    void (*dtor)(UnkFireBallDef_ovb0072* self);
};

struct UnkFireBallDef_ovb0072 {
    const UnkFireBallDefVtable_ovb0072* vtable;
    UnkFireBallDefData_ovb0072 data;  // 0x8

    // The vtable is stored untyped: the original reloads the array after it.
    UnkFireBallDef_ovb0072() {
        const char* vt = g_unk_7101a3e2b0 + 0x10;
        __builtin_memcpy(&vtable, &vt, sizeof(vt));
        data.field_0x0 = true;
        data.field_0x4 = 0.0f;
        data.field_0x8 = false;
    }
    UnkFireBallDef_ovb0072(const UnkFireBallDef_ovb0072& other)
        : vtable(reinterpret_cast<const UnkFireBallDefVtable_ovb0072*>(g_unk_7101a3e2b0 + 0x10)), data(other.data) {}
    UnkFireBallDef_ovb0072(UnkFireBallDef_ovb0072&& other)
        : vtable(reinterpret_cast<const UnkFireBallDefVtable_ovb0072*>(g_unk_7101a3e2b0 + 0x10)), data(other.data) {}
    ~UnkFireBallDef_ovb0072() { vtable->dtor(this); }
};
static_assert(sizeof(UnkFireBallDef_ovb0072) == 0x18, "UnkFireBallDef_ovb0072 layout");

// The begin iterator only needs the element's size; its value holder is reset without a destructor.
struct UnkFireBallDefWords_ovb0072 {
    u64 words[3];
};



using UnkFireBallDefVector_ovb0072 = base::UnkVector<UnkFireBallDef_ovb0072>;

void fn_71004c0bac(const base::UnkVector<UnkFireBallDefWords_ovb0072>* v, base::UnkCollectionIter<UnkFireBallDefWords_ovb0072>* out) {
    base::refl_Begin(v, out);
}

// UnkVector::EraseAt, written as fn_71004bf588 (this unit): the moved-from
// element is destroyed through a reload of the array.
static inline void EraseAt(UnkFireBallDefVector_ovb0072* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkFireBallDef_ovb0072();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkFireBallDef_ovb0072* cur = &self->data[i];
            new (cur) UnkFireBallDef_ovb0072(static_cast<UnkFireBallDef_ovb0072&&>(cur[1]));
            (&self->data[i])[1].~UnkFireBallDef_ovb0072();
        }
        self->count = self->count - 1;
    }
}

void fn_71004c0078(UnkFireBallDefVector_ovb0072* self, const u32* it) {
    UnkFireBallDef_ovb0072* base = reinterpret_cast<UnkFireBallDef_ovb0072*>(self);
    u64 offset = (u64)*it * sizeof(UnkFireBallDef_ovb0072);
    UnkFireBallDef_ovb0072* p = reinterpret_cast<UnkFireBallDef_ovb0072*>(reinterpret_cast<char*>(base) + offset);
    EraseAt(self, (u32)(p - base));
}

UnkFireBallDef_ovb0072* fn_71004c095c(UnkFireBallDefVector_ovb0072* self, u32 index) { return &self->GrowAt(index); }

__attribute__((flatten)) void fn_71004bd2f0(UnkFireBallDefVector_ovb0072* self, const UnkFireBallDefVector_ovb0072* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkFireBallDef_ovb0072 *p = other->data, *e = p + other->count; p != e; ++p)
        self->PushBackCopy(*p);
}
