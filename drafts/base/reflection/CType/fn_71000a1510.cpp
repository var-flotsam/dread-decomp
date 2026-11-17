// base::reflection::CType's unit (81): its static initialiser. Builds the unit's two string ids, registers
// the unit's type object, then builds and registers the "void" type. Names and layouts are placeholders.
#include <new>

#include "base/UnkAtExit.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/CType.h"
#include "base/reflection/UnkClassGetters.h"

using base::global::CStrId;
using base::reflection::CType;

// The unit's data (its statics, merged into one block).
struct UnkCTypeUnitData {
    CStrId sources;
    CStrId binaries;
    u8 pad_0x10[0x18];
    u8 type_0x28[1];  // a type object registered below
};
extern UnkCTypeUnitData g_unk_7101d5b5c0 __attribute__((visibility("hidden")));
extern void* g_unk_7101a1b540[];  // CType's vtable

void fn_71000a1510() {
    UnkCTypeUnitData& unit = g_unk_7101d5b5c0;
    new (&unit.sources) CStrId("sources", true);
    fn_7100000250(fn_710000051c, &unit.sources, &g_unk_7101a19000);
    new (&unit.binaries) CStrId("binaries", true);
    fn_7100000250(fn_710000051c, &unit.binaries, &g_unk_7101a19000);
    fn_710009c7b8();
    // The registry's list argument is not read by fn_710009ec00 (the original, which shares its unit,
    // leaves the register as it is).
    fn_710009ec00(nullptr, unit.type_0x28);

    {
        CStrId name("void", true);
        CType& type = g_unk_7101d5af48;
        *reinterpret_cast<void***>(&type) = &g_unk_7101a1b540[2];
        new (&type.m_name) CStrId(name);
        type.m_kind = -1;
        type.m_size = 0;
        type.m_flags = 0;
        type.field_0x24 = -1;
        type.m_fn_0x28 = nullptr;
        type.m_fn_0x30 = nullptr;
        type.m_fn_0x38 = nullptr;
        type.m_fn_0x40 = nullptr;
        type.m_fn_0x48 = nullptr;
        type.m_fn_0x50 = nullptr;
        type.m_fn_0x58 = nullptr;
        type.m_fn_0x60 = nullptr;
        type.m_fn_0x68 = nullptr;
        type.m_fn_0x70 = nullptr;
        type.m_parent = nullptr;
        type.field_0x80 = nullptr;
        type.field_0x88 = nullptr;
        new (&type.m_array_0x90) base::global::CRntVector<void*>;
        new (&type.m_array_0xb8) base::reflection::UnkReflArray(10, 0x3e);
    }
    fn_7100000250(reinterpret_cast<void (*)(void*)>(fn_710009cc04), &g_unk_7101d5af48, &g_unk_7101a19000);
    fn_710009ec00(nullptr, &g_unk_7101d5af48);
}
