// A unit of tools/tu_map.py (2648, no reliable name): a chained hash map keyed by a string id (the
// bucket is the string's 64-bit hash at +0x30, halves xored, modulo the bucket capacity) whose
// 0x5f0-byte nodes hold the key, a polymorphic 0x5d0-byte value, the links of the list of all nodes
// and the link of the bucket chain (base::global::CRntDictionary). Its clear, and its inserts by copy
// and by move. Names and layouts are placeholders.
#include "base/global/CRntDictionary.h"
#include "base/global/CStrId.h"
#include "game/UnkTU2647.h"
#include "types.h"

using base::global::CStrId;

void fn_7101041d34(void* value, const void* other);  // the value's copy constructor
void fn_7101036fb8(void* value, void* other);        // the value's move constructor

struct UnkVirtualValue_b0248 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
};

// The 0x5d0-byte value: an object with a virtual table, copied, moved and destroyed out of line.
struct UnkValue_b0248 {
    u8 bytes[0x5d0];

    UnkValue_b0248(const UnkValue_b0248& other) { fn_7101041d34(bytes, other.bytes); }
    UnkValue_b0248(UnkValue_b0248&& other) { fn_7101036fb8(bytes, other.bytes); }
    ~UnkValue_b0248() { fn_7101036e34(bytes); }

    UnkVirtualValue_b0248* Object() { return reinterpret_cast<UnkVirtualValue_b0248*>(bytes); }
};

using UnkMap_b0248 = base::global::CRntDictionary<CStrId, UnkValue_b0248>;

// Lets every value go (slot 0x28 of its object), then clears the map.
void fn_710103fa90(UnkMap_b0248* self) {
    for (UnkMap_b0248::TNode* n = self->first; n; n = n->next)
        n->value.Object()->vf_0x28();
    self->Clear();
}

// The collection's add by copy and add by move.
void fn_7101042390(UnkMap_b0248* self, const CStrId* key, const UnkValue_b0248* value) { self->Insert(*key, *value); }

void fn_710104249c(UnkMap_b0248* self, const CStrId* key, UnkValue_b0248* value) {
    self->Insert(*key, static_cast<UnkValue_b0248&&>(*value));
}
