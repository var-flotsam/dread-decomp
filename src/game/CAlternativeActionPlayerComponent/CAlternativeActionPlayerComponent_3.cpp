// CAlternativeActionPlayerComponent: the constructor variant without the post-construction virtual
// call (fn_710071185c), otherwise as fn_7100712448 in CAlternativeActionPlayerComponent_2.cpp: the
// component base constructor fn_71010c4148, the vtable, 0.5 at +0x40, an empty array from +0x48, a
// listener at +0x70 with two callbacks bound to the component. The fields are addressed by offset.
// Names and layouts are placeholders.
#include "base/UnkVector.h"

struct UnkStateObject;
void fn_71010c4148(UnkStateObject* self);
extern void* g_unk_7101a6c0b8[];
extern void* g_unk_7101a22d48[];

struct UnkAlternativeActionPlayer {
    void** vtable;
};

void fn_71007124d8(UnkAlternativeActionPlayer* self);
void fn_7100712538(UnkAlternativeActionPlayer* self);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_710071185c(UnkAlternativeActionPlayer* self) {
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
    At<u64>(self, 0x88) = 0;
    At<u64>(self, 0x90) = 0;
    At<UnkAlternativeActionPlayer*>(self, 0x98) = self;
    At<void (*)(UnkAlternativeActionPlayer*)>(self, 0xa0) = fn_71007124d8;
    At<UnkAlternativeActionPlayer*>(self, 0x78) = self;
    At<void (*)(UnkAlternativeActionPlayer*)>(self, 0x80) = fn_7100712538;
}
