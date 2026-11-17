// Unit 268: rebinds the owner's (handle, object) bindings to a new list of objects. A binding whose
// object is still in the list keeps its handle; the handle of every other binding is given back to
// the pool of handles in use. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "types.h"

struct UnkBoundObj_l1ea1e8;

struct UnkBinding_l1ea1e8 {
    void* handle;
    UnkBoundObj_l1ea1e8* obj;
};

struct UnkBinder_l1ea1e8 {
    u8 pad_0x0[0x38];
    void* handles[10];  // 0x38, the first `handle_count` are in use
    u32 handle_count;   // 0x88
    u8 pad_0x8c[0x4c];
    UnkBinding_l1ea1e8 bindings[10];  // 0xd8
    u32 binding_count;                // 0x178
};
static_assert(__builtin_offsetof(UnkBinder_l1ea1e8, binding_count) == 0x178, "layout");

// Moves `handle` out of the handles in use (to just past them).
static inline void ReleaseHandle(UnkBinder_l1ea1e8* self, void* handle) {
    for (u32 i = 0; i < self->handle_count; ++i) {
        if (self->handles[i] == handle) {
            --self->handle_count;
            self->handles[i] = self->handles[self->handle_count];
            self->handles[self->handle_count] = handle;
            return;
        }
    }
}

void fn_71001ea1e8(UnkBinder_l1ea1e8& self, const base::global::CRntVector<UnkBoundObj_l1ea1e8*>* objs) {
    bool matched[10] = {};
    UnkBinding_l1ea1e8 kept[10] = {};
    u32 kept_count = 0;
    for (u32 i = 0; i < objs->count; ++i) {
        UnkBoundObj_l1ea1e8* obj = objs->data[i];
        if (!obj)
            continue;
        for (u32 j = 0; j < self.binding_count; ++j) {
            if (matched[j])
                continue;
            if (self.bindings[j].obj == obj) {
                matched[j] = true;
                kept[kept_count].handle = self.bindings[j].handle;
                kept[kept_count].obj = obj;
                ++kept_count;
                break;
            }
        }
    }
    for (u32 j = 0; j < self.binding_count; ++j) {
        if (!matched[j])
            ReleaseHandle(&self, self.bindings[j].handle);
        self.bindings[j] = UnkBinding_l1ea1e8();
    }
    self.binding_count = kept_count;
    for (u32 i = 0; i < self.binding_count; ++i)
        self.bindings[i] = kept[i];
}
