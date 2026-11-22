// Copy constructor of an array (base::global::CRntVector's layout) of 0x38-byte elements through an
// out-of-line Reserve (fn_7100b5c28c) and push-back (fn_7100b593d0): takes the source's settings,
// reserves its capacity and appends each element.
#include "base/global/CRntVector.h"
#include "game/GUI/UnkMissionLogRecords.h"
#include "game/UnkElem78.h"
#include "types.h"
#include "game/UnkTU2640.h"

template <class Elem, void (*Reserve)(UnkElemArray<Elem>*, u32), void (*PushBack)(UnkElemArray<Elem>*, const Elem*)>
static inline void CopyConstruct(UnkElemArray<Elem>* self, const UnkElemArray<Elem>& other) {
    self->data = nullptr;
    self->count = 0;
    self->capacity = 0;
    self->step = other.step;
    self->field_0x14 = other.field_0x14;
    self->flags = other.flags;
    self->allocator = other.allocator;
    Reserve(self, other.capacity);
    for (const Elem *it = other.data, *end = it + other.count; it != end; ++it)
        PushBack(self, it);
}

void fn_7100b5bfdc(UnkElem38Array* self, const UnkElem38Array& other) {
    CopyConstruct<UnkElem38, fn_7100b5c28c, fn_7100b593d0>(self, other);
}

void fn_71010012d4(UnkElemArray<UnkElem78>* self, const UnkElemArray<UnkElem78>& other) {
    CopyConstruct<UnkElem78, fn_710100147c, fn_7101001694>(self, other);
}
