// Unit 1726 (no reliable name): creates a GUI::IDisplayObjectTrack-derived object from a reflected type (when its
// constructor exists) and appends the pointer to a vector (base::UnkVector's layout). Names are placeholders.
#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "types.h"

using base::reflection::CType;

void* fn_7100b88198(base::UnkVector<void*>* self, CType* type) {
    void* obj = nullptr;
    if (fn_710009d50c(type, fn_7100b0da54()) && type->m_fn_0x28) {
        obj = fn_7100083990()->vf_0x40(type->m_size, 8, 0xc);
        if (obj)
            reinterpret_cast<void (*)(void*)>(type->m_fn_0x28)(obj);
    }
    self->PushBack(obj);
    return obj;
}
