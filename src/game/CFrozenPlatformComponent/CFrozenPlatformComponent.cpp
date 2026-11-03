// CFrozenPlatformComponent: its constructor (fn_71009d5ff8). The component base constructor
// fn_71010c4148, the vtable, a link at +0x40 (0, the component and the callback fn_71009d6098), a
// callback list at +0x58 (base vtable with an empty array of callables: step 10, flags 0xf; then the
// list's vtable and cleared fields), then the post-construction virtual call (slot 3). The fields are
// addressed by offset. Names and layouts are placeholders.
#include "base/UnkVector.h"

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
extern void* g_unk_7101a89908[];  // the component's vtable
extern void* g_unk_7101a20998[];   // the callback list base's vtable
extern void* g_unk_7101a33fa0[];   // the callback list's vtable

struct UnkFrozenPlatform {
    void** vtable;
};

void fn_71009d6098(UnkFrozenPlatform* self);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_71009d5ff8(UnkFrozenPlatform* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a89908 + 2;
    At<u64>(self, 0x40) = 0;
    At<UnkFrozenPlatform*>(self, 0x48) = self;
    At<void (*)(UnkFrozenPlatform*)>(self, 0x50) = fn_71009d6098;
    At<char*>(self, 0x58) = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    At<u64>(self, 0x60) = 0;
    At<u64>(self, 0x68) = 0;
    At<u32>(self, 0x70) = 10;
    At<u8>(self, 0x74) = 1;
    At<u32>(self, 0x78) = 0xf;
    At<base::UnkAllocator*>(self, 0x80) = fn_7100083990();
    At<u32>(self, 0x88) = 0;
    At<char*>(self, 0x58) = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    At<u64>(self, 0x90) = 0;
    reinterpret_cast<void (*)(UnkFrozenPlatform*)>(self->vtable[3])(self);
}
