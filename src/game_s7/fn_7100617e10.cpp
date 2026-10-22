// Copies of fn_7100f49614 (src/game_7100ece7fc/fn_7100f49614.cpp) for other types, with their own callees and data (the names, strings and comments below are the original's): fn_7100617e10, fn_7100618288.
// Resets a three-state value holder (state at +0x38): state 1 holds an id/value element at
// +0x18, which is destroyed (its base vtable is restored, then its string id released);
// state 2 holds nothing to destroy. Both go back to state 0; any other state is left alone.
// Six copies, one per element type.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

extern char g_unk_7101cab1f8[];  // the elements' base vtable

struct UnkIdValueBase_0x18 {
    char* vtable;
    UnkStringData* id;  // CStrId; released in two steps (decrement, then test)

    __attribute__((always_inline)) ~UnkIdValueBase_0x18() {
        vtable = g_unk_7101cab1f8 + 0x10;
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (id && id != &pool->empty) {
                --id->ref_count;
                if (id->ref_count == 0) {
                    fn_71000803a4(pool, id);
                    pool = g_unk_7101d47108;
                }
            }
            id = &pool->empty;
        }
    }
};

// The value's storage ends where the state begins (+0x38 or +0x40).
template <u32 StateOffset>
struct UnkValueHolder {
    u8 unk_0x0[0x18];
    UnkIdValueBase_0x18 value;
    u8 unk_0x28[StateOffset - 0x28];
    s32 state;
};

template <u32 StateOffset>
static inline void Reset(UnkValueHolder<StateOffset>* self) {
    if (self->state == 2) {
    } else if (self->state == 1) {
        self->value.~UnkIdValueBase_0x18();
    } else {
        return;
    }
    self->state = 0;
}

void fn_7100617e10(UnkValueHolder<0x38>* self) { Reset(self); }

void fn_7100618288(UnkValueHolder<0x38>* self) { Reset(self); }

