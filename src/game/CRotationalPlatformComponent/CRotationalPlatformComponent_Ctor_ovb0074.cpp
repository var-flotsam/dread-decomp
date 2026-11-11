// CRotationalPlatformComponent's constructor (fn_71009d957c). The component base constructor
// fn_71010c4148, the vtable and the secondary one at +0x40, a callback list at +0x48 (base vtable with
// an empty array of callables: step 10, flags 0xf; then the list's vtable and cleared fields), a
// cleared word and flag, the floats 0 and -90 at +0x8c, the member at +0x98 (fn_71009d92b4), a link at
// +0xd0 (0, the component and the callback fn_71009d9638), then the post-construction virtual call
// (slot 3). The fields are addressed by offset. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "game/CCentralUnitAIComponent.h"
#include "game/CComponent.h"
#include "game/UnkTU341.h"

struct UnkStateObject;
extern void* g_unk_7101a89c50[];  // the component's vtables

struct UnkRotationalPlatform_ovb0074 {
    void** vtable;
};

void fn_71009d92b4(void* self);
void fn_71009d9638(UnkRotationalPlatform_ovb0074* self);

template <class T>
static inline T& At(void* base, u64 offset) {
    return *reinterpret_cast<T*>(reinterpret_cast<char*>(base) + offset);
}

void fn_71009d957c(UnkRotationalPlatform_ovb0074* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a89c50 + 2;
    At<void**>(self, 0x40) = g_unk_7101a89c50 + 0x34;
    At<char*>(self, 0x48) = reinterpret_cast<char*>(g_unk_7101a20998) + 0x10;
    At<u64>(self, 0x50) = 0;
    At<u64>(self, 0x58) = 0;
    At<u32>(self, 0x60) = 10;
    At<u8>(self, 0x64) = 1;
    At<u32>(self, 0x68) = 0xf;
    At<base::UnkAllocator*>(self, 0x70) = fn_7100083990();
    At<u32>(self, 0x78) = 0;
    At<char*>(self, 0x48) = reinterpret_cast<char*>(g_unk_7101a33fa0) + 0x10;
    At<u64>(self, 0x80) = 0;
    At<u8>(self, 0x88) = 0;
    At<f32>(self, 0x8c) = 0.0f;
    At<f32>(self, 0x90) = -90.0f;
    fn_71009d92b4(&At<u8>(self, 0x98));
    At<u64>(self, 0xd0) = 0;
    At<UnkRotationalPlatform_ovb0074*>(self, 0xd8) = self;
    At<void (*)(UnkRotationalPlatform_ovb0074*)>(self, 0xe0) = fn_71009d9638;
    reinterpret_cast<void (*)(UnkRotationalPlatform_ovb0074*)>(self->vtable[3])(self);
}
