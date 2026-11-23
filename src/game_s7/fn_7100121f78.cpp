// Copies of fn_7100c0b7a4, fn_7100b177a4 (src/game/SCameraRailNode/fn_7100c0b7a4.cpp), which the binary
// repeats at other addresses: fn_7100121f78, fn_71010607b4, fn_71000cf3b4.
// Copy-assignments of growable arrays (base::global::CRntVector's layout) of plain elements copied one
// by one: the array is emptied, reserves the other's capacity and appends each element,
// growing like PushBack (including its hang).
#include <new>
#include "base/UnkVtableList.h"

#include "base/global/CRntVector.h"
#include "game/CGameManager/CTunableGameManager.h"
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

UnkCopiedVector<UnkValuePair>* fn_7100121f78(UnkCopiedVector<UnkValuePair>* self, const UnkCopiedVector<UnkValuePair>* other) {
    return &(*self = *other);
}

UnkCopiedVector<UnkValuePair>* fn_71010607b4(UnkCopiedVector<UnkValuePair>* self, const UnkCopiedVector<UnkValuePair>* other) {
    return &(*self = *other);
}

// A plain word, and two words and a value, copied piece by piece.
struct UnkWord {
    u64 value;

    UnkWord(const UnkWord& other) { value = other.value; }
};

UnkCopiedVector<UnkWord>* fn_71000cf3b4(UnkCopiedVector<UnkWord>* self, const UnkCopiedVector<UnkWord>* other) {
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

// Copy-assignment of an array of polymorphic records (destroyed through vtable slot 1): the
// elements are destroyed, the array reserves the other's capacity (moving each record over and
// destroying the old one) and each of the other's records is appended by fn_7100e4ea08.

struct UnkPolyVector : UnkCopiedVector<UnkPolyValues> {};

// NON_MATCHING(minor): the shared end of the append (setting the tail and the count) is duplicated
// into the empty-list case; the original branches to it.
UnkWordList* fn_7100d833f8(UnkWordList* self, const UnkWordList* other);
// NON_MATCHING(minor): as fn_7100d833f8.
