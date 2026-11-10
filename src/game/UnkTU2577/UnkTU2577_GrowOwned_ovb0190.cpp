// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100eef244: the element accessor of the owned-entry array at +0x888 (UnkVector::GrowAt, out of line): past the end it grows the array to the next multiple of the step and then to index + 1, moving the owned entries and filling the new ones with null. Names and layouts are placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include "base/UnkVector.h"
#include "types.h"

struct UnkNamedIds;
void fn_7100ef1e1c(UnkNamedIds* self);

struct UnkOwnedEntry_b190 {
    union {
        UnkNamedIds* ptr;
    };
    UnkOwnedEntry_b190() : ptr(nullptr) {}
    UnkOwnedEntry_b190(UnkOwnedEntry_b190&& o) {
        UnkNamedIds* moved = o.ptr;
        o.ptr = nullptr;
        ptr = moved;
    }
    ~UnkOwnedEntry_b190() {
        UnkNamedIds* old = ptr;
        ptr = nullptr;
        if (old) {
            fn_7100ef1e1c(old);
            operator delete(old);
        }
    }
};

UnkOwnedEntry_b190& fn_7100eef244(base::UnkVector<UnkOwnedEntry_b190>* v, u32 index) {
    return v->GrowAt(index);
}
