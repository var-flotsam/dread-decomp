// CActorComponent's unit: the object of the first ref-counted handle in the list whose object's
// id matches the key, or null. Names and layouts are placeholders.
#include <algorithm>
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkCompInfo {
    u8 pad_0x0[0x30];
    u64 id;
};

struct UnkCompObject {
    u8 pad_0x0[0x18];
    UnkCompInfo* info;
};

struct UnkCompHandle : UnkRefObject {
    UnkCompObject* object;
};

// A handle copied by value: holds a reference for its lifetime.
struct UnkCompRef {
    UnkCompHandle* p;
    UnkCompRef(UnkCompHandle* h) : p(h) {
        if (p)
            ++p->ref_count;
    }
    ~UnkCompRef() {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

struct UnkCompHandleList {
    UnkCompHandle** data;
    u32 count;
};

UnkCompObject* fn_710024e2e8(UnkCompHandleList* self, const u64* key) {
    UnkCompHandle** it = std::find_if(self->data, self->data + self->count, [key](UnkCompRef ref) {
        return ref.p->object->info->id == *key;
    });
    if (it == self->data + self->count || !*it)
        return nullptr;
    return (*it)->object;
}
