// Passes the weak handle of `target` (created on first use) to a handler as a reference held for
// the call.
#include "game_s2/UnkWeakHandle.h"

void fn_7100c77fec(void* self, UnkWeakHandle** ref);
void fn_7100c783f8(void* self, UnkWeakHandle** ref);

template <void (*Handle)(void*, UnkWeakHandle**)>
__attribute__((always_inline)) static inline void PassWeakHandle(void* self, UnkWeakTarget* target) {
    UnkWeakHandle* handle = GetWeakHandle(target);
    UnkWeakHandle* ref = nullptr;
    if (handle) {
        ++handle->ref_count;
        ref = handle;
    }
    Handle(self, &ref);
    if (handle) {
        if (--handle->ref_count == 0)
            handle->vf_0x10();
    }
}

// NON_MATCHING(minor): the final release is not a tail call (the held reference's address escapes
// into the handler, which stops it here).
void fn_7100c782d0(void* self, UnkWeakTarget* target) { PassWeakHandle<fn_7100c77fec>(self, target); }
// NON_MATCHING(minor): as fn_7100c782d0.
void fn_7100c787ec(void* self, UnkWeakTarget* target) { PassWeakHandle<fn_7100c783f8>(self, target); }

// Whether the list of weak handles (at 0x10, count at 0x18) has an entry for the same object as
// `target`'s handle, which is held for the search.
struct UnkHandleList {
    u8 unk_0x0[0x10];
    UnkWeakHandle** data;
    u32 count;
};

struct UnkHeldHandle {
    UnkWeakHandle* ptr;

    __attribute__((always_inline)) explicit UnkHeldHandle(UnkWeakHandle* p) : ptr(p) {
        if (p)
            ++p->ref_count;
    }
    __attribute__((always_inline)) ~UnkHeldHandle() {
        if (UnkWeakHandle* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    __attribute__((always_inline)) UnkWeakTarget* Get() const { return ptr ? ptr->target : nullptr; }
};

// NON_MATCHING(minor): the null-handle flag is tested with cbz instead of tbz.
bool fn_7100c77e78(UnkHandleList* self, UnkWeakTarget* target) {
    UnkHeldHandle handle(GetWeakHandle(target));
    UnkWeakHandle** it = self->data;
    for (u64 n = self->count; n != 0; --n, ++it) {
        UnkWeakHandle* e = *it;
        UnkWeakTarget* et = e ? e->target : nullptr;
        if (et == handle.Get())
            return true;
    }
    return false;
}
