// Constructor of a component in the CPickableComponent unit (no post-construction virtual call): the
// component base constructor fn_71010c4148, the vtable, two empty string ids at +0x40 and +0x48,
// -1.0 at +0x50, an empty file-path id at +0x58 (its constructor inlined and folded), -1 at +0x68 and cleared
// fields after it. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CComponent.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkStateObject;
extern void* g_unk_7101a891b0[];

struct UnkPickableComponent {
    void** vtable;
    u8 pad_0x8[0x38];
    CStrId field_0x40;
    CStrId field_0x48;
    f32 field_0x50;
    CFilePathStrId field_0x58;
    void* field_0x60;
    s64 field_0x68;
    void* field_0x70;
    void* field_0x78;
    void* field_0x80;
    void* field_0x88;
    void* field_0x90;
};

void fn_71009ccf7c(UnkPickableComponent* self) {
    fn_71010c4148(reinterpret_cast<UnkStateObject*>(self));
    self->vtable = g_unk_7101a891b0 + 2;
    new (&self->field_0x40) CStrId("", true);
    new (&self->field_0x48) CStrId("", true);
    self->field_0x50 = -1.0f;
    // The empty file-path id, with the constructor's string lookup folded away.
    self->field_0x58.data = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->field_0x58.data = &g_unk_7101d47200->empty;
    self->field_0x60 = nullptr;
    self->field_0x68 = -1;
    self->field_0x70 = nullptr;
    self->field_0x78 = nullptr;
    self->field_0x80 = nullptr;
    self->field_0x88 = nullptr;
    self->field_0x90 = nullptr;
}
