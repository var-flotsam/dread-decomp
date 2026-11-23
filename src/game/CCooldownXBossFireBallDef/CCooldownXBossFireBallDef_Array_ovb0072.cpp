// CCooldownXBossFireBallDef's unit (tools/tu_map.py): the reflected-array helpers of its array of
// 0x18-byte polymorphic elements (base/global/CRntVector.h), as CCooldownXBossLavaDropsMovementDef's
// (../CCooldownXBossLavaDropsMovementDef/CCooldownXBossLavaDropsMovementDef_Array_ovb0072.cpp): the
// begin iterator, erase, grow-to-index and copy constructor. The element is the class
// (reflected/CCooldownXBossFireBallDef.h): its default constructor sets bEnabled, fPreparationTimeOffset
// and bBreakable to true, 0 and false.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkCollectionIter.h"
#include "base/global/CRntVector.h"
#include "reflected/CCooldownXBossFireBallDef.h"
#include "types.h"

// The begin iterator only needs the element's size; its value holder is reset without a destructor.
struct UnkFireBallDefWords_ovb0072 {
    u64 words[3];
};

using FireBallDefVector = base::global::CRntVector<CCooldownXBossFireBallDef>;

void fn_71004c0bac(const base::global::CRntVector<UnkFireBallDefWords_ovb0072>* v, base::UnkCollectionIter<UnkFireBallDefWords_ovb0072>* out) {
    base::refl_Begin(v, out);
}

// CRntVector::EraseAt, written as fn_71004bf588 (this unit): the moved-from
// element is destroyed through a reload of the array.
static inline void EraseAt(FireBallDefVector* self, u32 index) {
    if (index < self->count) {
        u64 i = index;
        self->data[i].~CCooldownXBossFireBallDef();
        for (; i < (u32)(self->count - 1); ++i) {
            CCooldownXBossFireBallDef* cur = &self->data[i];
            new (cur) CCooldownXBossFireBallDef(static_cast<CCooldownXBossFireBallDef&&>(cur[1]));
            (&self->data[i])[1].~CCooldownXBossFireBallDef();
        }
        self->count = self->count - 1;
    }
}

void fn_71004c0078(FireBallDefVector* self, const u32* it) {
    CCooldownXBossFireBallDef* base = reinterpret_cast<CCooldownXBossFireBallDef*>(self);
    u64 offset = (u64)*it * sizeof(CCooldownXBossFireBallDef);
    CCooldownXBossFireBallDef* p = reinterpret_cast<CCooldownXBossFireBallDef*>(reinterpret_cast<char*>(base) + offset);
    EraseAt(self, (u32)(p - base));
}

CCooldownXBossFireBallDef* fn_71004c095c(FireBallDefVector* self, u32 index) { return &self->GrowAt(index); }

__attribute__((flatten)) void fn_71004bd2f0(FireBallDefVector* self, const FireBallDefVector* other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other->step;
    self->field_0x14 = other->field_0x14;
    self->flags = other->flags;
    self->allocator = other->allocator;
    self->Reserve(other->capacity);
    for (const CCooldownXBossFireBallDef *p = other->data, *e = p + other->count; p != e; ++p)
        self->PushBackCopy(*p);
}
