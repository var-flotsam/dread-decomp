// CCharClassSteamJetComponent's unit (tools/tu_map.py): a constructor: the object cleared, the base
// constructor fn_71011acd10, the vtable and seven empty file-path string ids, built with the flag at
// +0x58 raised. Names and layouts are placeholders.
#include <new>

#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::CFilePathStrId;

extern void* g_unk_7101a7cc60[];

struct UnkSteamJetDef_l89dbe0 {
    void** vtable;
    u8 pad_0x8[0x50];
    bool field_0x58;
    CFilePathStrId path_a;  // 0x60
    CFilePathStrId path_b;
    CFilePathStrId path_c;
    CFilePathStrId path_d;
    CFilePathStrId path_e;
    CFilePathStrId path_f;
    CFilePathStrId path_g;
};
static_assert(__builtin_offsetof(UnkSteamJetDef_l89dbe0, path_a) == 0x60, "layout");

void fn_710089dbe0(UnkSteamJetDef_l89dbe0* self) {
    __builtin_memset(self, 0, 0x59);
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->field_0x58 = true;
    self->vtable = g_unk_7101a7cc60 + 2;
    new (&self->path_a) CFilePathStrId();
    new (&self->path_b) CFilePathStrId();
    new (&self->path_c) CFilePathStrId();
    new (&self->path_d) CFilePathStrId();
    new (&self->path_e) CFilePathStrId();
    new (&self->path_f) CFilePathStrId();
    new (&self->path_g) CFilePathStrId();
    self->field_0x58 = false;
}
