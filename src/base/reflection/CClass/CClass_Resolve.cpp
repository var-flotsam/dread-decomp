// From the base::reflection::CClass unit: a pass over one class descriptor once every type is
// registered. It runs the registry's hook at +0x178 (g_unk_7101d4c8d0) on the class when one is
// set, then hands each entry of the two arrays (0xb0-byte entries at +0xe8, the functions, 0xb8
// bytes each, at +0x110) with the class to fn_71000e7420 and fn_710009ad4c. Names are placeholders.
#include "base/reflection/CClass.h"
#include "game/UnkTU85.h"
#include "types.h"

using base::reflection::CClass;

struct UnkVersionRegistry {
    u8 unk_0x0[0x178];
    u64 field_0x178;
    u8 unk_0x180[0x28];
};
extern UnkVersionRegistry* g_unk_7101d4c8d0;

struct UnkReflClassEntry {
    u8 unk_0x0[0xb0];
};
struct UnkReflClassFunction {
    u8 unk_0x0[0xb8];
};

void fn_71000e7420(UnkReflClassEntry* self, CClass* owner);
void fn_710009ad4c(UnkReflClassFunction* self, CClass* owner);

void fn_7100096f30(CClass* self) {
    void (*hook)(CClass*) = reinterpret_cast<void (*)(CClass*)>(g_unk_7101d4c8d0->field_0x178);
    if (hook)
        hook(self);
    UnkReflClassEntry* entries = static_cast<UnkReflClassEntry*>(self->m_array_0xe8.data);
    for (UnkReflClassEntry* it = entries, *end = entries + self->m_array_0xe8.count; it != end; ++it)
        fn_71000e7420(it, self);
    UnkReflClassFunction* functions = static_cast<UnkReflClassFunction*>(self->m_array_0x110.data);
    for (UnkReflClassFunction* it = functions, *end = functions + self->m_array_0x110.count; it != end; ++it)
        fn_710009ad4c(it, self);
}

// Hands each entry of the array at +0xe8 to fn_71000e82a8 with a null second argument.
struct UnkElem_0xb0;  // the entry, as src/engine/render/anim/CSkeletalAnimTree/fn_710014be78.cpp names it
void fn_7100097c84(CClass* self) {
    UnkReflClassEntry* entries = static_cast<UnkReflClassEntry*>(self->m_array_0xe8.data);
    for (UnkReflClassEntry* it = entries, *end = entries + self->m_array_0xe8.count; it != end; ++it)
        fn_71000e82a8(reinterpret_cast<UnkElem_0xb0*>(it), nullptr);
}
