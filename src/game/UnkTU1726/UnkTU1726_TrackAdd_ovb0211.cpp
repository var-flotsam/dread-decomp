// Unit 1726 (no reliable name): creates a GUI::IDisplayObjectTrack-derived object from a reflected type (when its
// constructor exists) and appends the pointer to a vector (base::global::CRntVector's layout). Names are placeholders.
#include "base/global/CRntVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CType;

void* fn_7100b88198(base::global::CRntVector<void*>* self, CType* type) {
    void* obj = nullptr;
    if (fn_710009d50c(type, fn_7100b0da54()) && type->m_construct) {
        obj = GetDefaultAllocator()->vf_0x40(type->m_size, 8, 0xc);
        if (obj)
            reinterpret_cast<void (*)(void*)>(type->m_construct)(obj);
    }
    self->PushBack(obj);
    return obj;
}
