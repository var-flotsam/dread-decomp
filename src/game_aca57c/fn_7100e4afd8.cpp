// Variants of two string ids (state 1; the second at `Second`) or a pair of words (state 2),
// with the state at `State`: assignment from another variant (taking its state and first 16
// bytes) and from a pair. Reset releases the second id, then the first.
#include "base/UnkDestructors.h"
#include "base/UnkKeyedStrIds.h"
#include "base/UnkVector.h"
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

using UnkVariantA = UnkStrIdPairVariant<0x20, 0x30>;
using UnkVariantB = UnkStrIdPairVariant<0x8, 0x20>;

UnkVariantA* fn_7100e4afd8(UnkVariantA* self, UnkVariantA* other) { return &self->Assign(*other); }
UnkVariantA* fn_7100e4b4a8(UnkVariantA* self, UnkVariantA* other) { return &self->Assign(*other); }
UnkVariantB* fn_7100e6d724(UnkVariantB* self, UnkVariantB* other) { return &self->Assign(*other); }
UnkVariantB* fn_7100e6dbf4(UnkVariantB* self, UnkVariantB* other) { return &self->Assign(*other); }

UnkVariantA* fn_7100e4aeec(UnkVariantA* self, const UnkWordPair& pair) { return &self->Assign(pair); }
UnkVariantA* fn_7100e4b3bc(UnkVariantA* self, const UnkWordPair& pair) { return &self->Assign(pair); }
UnkVariantB* fn_7100e6d638(UnkVariantB* self, const UnkWordPair& pair) { return &self->Assign(pair); }
UnkVariantB* fn_7100e6db08(UnkVariantB* self, const UnkWordPair& pair) { return &self->Assign(pair); }

// Frees parallel arrays of keys (at 0) and string ids (at 8) sharing a count and an allocator;
// the ids are released first (fn_7100ae2df8 and its copies).
__attribute__((always_inline)) static inline void Free(UnkKeyedStrIds* self) {
    if (self->ids && self->count)
        for (u64 i = 0; i < self->count; ++i)
            self->ids[i].~UnkStrIdMember();
    self->count = 0;
    if (self->ids)
        self->allocator->vf_0x48(self->ids);
    if (self->keys)
        self->allocator->vf_0x48(self->keys);
}

void fn_7100ae2df8(UnkKeyedStrIds* self) { Free(self); }
void fn_7100b39830(UnkKeyedStrIds* self) { Free(self); }
void fn_7100d86acc(UnkKeyedStrIds* self) { Free(self); }
void fn_7100e7241c(UnkKeyedStrIds* self) { Free(self); }
