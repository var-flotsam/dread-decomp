// Unit 72 (base::reflection::CClass): a class descriptor's set-up after construction: the type's own
// step (fn_710009ce10), the parent's part (fn_7100096884, only with a parent), the attribute and
// member-field functions (CType+0x70, CClass+0xe0), the two arrays shrunk to fit, then every
// 0xb0-byte record of the array at +0xe8 has its function references resolved against the class.
// Names are placeholders.
#include "base/reflection/CClass.h"
#include "types.h"

using base::reflection::CClass;
using base::reflection::CType;

struct UnkKeyedB0Array_b51;   // src/base/reflection/CClass/UnkKeyedB0_ShrinkToFit_b51.cpp
struct UnkFunctionRefs_b52;   // src/base/reflection/UnkUnit85/UnkFunctionRefs_Resolve_b52.cpp
void fn_7100096b54(UnkKeyedB0Array_b51* self);
void fn_71000e7e54(UnkFunctionRefs_b52* self, CClass* cls);
void fn_710009ce10(CClass* self);
void fn_7100096884(CClass* self, const CType* parent);
void fn_7100096c80(base::reflection::UnkReflArray* array);

struct UnkRecordB0_A1b71 {
    u8 bytes[0xb0];
};

void fn_71000967ec(CClass* self) {
    fn_710009ce10(self);
    if (const CType* parent = self->m_parent)
        fn_7100096884(self, parent);
    if (self->m_fn_0x70)
        self->m_fn_0x70(self);
    if (self->m_fn_0xe0)
        self->m_fn_0xe0(self);
    fn_7100096b54(reinterpret_cast<UnkKeyedB0Array_b51*>(&self->m_array_0xe8));
    fn_7100096c80(&self->m_array_0x110);
    UnkRecordB0_A1b71* it = static_cast<UnkRecordB0_A1b71*>(self->m_array_0xe8.data);
    for (UnkRecordB0_A1b71* end = it + self->m_array_0xe8.count; it != end; ++it)
        fn_71000e7e54(reinterpret_cast<UnkFunctionRefs_b52*>(it), self);
}
