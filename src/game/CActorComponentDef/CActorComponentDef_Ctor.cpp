// From unit 343 (CActorComponentDef): a constructor over the base fn_7100023838 builds (vtable
// g_unk_7101a20ba8 + 0x10): two string ids copied from the empty id g_unk_7101d470e0, a cleared word
// at 0x18, the bytes 0x20 and 0x21 set and a cleared pointer at 0x28. Names are placeholders.
#include "base/core/CBaseObject.h"
#include "base/global/CStrId.h"
#include "game/CActorComponentDef.h"
#include "types.h"

using base::global::CStrId;

struct UnkComponentDefEntry {
    void* vtable;
    CStrId id_0x8;
    CStrId id_0x10;
    u64 field_0x18;
    bool flag_0x20;
    bool flag_0x21;
    void* field_0x28;
};

using base::global::UnkStringData;

// A string id copied from a known data pointer: both copies take the empty id's data, read once.
static inline void CopyId(CStrId* id, UnkStringData* data) {
    id->data = data;
    if (&g_unk_7101d47108->empty != data)
        ++data->ref_count;
}

void fn_71002536b0(UnkComponentDefEntry* self) {
    fn_7100023838(self);
    self->vtable = g_unk_7101a20ba8 + 0x10;
    UnkStringData* empty = g_unk_7101d470e0.data;
    CopyId(&self->id_0x8, empty);
    CopyId(&self->id_0x10, empty);
    self->field_0x18 = 0;
    self->flag_0x20 = true;
    self->flag_0x21 = true;
    self->field_0x28 = nullptr;
}
