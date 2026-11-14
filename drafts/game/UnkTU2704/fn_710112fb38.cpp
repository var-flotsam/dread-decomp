// CSpawnPointComponent's unit (2704): assigns a reflected value to a slot of a value array, growing
// the array to hold the slot and releasing the payload the slot owned. Names and layouts are placeholders.
#include <new>

#include "base/UnkHashStream.h"
#include "base/UnkVector.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflValue.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkBoundObj;
struct UnkAssignedRecord_a1ba4;

// A value array element: a fresh one is empty (kind 2, stored through field_0x0).
struct UnkValueSlot : UnkReflValue {
    UnkValueSlot() {
        field_0x0 = nullptr;
        type = nullptr;
        kind = 2;
        field_0x14 = 1;
    }
};

struct UnkValueVec_l12fb38 {
    UnkValueSlot* data;  // 0x0
    u32 count;           // 0x8
    u32 capacity;        // 0xc
    u32 step;            // 0x10
    u8 growable;         // 0x14
};

void fn_7101130010(UnkValueVec_l12fb38* self, u32 capacity);  // reallocates to `capacity` elements

void fn_710112fb38(UnkValueVec_l12fb38* self, const u32* index, const UnkAssignedRecord_a1ba4* value) {
    u32 i = *index;
    if (self->growable && self->count <= i) {
        u32 grown = self->capacity;
        if (grown <= i && self->step != 0)
            grown = (i / self->step + 1) * self->step;
        fn_7101130010(self, grown);
        u32 n = i + 1;
        fn_7101130010(self, n);
        for (u32 k = self->count; k < n; ++k)
            new (&self->data[k]) UnkValueSlot();
        self->count = n;
    }
    UnkValueSlot* slot = &self->data[i];
    if (slot->type) {
        bool released = fn_7100deabe4(g_unk_7101d47310);
        void* payload;
        if (!slot->type)
            payload = nullptr;
        else if (slot->field_0x14 == 1 || *reinterpret_cast<const u64*>(slot->type + 0x18) > 8)
            payload = slot->field_0x0;
        else
            payload = slot;
        if (released) {
            fn_7100232124(static_cast<UnkBoundObj*>(payload));
        } else if (payload) {
            fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
            fn_7100083c00(payload);
        }
    }
    fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(slot), value);
}
