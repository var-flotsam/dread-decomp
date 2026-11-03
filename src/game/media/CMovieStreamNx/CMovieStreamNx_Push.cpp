// From the media::CMovieStreamNx unit: pushing a word onto the stream's locked ring of 64 4-byte
// entries (mutex at +0x2a0, ring at +0x2c8; see fn_7100225ef8.cpp), wrapping the write position.
// The entry is written as a word that may alias the ring's own fields (the original reloads the
// write position after it). Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

typedef u32 __attribute__((may_alias)) UnkMovieWord;

struct UnkMovieWordRing {
    UnkMovieWord* read;
    UnkMovieWord* write;
    u64 count;
    UnkMovieWord entries[64];
};

struct UnkMovieStreamPush {
    u8 pad_0x0[0x2a0];
    nn::os::MutexType mutex;  // 0x2a0
    u8 pad_0x2c0[8];
    UnkMovieWordRing ring;  // 0x2c8
};

void fn_7100225fd0(UnkMovieStreamPush* self, u32 value) {
    fn_710006691c(&self->mutex);
    UnkMovieWordRing& r = self->ring;
    *r.write = value;
    UnkMovieWord* next = r.write + 1;
    r.write = next == r.entries + 64 ? r.entries : next;
    ++r.count;
    fn_7100066920(&self->mutex);
}
