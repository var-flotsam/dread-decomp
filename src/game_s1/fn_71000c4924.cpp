// Copies of fn_7100e6db08 and fn_7100e4b4a8 (src/game_aca57c/fn_7100e4afd8.cpp) for another variant layout, naming
// their own callees and data (the comments are adapted from the original's): fn_71000c4924, fn_71000c4df4;
// fn_71000c4a10, fn_71000c4ee0. Variants of two string ids (state 1; the second at `Second`) or a pair of words (state
// 2), with the state at `State`: assignment from another variant (taking its state and first 16 bytes) and from a pair.
// Reset releases the second id, then the first.
#include "base/UnkDestructors.h"
#include "types.h"

using base::UnkStrIdMember;


struct UnkWordPair {
    u64 a, b;
};

template <u32 Second, u32 State>
struct UnkStrIdPairVariant {
    UnkStrIdMember& First() { return *reinterpret_cast<UnkStrIdMember*>(this); }
    UnkStrIdMember& SecondId() { return *reinterpret_cast<UnkStrIdMember*>(reinterpret_cast<char*>(this) + Second); }
    u32& StateRef() { return *reinterpret_cast<u32*>(reinterpret_cast<char*>(this) + State); }
    UnkWordPair& Pair() { return *reinterpret_cast<UnkWordPair*>(this); }

    __attribute__((always_inline)) void Reset() {
        switch (StateRef()) {
        case 1:
            SecondId().~UnkStrIdMember();
            First().~UnkStrIdMember();
            StateRef() = 0;
            break;
        case 2:
            StateRef() = 0;
            break;
        }
    }

    __attribute__((always_inline)) UnkStrIdPairVariant& Assign(UnkStrIdPairVariant& other) {
        if (this != &other) {
            Reset();
            StateRef() = other.StateRef();
            Pair() = other.Pair();
        }
        return *this;
    }

    __attribute__((always_inline)) UnkStrIdPairVariant& Assign(const UnkWordPair& pair) {
        if (reinterpret_cast<const void*>(this) != &pair) {
            u64 a = pair.a;
            u64 b = pair.b;
            Reset();
            StateRef() = 2;
            Pair().a = a;
            Pair().b = b;
        }
        return *this;
    }
};

using UnkVariant = UnkStrIdPairVariant<0x8, 0x10>;

UnkVariant* fn_71000c4924(UnkVariant* self, const UnkWordPair& pair) { return &self->Assign(pair); }
UnkVariant* fn_71000c4df4(UnkVariant* self, const UnkWordPair& pair) { return &self->Assign(pair); }

UnkVariant* fn_71000c4a10(UnkVariant* self, UnkVariant* other) { return &self->Assign(*other); }
UnkVariant* fn_71000c4ee0(UnkVariant* self, UnkVariant* other) { return &self->Assign(*other); }
