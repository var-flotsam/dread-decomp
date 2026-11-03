// The copy assignments of the three callback-set array types whose copy constructors are in
// fn_710090dbb4.cpp (fn_710090ce1c and its copies): each truncates the array to nothing, reserves
// the source's capacity and pushes a copy of each source element, through that type's own
// out-of-line Truncate, Reserve and PushBack. fn_710090cf20 is in the CCharClassWaterPoolComponent
// unit, fn_7100a3b7f4 in CChangeStateDoorsLogicAction's, fn_7100a955cc in CThermalDeviceComponent's.
// Names are placeholders.
#include "game/UnkCallbackSetVector.h"

// Truncate (one copy per array type; fn_710090dbb4.cpp).
void fn_710090d2f4(UnkCallbackSetVector* v, u32 size);
void fn_7100a3bbc8(UnkCallbackSetVector* v, u32 size);
void fn_7100a959a0(UnkCallbackSetVector* v, u32 size);
// Reserve (one copy per array type)
void fn_710090d00c(UnkCallbackSetVector* v, u32 n);
void fn_7100a3b8e0(UnkCallbackSetVector* v, u32 n);
void fn_7100a956b8(UnkCallbackSetVector* v, u32 n);
// PushBack of a copy (one per array type)
void fn_710090d1bc(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);
void fn_7100a3ba90(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);
void fn_7100a95868(UnkCallbackSetVector* v, const UnkCallbackSetElem* value);

namespace {
template <void (*Truncate)(UnkCallbackSetVector*, u32), void (*Reserve)(UnkCallbackSetVector*, u32),
          void (*PushBack)(UnkCallbackSetVector*, const UnkCallbackSetElem*)>
__attribute__((always_inline)) inline UnkCallbackSetVector* Assign(UnkCallbackSetVector* self,
                                                                   const UnkCallbackSetVector* other) {
    if (self != other) {
        Truncate(self, 0);
        Reserve(self, other->capacity);
        for (const UnkCallbackSetElem *p = other->data, *end = p + other->count; p != end; ++p)
            PushBack(self, p);
    }
    return self;
}
}  // namespace

UnkCallbackSetVector* fn_710090cf20(UnkCallbackSetVector* self, const UnkCallbackSetVector* other) {
    return Assign<fn_710090d2f4, fn_710090d00c, fn_710090d1bc>(self, other);
}

UnkCallbackSetVector* fn_7100a3b7f4(UnkCallbackSetVector* self, const UnkCallbackSetVector* other) {
    return Assign<fn_7100a3bbc8, fn_7100a3b8e0, fn_7100a3ba90>(self, other);
}

UnkCallbackSetVector* fn_7100a955cc(UnkCallbackSetVector* self, const UnkCallbackSetVector* other) {
    return Assign<fn_7100a959a0, fn_7100a956b8, fn_7100a95868>(self, other);
}
