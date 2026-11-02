// A constructor in the CCharClassFanComponent unit: the base constructor fn_71011acd10, the
// vtable, cleared members and two empty file-path string ids at +0x50 and +0x58 (the shape of
// fn_710037d138, on the file-path pool). Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;

struct UnkStruct_71008bca30;
void fn_71011acd10(UnkStruct_71008bca30* self);
extern void* g_unk_7101a7b560[];

struct UnkFanDef {
    void** vtable;
    u8 pad_0x8[0x50];
    bool field_0x58;
    u8 pad_0x59[0x7];
    u64 field_0x60;
    UnkStringData* field_0x68;
};

void fn_710087f474(UnkFanDef* self) {
    fn_71011acd10(reinterpret_cast<UnkStruct_71008bca30*>(self));
    self->field_0x58 = true;
    self->field_0x60 = 0;
    self->vtable = g_unk_7101a7b560 + 2;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->field_0x60 = (u64)&g_unk_7101d47200->empty;
    self->field_0x68 = nullptr;
    if (!g_unk_7101d47200)
        base::global::UnkEnsureFilePathPool();
    self->field_0x68 = &g_unk_7101d47200->empty;
}
