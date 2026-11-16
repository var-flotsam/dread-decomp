// Unit 2702 (CAIComponent's neighbourhood): after the base update (fn_710024facc), copies the
// object's current callback set and word into an element and appends it to the object's list
// unless an equal element is already there, then clears a flag pair. Names and layouts are
// placeholders.
#include "base/UnkErasedVector.h"
#include "base/UnkRefHandle.h"
#include "game/CActorComponent.h"
#include "game/UnkCallbackSetVector.h"
#include "game/UnkTU2702.h"
#include "types.h"

struct UnkAIObject_l0f9e2c {
    u8 pad_0x0[0x1350];
    UnkCallbackSet_7101a20950 current;  // 0x1350
    UnkCallbackSetVector list;          // 0x1390
    u32 field_0x13b8;
    u8 pad_0x13bc[0x14];
    u16 field_0x13d0;
};
static_assert(__builtin_offsetof(UnkAIObject_l0f9e2c, field_0x13d0) == 0x13d0, "layout");

// The set's copy assignment: shares the owner and copies the count and the callbacks.
inline void AssignSet(UnkCallbackSet_7101a20950& dst, const UnkCallbackSet_7101a20950& src) {
    fn_7100234f78(reinterpret_cast<base::UnkRefHandle*>(&dst.owner),
                  reinterpret_cast<const base::UnkRefHandle*>(&src.owner));
    dst.field_0x30 = src.field_0x30;
    fn_710023b29c(&dst.callbacks, &src.callbacks);
}

inline bool Contains(const UnkCallbackSetVector& list, const UnkCallbackSetElem& value) {
    for (u64 i = 0; i < list.count; ++i) {
        if (fn_71011136b8(reinterpret_cast<const UnkCompared_0x48*>(&list.data[i]),
                          reinterpret_cast<const UnkCompared_0x48*>(&value)))
            return true;
    }
    return false;
}

void fn_71010f9e2c(UnkAIObject_l0f9e2c* self) {
    fn_710024facc(reinterpret_cast<UnkSmBase_b60*>(self));
    UnkCallbackSetElem elem;
    elem.field_0x40 = self->field_0x13b8;
    AssignSet(elem.set, self->current);
    if (!Contains(self->list, elem))
        fn_7101114588(reinterpret_cast<UnkEntryVector*>(&self->list), reinterpret_cast<const UnkEntry*>(&elem));
    self->field_0x13d0 = 0;
}
