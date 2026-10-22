// Copies of fn_7100d89148 (src/game_aca57c/fn_7100ca9a30.cpp), which the binary repeats at
// other addresses: fn_71002444f4, fn_71002478e4, fn_710075f714.
// Move constructors of a variant of a string id and a byte vector (state 1) or a pair of
// pointers to them (state 2): either way the result holds the id and the vector (state 1),
// moved from the other variant's own or from the referenced ones (fn_7100ca9a30 and its
// copies). The vector is moved field by field, leaving the source's data and sizes empty. The
// referenced vector is the engine's (another type with the same layout: the original keeps the
// two moves' loads apart).
#include <new>

#include "base/UnkDestructors.h"
#include "base/UnkVector.h"
#include "types.h"

using base::UnkAllocator;
using base::UnkStrIdMember;

struct UnkMovedVec {
    u8* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    UnkAllocator* allocator;

    UnkMovedVec(UnkMovedVec&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
    // From the engine's vector (the same layout).
    UnkMovedVec(base::UnkVector<u8>&& other)
        : data(other.data), count(other.count), capacity(other.capacity), step(other.step),
          field_0x14(other.field_0x14), flags(other.flags), allocator(other.allocator) {
        other.data = nullptr;
        other.count = 0;
        other.capacity = 0;
    }
};

struct UnkStrIdVecMoveVariant {
    struct Named {
        UnkStrIdMember id;
        UnkMovedVec vec;
    };
    union {
        Named named;
        struct {
            UnkStrIdMember* id;
            base::UnkVector<u8>* vec;
        } refs;
    };
    u32 state;

    __attribute__((always_inline)) UnkStrIdVecMoveVariant(UnkStrIdVecMoveVariant&& other) {
        state = 0;
        switch (other.state) {
        case 1:
            state = 1;
            new (&named) Named(static_cast<Named&&>(other.named));
            break;
        case 2: {
            state = 1;
            UnkStrIdMember* id = other.refs.id;
            base::UnkVector<u8>* vec = other.refs.vec;
            new (&named.id) UnkStrIdMember(static_cast<UnkStrIdMember&&>(*id));
            new (&named.vec) UnkMovedVec(static_cast<base::UnkVector<u8>&&>(*vec));
            break;
        }
        }
    }
};

#define MOVE_CTOR(addr)                                                                   \
    void fn_##addr(UnkStrIdVecMoveVariant* self, UnkStrIdVecMoveVariant* other) {         \
        new (self) UnkStrIdVecMoveVariant(static_cast<UnkStrIdVecMoveVariant&&>(*other)); \
    }

MOVE_CTOR(71002444f4)
MOVE_CTOR(71002478e4)
MOVE_CTOR(710075f714)
