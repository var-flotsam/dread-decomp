// Initialisation of the Yamplot X AI component: runs the base step, keeps the owner's object at
// +0x14728 when it is a Yamplot X AI component (null otherwise), registers two embedded entries in
// the list at 0x161c (each initialised from a fixed offset into that component) and installs a
// callback with its context. Returns bit 0 of the base step's result.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"
#include "game/UnkBehaviorTree.h"

using base::reflection::CClass;

typedef u64 u64_a4 __attribute__((aligned(4)));

namespace {
struct UnkYamplotObject {
    void** vtable;
    const CClass* vf_0x0() { return reinterpret_cast<const CClass* (*)(UnkYamplotObject*)>(vtable[0])(this); }
};
}  // namespace

struct UnkComponent;
void fn_71006eca24(void* entry, u64 source);
void fn_71006ed63c(void* entry, u64 source);
void fn_71006f3258();

__attribute__((always_inline)) static inline void Register(char* self, char* entry) {
    char* list = self + 0x161c;
    u32 index = *reinterpret_cast<u32*>(self + 0x1618);
    *reinterpret_cast<u64_a4*>(list + ((u64)index << 3)) = reinterpret_cast<u64>(entry);
    *reinterpret_cast<u32*>(self + 0x1618) = index + 1;
}

u32 fn_71006ef458(char* self, void* arg) {
    // `arg` (x1) reaches the base step untouched in the original (tools/forwarded_args.py).
    u32 result = fn_7100369490(reinterpret_cast<char*>(self), arg);
    UnkYamplotObject* object = *reinterpret_cast<UnkYamplotObject**>(self + 0x10);
    UnkYamplotObject* component = nullptr;
    if (object && fn_710009d50c(object->vf_0x0(), fn_71006ee6b0()))
        component = object;
    u64* slot = reinterpret_cast<u64*>(self + 0x14728);
    *slot = reinterpret_cast<u64>(component);
    Register(self, self + 0x146c8);
    fn_71006eca24(self + 0x146c8, *slot + 0x510);
    Register(self, self + 0x146f8);
    fn_71006ed63c(self + 0x146f8, *slot + 0x600);
    *reinterpret_cast<char**>(self + 0x15a0) = self;
    *reinterpret_cast<void (**)()>(self + 0x15a8) = fn_71006f3258;
    return result & 1;
}
