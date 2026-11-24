// Constructor of an object that holds two copies of the default string id (+0x58, +0x60) and a
// growable array (+0x68, flags 0xf). The base's constructor (fn_7100016ad8) runs first; the
// object's virtual function in slot 3 is called at the end.
#include <new>

#include "base/global/CRntVector.h"
#include "base/core/CAsset.h"
#include "base/global/CStrId.h"
#include "game/CPlaythroughDef.h"

using base::global::CStrId;

struct UnkIdArrayHolder {
    void** vtable;
    u8 pad_0x8[0x50];
    CStrId sName;
    CStrId sLevelID;
    base::global::CRntVector<void*> field_0x68;
};

void fn_71002f458c(UnkIdArrayHolder* self) {
    fn_7100016ad8(self);
    self->vtable = g_unk_7101a22240 + 2;
    new (&self->sName) CStrId(base::global::CStrId::Empty());
    new (&self->sLevelID) CStrId(base::global::CStrId::Empty());
    new (&self->field_0x68) base::global::CRntVector<void*>(0xf);
    reinterpret_cast<void (*)(UnkIdArrayHolder*)>(self->vtable[3])(self);
}
