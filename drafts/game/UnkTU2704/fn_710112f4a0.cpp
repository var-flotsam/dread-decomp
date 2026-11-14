// CSpawnPointComponent's unit (2704): assigns a variant holding an id and a reflected value (held inline,
// state 1, or referred to through pointers, state 2) from another one, releasing the old payload first.
// Names and layouts are placeholders.
#include "base/UnkHashStream.h"
#include "base/reflection.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU341.h"
#include "types.h"

using base::reflection::UnkReflValue;

struct UnkBoundObj;
struct UnkAssignedRecord_a1ba4;
void fn_7100083c00(void* p);

struct UnkIdValue_l12f4a0 {
    u8 storage[0x20];  // an id at 0x0 and a value at 0x8, or pointers to them
    u32 state;         // 1: inline, 2: by reference

    u32* Id() { return state == 2 ? *reinterpret_cast<u32**>(storage) : reinterpret_cast<u32*>(storage); }
    UnkReflValue* Value() {
        return state == 2 ? *reinterpret_cast<UnkReflValue**>(storage + 8)
                          : reinterpret_cast<UnkReflValue*>(storage + 8);
    }
};
static_assert(__builtin_offsetof(UnkIdValue_l12f4a0, state) == 0x20, "layout");

// Releases the payload the value owns.
static inline void DestroyPayload(UnkReflValue* dst) {
    if (dst->type) {
        bool released = fn_7100deabe4(g_unk_7101d47310);
        void* payload = nullptr;
        if (dst->type) {
            payload = dst;
            if (dst->field_0x14 == 1 || *reinterpret_cast<const u64*>(dst->type + 0x18) > 8)
                payload = dst->field_0x0;
        }
        if (released) {
            fn_7100232124(static_cast<UnkBoundObj*>(payload));
        } else if (payload) {
            fn_710007c0e4(static_cast<UnkHashStream_0x38*>(payload));
            fn_7100083c00(payload);
        }
    }
}

UnkIdValue_l12f4a0* fn_710112f4a0(UnkIdValue_l12f4a0* self, UnkIdValue_l12f4a0* other) {
    switch (other->state) {
    case 1: {
        *self->Id() = *reinterpret_cast<u32*>(other->storage);
        UnkReflValue* dst = self->Value();
        DestroyPayload(dst);
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(dst),
                      reinterpret_cast<const UnkAssignedRecord_a1ba4*>(other->storage + 8));
        break;
    }
    case 2: {
        *self->Id() = **reinterpret_cast<u32**>(other->storage);
        const UnkReflValue* src = *reinterpret_cast<UnkReflValue**>(other->storage + 8);
        UnkReflValue* dst = self->Value();
        DestroyPayload(dst);
        fn_71000a1ba4(reinterpret_cast<UnkAssignedRecord_a1ba4*>(dst), reinterpret_cast<const UnkAssignedRecord_a1ba4*>(src));
        break;
    }
    }
    return self;
}
