// media::CMovieStreamNx (unit 332): the destructor (fn_710022594c) and the deleting destructor
// (fn_7100225aac). The layout is the one of src/game/media/CMovieStreamNx.cpp: a movie::Decoder at
// +0x50 and two fixed rings of 64 entries (4-byte words at +0x2c0, 24-byte records at +0x408), each
// with a vtable of its own; a ring's destructor walks its entries and empties it. Names are placeholders.
#include "types.h"

namespace movie {
class Decoder {
public:
    ~Decoder();
};
}  // namespace movie

extern void* g_unk_7101a20650[];  // the stream's vtable
extern void* g_unk_7101a206d0[];  // the rings' vtables
extern void* g_unk_7101a20700[];

namespace {

template <class T>
struct Ring_b50 {
    void** vtable;
    T* read;
    T* write;
    u64 count;
    T entries[64];

    __attribute__((always_inline)) void Destroy(void** vt) {
        vtable = vt;
        for (T* p = read; p != write;) {
            p->~T();
            T* next = p + 1;
            p = next == entries + 64 ? entries : next;
        }
        read = entries;
        write = entries;
        count = 0;
    }
};

struct Record_b50 {
    u8 data[0x18];
};

}  // namespace

struct UnkMovieStreamNx_b50 {
    void** vtable;
    u8 pad_0x8[0x48];
    u8 field_0x50[0x270];  // the movie::Decoder (and other state up to the word ring)
    Ring_b50<u32> field_0x2c0;
    u8 pad_0x3e0[0x28];  // a mutex and a counter
    Ring_b50<Record_b50> field_0x408;
};
static_assert(__builtin_offsetof(UnkMovieStreamNx_b50, field_0x408) == 0x408, "layout");

void fn_710021b7ac(UnkMovieStreamNx_b50* self);  // the base destructor

static inline __attribute__((always_inline)) void Destroy_b50(UnkMovieStreamNx_b50* self) {
    self->vtable = g_unk_7101a20650 + 2;
    self->field_0x408.Destroy(g_unk_7101a20700 + 2);
    self->field_0x2c0.Destroy(g_unk_7101a206d0 + 2);
    reinterpret_cast<movie::Decoder*>(self->field_0x50)->~Decoder();
    fn_710021b7ac(self);
}

void fn_710022594c(UnkMovieStreamNx_b50* self) {
    Destroy_b50(self);
}

void fn_7100225aac(UnkMovieStreamNx_b50* self) {
    Destroy_b50(self);
    operator delete(self);
}
