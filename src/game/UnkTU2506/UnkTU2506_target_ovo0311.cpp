#include "game_s2/UnkWeakHandle.h"
#include "types.h"

void fn_7100331c7c(void* self, const void* other);

struct UnkRec48_b0244 {
    u64 unk_0x0[9];
};

struct UnkTargetSrc_b0244 {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    UnkRec48_b0244 record;  // 0x10
    u32 field_0x58;
    u32 field_0x5c;
};

struct UnkTargetRec_b0244 {
    UnkRefObject* field_0x0;
    UnkRefObject* field_0x8;
    UnkRefObject* field_0x10;
    UnkRec48_b0244 record;  // 0x18
    u32 field_0x60;
    u32 field_0x64;
    f32 field_0x68;
};

void fn_7100d48f88(UnkTargetRec_b0244* self, UnkWeakTarget* owner, f32 value, const UnkTargetSrc_b0244* src) {
    UnkWeakHandle* handle = GetWeakHandle(owner);
    self->field_0x0 = nullptr;
    AssignRef(&self->field_0x0, static_cast<UnkRefObject*>(handle));
    self->field_0x8 = nullptr;
    AssignRef(&self->field_0x8, src->field_0x0);
    self->field_0x10 = nullptr;
    AssignRef(&self->field_0x10, src->field_0x8);
    fn_7100331c7c(&self->record, &src->record);
    self->field_0x60 = src->field_0x58;
    self->field_0x64 = src->field_0x5c;
    self->field_0x68 = value;

}
