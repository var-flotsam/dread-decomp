// Free (slot 0x48) of a sub-allocator held at 8 in a larger object: runs the object's free hook
// (a callback at 0x60 called with the user pointer at 0x68 and the object), then frees through
// the sub-allocator (fn_7100081420). fn_71001012f0 is the same for an object that is itself the
// sub-allocator. Called only through a vtable.

#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkFreeHookHost_71001039b4 {
    u64 field_0x0;
    u8 sub[0x58];  // a base::UnkSubAllocator
    void (*on_free)(void* user, UnkFreeHookHost_71001039b4* self);
    void* user;
};

void fn_7100081420(base::UnkSubAllocator* self, void* ptr);

void fn_71001039b4(base::UnkSubAllocator* sub, void* ptr) {
    auto* self = reinterpret_cast<UnkFreeHookHost_71001039b4*>(reinterpret_cast<char*>(sub) - 8);
    if (self->on_free)
        self->on_free(self->user, self);
    fn_7100081420(reinterpret_cast<base::UnkSubAllocator*>(self->sub), ptr);
}
