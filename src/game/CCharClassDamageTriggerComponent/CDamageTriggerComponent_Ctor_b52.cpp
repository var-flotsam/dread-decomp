// CDamageTriggerComponent unit (tools/tu_map.py unit 1563): its constructor, on the component base
// (fn_71011acd10): the vtable, a flag at +0x58, an empty file-path id at +0x60, string ids at +0x70
// ("") and +0x88 ("Lava Pool"), a flag pair at +0x90, a copy of the global id g_unk_7101d470e0 at
// +0x98 and cleared words. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

// The base constructor, as src/game/CMagnetSurfaceHuskComponent/CMagnetSurfaceHuskComponent_Ctor_b51.cpp
// declares it.
struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30*);
extern char g_unk_7101a8f020[];

struct UnkDamageTriggerComponent_b52 {
    void* vtable;
    u8 pad_0x8[0x50];
    bool field_0x58;
    UnkStringData* path;  // 0x60, a CFilePathStrId
    u32 field_0x68;
    CStrId field_0x70;
    u64 field_0x78;
    u64 field_0x80;
    CStrId field_0x88;
    bool field_0x90;
    bool field_0x91;
    u32 field_0x94;
    CStrId field_0x98;
    u32 field_0xa0;
};
static_assert(__builtin_offsetof(UnkDamageTriggerComponent_b52, field_0xa0) == 0xa0, "layout");

void fn_7100a4b100(UnkDamageTriggerComponent_b52* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->field_0x58 = true;
    self->vtable = g_unk_7101a8f020 + 0x10;
    self->path = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->path = &g_unk_7101d47200->empty;
    self->field_0x68 = 0;
    new (&self->field_0x70) CStrId("", true);
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    new (&self->field_0x88) CStrId("Lava Pool", true);
    self->field_0x90 = true;
    self->field_0x91 = false;
    self->field_0x94 = 0;
    new (&self->field_0x98) CStrId(g_unk_7101d470e0);
    self->field_0xa0 = 0;
}
