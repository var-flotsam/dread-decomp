// media::CMediaStream's unit (tools/tu_map.py unit 319): the copy and move helpers (CType+0x30 and
// +0x38) of a reflected class built on a shared object (base vtable unk_7101a203f8, an atomic count
// at +0x8 cleared with release order) whose own vtable is unk_7101a20380; both copy the fields from
// +0x10 on. Names and layouts are placeholders.
#include "game/media/CMediaStream.h"
#include "types.h"

struct UnkStreamBlock16_b52 {
    u64 words[2];
};

struct UnkStreamBlock32_b52 {
    u64 words[4];
};

struct UnkStreamDesc_b52 {
    char* vtable;
    u32 ref_count;
    u32 pad_0xc;
    UnkStreamBlock16_b52 field_0x10;
    UnkStreamBlock32_b52 field_0x20;
    u32 field_0x40;
    u64 field_0x48;
};

__attribute__((always_inline)) static inline void CopyStreamDesc_b52(void* dst, const void* src) {
    auto* self = static_cast<UnkStreamDesc_b52*>(dst);
    auto* other = static_cast<const UnkStreamDesc_b52*>(src);
    self->vtable = g_unk_7101a203f8 + 0x10;
    __atomic_store_n(&self->ref_count, 0, __ATOMIC_RELEASE);
    self->vtable = g_unk_7101a20380 + 0x10;
    self->field_0x10 = other->field_0x10;
    self->field_0x20 = other->field_0x20;
    self->field_0x40 = other->field_0x40;
    self->field_0x48 = other->field_0x48;
}

void fn_710021bb0c(void* dst, const void* src) { CopyStreamDesc_b52(dst, src); }
void fn_710021bb60(void* dst, const void* src) { CopyStreamDesc_b52(dst, src); }
