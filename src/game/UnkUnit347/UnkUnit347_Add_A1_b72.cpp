// Unit 347 of tools/tu_map.py (no class hint): creates an object for a string id with fn_71002548c0 and
// files it under the id in the owner's chained hash map at +0x10 (bucket = the string's 64-bit hash
// at +0x30, halves xored, modulo the bucket capacity; the node goes at the head of its bucket chain
// and the tail of the list of all nodes, as in src/base/UnkUnit204/UnkStrIdMapInsert_A1_b71.cpp).
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkUnit347Owner_A1b72 {
    u8 pad_0x0[0x10];
    base::global::CRntDictionary<CStrId, void*> map;  // 0x10
};

void* fn_71002548c0(const CStrId* key, void* arg, UnkUnit347Owner_A1b72* owner);

// The signature src/game/UnkUnit347/UnkUnit347_ReflCall_b41.cpp declares.
void* fn_71002634f8(char* self, void* a, void* b) {
    auto* owner = reinterpret_cast<UnkUnit347Owner_A1b72*>(self);
    const CStrId* key = static_cast<const CStrId*>(a);
    void* object = fn_71002548c0(key, b, owner);
    if (object)
        owner->map.Insert(*key, object);
    return object;
}
