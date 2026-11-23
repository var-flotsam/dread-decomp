// Copy-assignments of growable arrays (base::global::CRntVector's layout) of plain elements copied one
// by one: the array is emptied, reserves the other's capacity and appends each element,
// growing like PushBack (including its hang).
#include <new>
#include "base/UnkVtableList.h"

#include "base/global/CRntVector.h"
#include "game/CGoliathXBurstProjectionBombMovement.h"
#include "game/SLogicPathNode.h"
#include "game/UnkBlockVector.h"
#include "game/UnkCopiedVector.h"
#include "game/UnkPolyValues.h"
#include "game/UnkPushedVector.h"
#include "game/UnkTU2563.h"
#include "game/UnkValuePair.h"
#include "game/UnkWordList.h"
#include "game/UnkWordNode.h"
#include "game/UnkWordsValue.h"
#include "types.h"

UnkCopiedVector<UnkValuePair>* fn_7100c0b7a4(UnkCopiedVector<UnkValuePair>* self, const UnkCopiedVector<UnkValuePair>* other) {
    return &(*self = *other);
}

// A plain word, and two words and a value, copied piece by piece.
struct UnkWord {
    u64 value;

    UnkWord(const UnkWord& other) { value = other.value; }
};

UnkCopiedVector<UnkWord>* fn_7100b177a4(UnkCopiedVector<UnkWord>* self, const UnkCopiedVector<UnkWord>* other) {
    return &(*self = *other);
}
UnkCopiedVector<UnkWord>* fn_7100b18f80(UnkCopiedVector<UnkWord>* self, const UnkCopiedVector<UnkWord>* other) {
    return &(*self = *other);
}
UnkCopiedVector<UnkWordsValue>* fn_7100e05764(UnkCopiedVector<UnkWordsValue>* self,
                                              const UnkCopiedVector<UnkWordsValue>* other) {
    return &(*self = *other);
}

// Copy-assignment of a record of two arrays and four values: the first array (of three
// values per element) appends through its out-of-line PushBack; the second (of half-words)
// is copied as one block.
struct UnkValues3 {
    u32 values[3];

    UnkValues3(const UnkValues3& other) {
        values[0] = other.values[0];
        values[1] = other.values[1];
        values[2] = other.values[2];
    }
};

__attribute__((always_inline)) inline UnkPushedVector& UnkPushedVector::operator=(const UnkPushedVector& other) {
    if (this != &other) {
        count = 0;
        u32 reserve = other.capacity;
        if (capacity < reserve)
            Reallocate(reserve);
        for (const UnkValues3 *it = other.data, *end = it + other.count; it != end; ++it)
            fn_710058c768(this, it);
    }
    return *this;
}

struct UnkArraysRecord {
    UnkPushedVector first;
    UnkBlockVector second;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;
};

UnkArraysRecord* fn_7100e4f444(UnkArraysRecord* self, const UnkArraysRecord* other) {
    self->first = other->first;
    self->second = other->second;
    self->field_0x50 = other->field_0x50;
    self->field_0x54 = other->field_0x54;
    self->field_0x58 = other->field_0x58;
    self->field_0x5c = other->field_0x5c;
    return self;
}

// Copy-assignment of an array of polymorphic records (destroyed through vtable slot 1): the
// elements are destroyed, the array reserves the other's capacity (moving each record over and
// destroying the old one) and each of the other's records is appended by fn_7100e4ea08.

struct UnkPolyVector : UnkCopiedVector<UnkPolyValues> {};

UnkPolyVector* fn_7100e4e898(UnkPolyVector* self, const UnkPolyVector* other) {
    if (self != other) {
        for (u64 i = 0; i < self->count; ++i)
            self->data[i].Destroy();
        self->count = 0;
        u32 reserve = other->capacity;
        if (self->capacity < reserve) {
            UnkPolyValues* p = static_cast<UnkPolyValues*>(
                self->allocator->vf_0x40((u64)reserve * sizeof(UnkPolyValues), 8, self->flags));
            if (UnkPolyValues* src = self->data) {
                UnkPolyValues* dst = p;
                for (u32 i = 0; i < self->count; ++i, ++src, ++dst) {
                    dst->vtable = g_unk_7101cac8f0 + 0x10;
                    dst->field_0x8 = src->field_0x8;
                    dst->field_0xc = src->field_0xc;
                    dst->field_0x10 = src->field_0x10;
                    dst->field_0x14 = src->field_0x14;
                    dst->field_0x18 = src->field_0x18;
                    src->Destroy();
                }
                self->allocator->vf_0x48(self->data);
            }
            self->data = p;
            self->capacity = reserve;
        }
        for (const UnkPolyValues *it = other->data, *end = it + other->count; it != end; ++it)
            fn_7100e4ea08(self, it);
    }
    return self;
}

// NON_MATCHING(minor): the shared end of the append (setting the tail and the count) is duplicated
// into the empty-list case; the original branches to it.
UnkWordList* fn_7100d833f8(UnkWordList* self, const UnkWordList* other) { return &(*self = *other); }
// NON_MATCHING(minor): as fn_7100d833f8.
UnkWordList* fn_7100d834c8(UnkWordList* self, const UnkWordList* other) { return &(*self = *other); }
