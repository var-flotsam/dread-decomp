// media unit 324 (a movie stream like media::CMovieStreamNx): the same fixed rings of 64 words
// (a vtable, the read and write positions, the count and the entries) as that class; taking a word
// from the front (fn_71002202e0) or the back (fn_710022034c). Names and layouts are placeholders.
#include "types.h"

struct UnkMovieWordRing_71002202e0 {
    void* vtable;
    u32* read;
    u32* write;
    u64 count;
    u32 entries[64];
};

u32 fn_71002202e0(UnkMovieWordRing_71002202e0* self) {
    u32 value = *self->read;
    u32* next = self->read + 1;
    self->read = next == self->entries + 64 ? self->entries : next;
    --self->count;
    return value;
}

u32 fn_710022034c(UnkMovieWordRing_71002202e0* self) {
    u32* prev = self->write == self->entries ? self->entries + 63 : self->write - 1;
    u32 value = *prev;
    self->write = prev;
    --self->count;
    return value;
}
