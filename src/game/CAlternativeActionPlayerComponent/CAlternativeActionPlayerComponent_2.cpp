// CAlternativeActionPlayerComponent: its constructor (fn_7100712448). The component base constructor
// fn_71010c4148, the vtable, 0.5 at +0x40, an empty array from +0x48 (step 10, flags 5, allocator),
// a listener at +0x70 (vtable g_unk_7101a22d48) with two callbacks bound to the component (+0x78:
// fn_7100712538, +0x98: fn_71007124d8), then the post-construction virtual call (slot 3). The fields
// are addressed by offset. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CAbilityGhostAura.h"
#include "game/CAlternativeActionPlayerComponent.h"
#include "game/CComponent.h"

struct UnkStateObject;

struct UnkAlternativeActionPlayer {
    void** vtable;
};

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_7100712448(UnkAlternativeActionPlayer* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a6c0b8 + 2;
    At<f32>(self, 0x40) = 0.5f;
    At<u64>(self, 0x48) = 0;
    At<u64>(self, 0x50) = 0;
    At<u64>(self, 0x58) = 0;
    At<u32>(self, 0x60) = 10;
    At<u32>(self, 0x64) = 5;
    At<base::UnkAllocator*>(self, 0x68) = fn_7100083990();
    At<void**>(self, 0x70) = g_unk_7101a22d48 + 2;
    At<UnkAlternativeActionPlayer*>(self, 0x98) = self;
    At<void (*)(UnkAlternativeActionPlayer*)>(self, 0xa0) = fn_71007124d8;
    At<u64>(self, 0x88) = 0;
    At<u64>(self, 0x90) = 0;
    At<UnkAlternativeActionPlayer*>(self, 0x78) = self;
    At<void (*)(UnkAlternativeActionPlayer*)>(self, 0x80) = reinterpret_cast<void (*)(UnkAlternativeActionPlayer *)>(&fn_7100712538);
    reinterpret_cast<void (*)(UnkAlternativeActionPlayer*)>(self->vtable[3])(self);
}
