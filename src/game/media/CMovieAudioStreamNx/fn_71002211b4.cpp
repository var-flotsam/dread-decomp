// media::CMovieAudioStreamNx (unit 326): the constructor. After the media::CMovieStreamNx part
// (fn_710022586c) it sets the stream's defaults and creates its two movie::Buffer arrays with room
// for 64 buffers each. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "game/media/CMovieAudioStreamNx.h"
#include "game/media/MovieBuffer.h"
#include "types.h"

struct UnkMovieStreamNx;  // src/game/media/CMovieStreamNx.cpp
struct UnkMovieSource;
void fn_710022586c(UnkMovieStreamNx* self, UnkMovieSource* source, void* extra);

struct UnkMovieAudioStreamNx_l2211b4 {
    void** vtable;
    u8 pad_0x8[0xc0c - 0x8];
    u8 field_0xc0c[0x1e];
    u8 pad_0xc2a[2];
    u32 field_0xc2c;
    u32 field_0xc30;
    s32 field_0xc34;
    s32 field_0xc38;
    u32 field_0xc3c;
    u64 field_0xc40;
    u64 field_0xc48;
    u8* source;      // 0xc50
    s32 size;        // 0xc58
    s32 chunks;      // 0xc5c
    s32 chunk_size;  // 0xc60
    u32 field_0xc64;
    bool field_0xc68;
    base::global::CRntVector<movie::Buffer> queued;   // 0xc70
    base::global::CRntVector<movie::Buffer> pending;  // 0xc98
    u64 field_0xcc0;
    u64 field_0xcc8;
    u64 field_0xcd0;
    u8 mutex[0x20];  // 0xcd8
    s64 field_0xcf8;
    s64 field_0xd00;
    u64 field_0xd08;
    u8 pad_0xd10;
    bool field_0xd11;
    u64 field_0xd18;
    u64 field_0xd20;
    u64 field_0xd28;
};
static_assert(__builtin_offsetof(UnkMovieAudioStreamNx_l2211b4, queued) == 0xc70, "layout");
static_assert(__builtin_offsetof(UnkMovieAudioStreamNx_l2211b4, field_0xd28) == 0xd28, "layout");

void fn_71002211b4(UnkMovieAudioStreamNx_l2211b4* self, UnkMovieSource* source, void* extra) {
    fn_710022586c(reinterpret_cast<UnkMovieStreamNx*>(self), source, extra);
    self->vtable = g_unk_7101a204f0 + 2;
    self->field_0xc2c = 0;
    self->field_0xc30 = 0;
    for (u32 i = 0; i < sizeof(self->field_0xc0c); ++i)
        self->field_0xc0c[i] = 0;
    self->field_0xc34 = -1;
    self->field_0xc3c = 2;
    self->field_0xc48 = 0x10;
    self->field_0xc40 = 0;
    self->source = nullptr;
    self->field_0xc38 = -1;
    self->chunks = 0x10;
    self->chunk_size = 0x2000;
    self->field_0xc64 = 0x1000;
    self->field_0xc68 = false;
    new (&self->queued) base::global::CRntVector<movie::Buffer>(0xf);
    self->queued.Reserve(64);
    new (&self->pending) base::global::CRntVector<movie::Buffer>(0xf);
    self->pending.Reserve(64);
    self->field_0xcc0 = 0;
    self->field_0xcf8 = -1;
    self->field_0xcc8 = 5;
    self->field_0xcd0 = 0x8000;
    self->field_0xd20 = 0x8000;
    self->field_0xd00 = -1;
    self->field_0xd08 = 0;
    self->field_0xd11 = false;
    self->field_0xd18 = 0;
    self->field_0xd28 = 0;
    self->size = self->chunk_size * self->chunks;
}
