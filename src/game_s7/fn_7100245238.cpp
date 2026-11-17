// Copies of fn_7100d8a010 (src/game/logic/collision/CCollider/fn_7100d419c0.cpp), which the binary
// repeats at other addresses: fn_7100245238, fn_71002487ac, fn_7100a75724.
// Hash map node inserts: a node holding a string id and a vector, moved in, is allocated, pushed onto the front of the key's bucket chain
// and appended to the list of all nodes. The bucket comes from the key's two hash words modulo
// the bucket count.
#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A dictionary of vectors by string id; the inserts move the vector into the node.
using UnkVectorMap = base::global::CRntDictionary<CStrId, base::global::CRntVector<void*>>;

UnkVectorMap::TNode* fn_7100245238(UnkVectorMap* self, const CStrId& key, base::global::CRntVector<void*>& value) {
    return self->Insert(key, static_cast<base::global::CRntVector<void*>&&>(value));
}

UnkVectorMap::TNode* fn_71002487ac(UnkVectorMap* self, const CStrId& key, base::global::CRntVector<void*>& value) {
    return self->Insert(key, static_cast<base::global::CRntVector<void*>&&>(value));
}

UnkVectorMap::TNode* fn_7100a75724(UnkVectorMap* self, const CStrId& key, base::global::CRntVector<void*>& value) {
    return self->Insert(key, static_cast<base::global::CRntVector<void*>&&>(value));
}

