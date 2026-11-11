// Destructors of a record of a string id and an array of reference-counted pointers (fn_7100239e0c and
// its copies): the array's elements are released and the array freed, then the id. Names are placeholders.
#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "game/UnkTU341.h"
#include "game_s2/UnkTwoStepStrId.h"
#include "types.h"

struct UnkRefObject_A1_b63 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    union {
        s32 ref_count;
    };
};

struct UnkRefPtr_A1_b63 {
    UnkRefObject_A1_b63* ptr;

    ~UnkRefPtr_A1_b63() {
        if (UnkRefObject_A1_b63* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
};

struct UnkIdRefVector_A1_b63 {
    UnkStrIdTwoStep id;
    base::UnkVector<UnkRefPtr_A1_b63> items;

    // (UnkVector::Free with a 64-bit index.)
    __attribute__((always_inline)) ~UnkIdRefVector_A1_b63() {
        if (items.data) {
            for (u64 i = 0; i < items.count; ++i)
                items.data[i].~UnkRefPtr_A1_b63();
            items.count = 0;
            items.allocator->vf_0x48(items.data);
            items.capacity = 0;
        }
    }
};

void fn_7100239e0c(UnkIdRefVector_A1_b63* self) { self->~UnkIdRefVector_A1_b63(); }
void fn_710023a2f8(UnkIdRefVector_A1_b63* self) { self->~UnkIdRefVector_A1_b63(); }
void fn_710024515c(UnkIdRefVector_A1_b63* self) { self->~UnkIdRefVector_A1_b63(); }
