// The media::CMovieStreamNx unit (tools/tu_map.py: 0x710022561c-0x71002264d0, whose reflected
// class is media::CMovieStreamNx): the Switch movie stream over the SDK's movie library. The
// stream holds a movie::Decoder at +0x50 and two ring queues (64 4-byte entries at +0x2c0 and 64
// 0x18-byte entries at +0x408, each after a mutex; src/game/media/CMovieStreamNx/fn_7100225ef8.cpp empties
// them). The names and layouts are placeholders.
// Holds fn_710022586c (the constructor).
#include <new>

#include "types.h"

// The SDK's movie library, as far as this unit calls it (the port declares the same names in
// port/runtime/include/movie.h).
namespace movie {
enum DecoderType : s32 {};
enum DecoderOutputFormat : s32 {};
enum DecoderMode : s32 {};
s32 GetThreadPriority();
class Decoder {
public:
    Decoder(DecoderType type, DecoderOutputFormat format, DecoderMode mode, s32 priority);
};
}  // namespace movie

// What the stream plays: +0x4 says whether it is a stream of its own, +0x8 its kind.
struct UnkMovieSource {
    u8 pad_0x0[0x4];
    u32 field_0x4;
    u32 field_0x8;
};

template <class T, u32 N>
struct UnkMovieRing {
    void** vtable;
    T* read;
    T* write;
    u64 count;
    T entries[N];
};

struct UnkMovieEntry_0x18 {
    u8 data[0x18];
};

struct UnkMovieStreamNx {
    void** vtable;
    u8 pad_0x8[0x30];
    UnkMovieSource* field_0x38;
    u8 pad_0x40[0x10];
    u8 field_0x50[0x98];  // the movie::Decoder
    bool field_0xe8;
    u8 pad_0xe9[0x87];
    bool field_0x170;
    u8 pad_0x171[0x87];
    bool field_0x1f8;
    u8 pad_0x1f9[0x87];
    bool field_0x280;
    u8 pad_0x281[0x3f];  // +0x2a0: a mutex
    UnkMovieRing<u32, 64> field_0x2c0;
    u8 pad_0x3e0[0x20];  // a mutex
    u64 field_0x400;
    UnkMovieRing<UnkMovieEntry_0x18, 64> field_0x408;
};
static_assert(__builtin_offsetof(UnkMovieStreamNx, field_0x408) == 0x408, "UnkMovieStreamNx placeholder layout");

extern void* g_unk_7101a20650[];  // the stream's vtable
extern void* g_unk_7101a206d0[];  // the rings' vtables
extern void* g_unk_7101a20700[];

void fn_710021b774(UnkMovieStreamNx* self);  // the base constructor
movie::DecoderType fn_7100225550(u32 kind);

void fn_710022586c(UnkMovieStreamNx* self) {
    fn_710021b774(self);
    self->vtable = g_unk_7101a20650 + 2;
    new (self->field_0x50) movie::Decoder(fn_7100225550(self->field_0x38->field_0x8),
                                          (movie::DecoderOutputFormat)(self->field_0x38->field_0x4 != 0 ? 1 : 3),
                                          (movie::DecoderMode)(self->field_0x38->field_0x4 != 0 ? 1 : 2),
                                          movie::GetThreadPriority());
    self->field_0xe8 = false;
    self->field_0x170 = false;
    self->field_0x1f8 = false;
    self->field_0x280 = false;
    self->field_0x2c0.vtable = g_unk_7101a206d0 + 2;
    self->field_0x2c0.read = self->field_0x2c0.entries;
    self->field_0x2c0.write = self->field_0x2c0.entries;
    self->field_0x2c0.count = 0;
    self->field_0x400 = 0;
    self->field_0x408.vtable = g_unk_7101a20700 + 2;
    self->field_0x408.read = self->field_0x408.entries;
    self->field_0x408.write = self->field_0x408.entries;
    self->field_0x408.count = 0;
}
