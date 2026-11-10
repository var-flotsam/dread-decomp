// media::CMediaStream's unit (tools/tu_map.py unit 319): the constructor (called by
// media::CMovieStreamNx's, src/game/media/CMovieStreamNx.cpp): the shared object's base (vtable
// unk_7101a203f8, an atomic count at +0x8 cleared with release order), then the stream's vtable
// (unk_7101a20380), its source (+0x38), the extra argument (+0x30) and cleared fields. Names and
// layouts are placeholders.
#include "types.h"

extern char g_unk_7101a203f8[];
extern char g_unk_7101a20380[];

struct UnkMovieSource;

struct UnkMovieStreamNx {
    char* vtable;
    u32 ref_count;
    u8 pad_0xc[0x24];
    void* field_0x30;
    UnkMovieSource* field_0x38;
    u32 field_0x40;
    u64 field_0x48;
};

void fn_710021b774(UnkMovieStreamNx* self, UnkMovieSource* source, void* extra) {
    self->vtable = g_unk_7101a203f8 + 0x10;
    __atomic_store_n(&self->ref_count, 0, __ATOMIC_RELEASE);
    self->vtable = g_unk_7101a20380 + 0x10;
    self->field_0x30 = extra;
    self->field_0x38 = source;
    self->field_0x40 = 0;
    self->field_0x48 = 0;
}
