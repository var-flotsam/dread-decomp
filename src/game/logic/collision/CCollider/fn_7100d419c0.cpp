// Hash map node inserts: a node holding a string id and a value (a reference-counted pointer,
// assigned, or a vector, moved in) is allocated, pushed onto the front of the key's bucket chain
// and appended to the list of all nodes. The bucket comes from the key's two hash words modulo
// the bucket count.
#include "base/global/CRntDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

struct UnkRefObject {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    // A union member, so the count's updates may alias the node's fields (the original reloads
    // the old value after the increment).
    union {
        s32 ref_count;
    };
};

struct UnkRefPtr {
    UnkRefObject* ptr = nullptr;

    UnkRefPtr() = default;
    // Copies by assignment into an empty pointer.
    UnkRefPtr(const UnkRefPtr& other) { *this = other; }

    UnkRefPtr& operator=(const UnkRefPtr& other) {
        UnkRefObject* p = other.ptr;
        if (p != ptr) {
            if (p)
                ++p->ref_count;
            if (UnkRefObject* old = ptr) {
                if (--old->ref_count == 0)
                    old->vf_0x10();
            }
            ptr = p;
        }
        return *this;
    }
};

// The dictionaries of reference-counted pointers and of vectors by string id; the inserts copy the
// pointer in and move the vector in.
using UnkRefPtrMap = base::global::CRntDictionary<CStrId, UnkRefPtr>;
using UnkVectorMap = base::global::CRntDictionary<CStrId, base::global::CRntVector<void*>>;

UnkRefPtrMap::TNode* fn_7100d419c0(UnkRefPtrMap* self, const CStrId& key, const UnkRefPtr& value) {
    return self->Insert(key, value);
}
UnkRefPtrMap::TNode* fn_7100d41b00(UnkRefPtrMap* self, const CStrId& key, const UnkRefPtr& value) {
    return self->Insert(key, value);
}

UnkVectorMap::TNode* fn_7100caae20(UnkVectorMap* self, const CStrId& key, base::global::CRntVector<void*>& value) {
    return self->Insert(key, static_cast<base::global::CRntVector<void*>&&>(value));
}
UnkVectorMap::TNode* fn_7100d8a010(UnkVectorMap* self, const CStrId& key, base::global::CRntVector<void*>& value) {
    return self->Insert(key, static_cast<base::global::CRntVector<void*>&&>(value));
}
