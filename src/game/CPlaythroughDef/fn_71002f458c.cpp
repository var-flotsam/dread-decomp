// Constructor of an object that holds two copies of the default string id (+0x58, +0x60) and a
// growable array (+0x68, flags 0xf). The base's constructor (fn_7100016ad8) runs first; the
// object's virtual function in slot 3 is called at the end.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"

using base::global::CStrId;

extern void* g_unk_7101a22240[];  // the object's vtable

void fn_7100016ad8(void* self);

struct UnkIdArrayHolder {
    void** vtable;
    u8 pad_0x8[0x50];
    CStrId field_0x58;
    CStrId field_0x60;
    base::UnkVector<void*> field_0x68;
};

void fn_71002f458c(UnkIdArrayHolder* self) {
    fn_7100016ad8(self);
    self->vtable = g_unk_7101a22240 + 2;
    new (&self->field_0x58) CStrId(g_unk_7101d470e0);
    new (&self->field_0x60) CStrId(g_unk_7101d470e0);
    new (&self->field_0x68) base::UnkVector<void*>(0xf);
    reinterpret_cast<void (*)(UnkIdArrayHolder*)>(self->vtable[3])(self);
}
