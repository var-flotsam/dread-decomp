// CCooldownXBossLavaDropsMovementDef's unit (tools/tu_map.py): the reflected-array helpers of
// base::global::CRntVector<CCooldownXBossLavaDropsMovementDef> (base/UnkVector.h): its begin
// iterator, erase, grow-to-index and copy constructor. The element is 0x18 bytes: a vtable
// (unk_7101a3e068) and three words, of which the default constructor sets the last two (3.5, 1.0).
// Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkCollectionIter.h"
#include "base/UnkVector.h"
#include "types.h"

extern char g_unk_7101a3e068[];



struct UnkLavaMoveDefData_ovb0072 {
    u32 field_0x0;
    f32 field_0x4;
    f32 field_0x8;
};

struct UnkLavaMoveDef_ovb0072;
struct UnkLavaMoveDefVtable_ovb0072 {
    void* vf_0x0;
    void (*dtor)(UnkLavaMoveDef_ovb0072* self);
};

struct UnkLavaMoveDef_ovb0072 {
    const UnkLavaMoveDefVtable_ovb0072* vtable;
    UnkLavaMoveDefData_ovb0072 data;  // 0x8

    // The vtable is stored untyped: the original reloads the array after it.
    UnkLavaMoveDef_ovb0072() {
        const char* vt = g_unk_7101a3e068 + 0x10;
        __builtin_memcpy(&vtable, &vt, sizeof(vt));
        data.field_0x4 = 3.5f;
        data.field_0x8 = 1.0f;
    }
    UnkLavaMoveDef_ovb0072(const UnkLavaMoveDef_ovb0072& other)
        : vtable(reinterpret_cast<const UnkLavaMoveDefVtable_ovb0072*>(g_unk_7101a3e068 + 0x10)), data(other.data) {}
    UnkLavaMoveDef_ovb0072(UnkLavaMoveDef_ovb0072&& other)
        : vtable(reinterpret_cast<const UnkLavaMoveDefVtable_ovb0072*>(g_unk_7101a3e068 + 0x10)), data(other.data) {}
    ~UnkLavaMoveDef_ovb0072() { vtable->dtor(this); }
};
static_assert(sizeof(UnkLavaMoveDef_ovb0072) == 0x18, "UnkLavaMoveDef_ovb0072 layout");

// The begin iterator only needs the element's size; its value holder is reset without a destructor.
struct UnkLavaMoveDefWords_ovb0072 {
    u64 words[3];
};



using UnkLavaMoveDefVector_ovb0072 = base::UnkVector<UnkLavaMoveDef_ovb0072>;

void fn_71004b2f98(const base::UnkVector<UnkLavaMoveDefWords_ovb0072>* v, base::UnkCollectionIter<UnkLavaMoveDefWords_ovb0072>* out) {
    base::refl_Begin(v, out);
}

// UnkVector::EraseAt, written as fn_71004bf588 (src/game/CCooldownXBossFireBallDef/): the moved-from
// element is destroyed through a reload of the array.
static inline void EraseAt(UnkLavaMoveDefVector_ovb0072* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~UnkLavaMoveDef_ovb0072();
        for (; i < (u32)(self->count - 1); ++i) {
            UnkLavaMoveDef_ovb0072* cur = &self->data[i];
            new (cur) UnkLavaMoveDef_ovb0072(static_cast<UnkLavaMoveDef_ovb0072&&>(cur[1]));
            (&self->data[i])[1].~UnkLavaMoveDef_ovb0072();
        }
        self->count = self->count - 1;
    }
}

void fn_71004b2468(UnkLavaMoveDefVector_ovb0072* self, const u32* it) {
    UnkLavaMoveDef_ovb0072* base = reinterpret_cast<UnkLavaMoveDef_ovb0072*>(self);
    u64 offset = (u64)*it * sizeof(UnkLavaMoveDef_ovb0072);
    UnkLavaMoveDef_ovb0072* p = reinterpret_cast<UnkLavaMoveDef_ovb0072*>(reinterpret_cast<char*>(base) + offset);
    EraseAt(self, (u32)(p - base));
}

UnkLavaMoveDef_ovb0072* fn_71004b2d4c(UnkLavaMoveDefVector_ovb0072* self, u32 index) { return &self->GrowAt(index); }

__attribute__((flatten)) void fn_71004b0b80(UnkLavaMoveDefVector_ovb0072* self, const UnkLavaMoveDefVector_ovb0072* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const UnkLavaMoveDef_ovb0072 *p = other->data, *e = p + other->count; p != e; ++p)
        self->PushBackCopy(*p);
}
