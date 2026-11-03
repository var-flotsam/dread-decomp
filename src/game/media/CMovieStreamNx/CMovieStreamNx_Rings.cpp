// media::CMovieStreamNx (unit 332): its fixed rings of 64 entries (a vtable, the read and write
// positions, the count and the entries): taking an entry from the front or the back of the ring of
// 4-byte words (fn_7100226038, fn_7100226344) and from the back of the ring of 24-byte records
// (fn_71002263ec) and queueing a record under the lock (fn_7100226224). Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

template <class T>
struct UnkMovieRing {
    void* vtable;
    T* read;
    T* write;
    u64 count;
    T entries[64];

    T PopFront() {
        T value = *read;
        T* next = read + 1;
        read = next == entries + 64 ? entries : next;
        --count;
        return value;
    }

    T PopBack() {
        T* prev = write == entries ? entries + 63 : write - 1;
        T value = *prev;
        write = prev;
        --count;
        return value;
    }
};

struct UnkMovieRecord {
    u32 field_0x0;
    void* field_0x8;
    u32 field_0x10;
};

typedef UnkMovieRing<u32> UnkMovieWordRing_7100226038;
typedef UnkMovieRing<UnkMovieRecord> UnkMovieRecordRing_71002263b0;

u32 fn_7100226038(UnkMovieWordRing_7100226038* self) { return self->PopFront(); }
u32 fn_7100226344(UnkMovieWordRing_7100226038* self) { return self->PopBack(); }
UnkMovieRecord fn_71002263ec(UnkMovieRecordRing_71002263b0* self) { return self->PopBack(); }

struct UnkMovieStream_7100226224 {
    u8 pad_0x0[0x2a0];
    nn::os::MutexType field_0x2a0;  // guards the word ring
    UnkMovieWordRing_7100226038 field_0x2c0;
    nn::os::MutexType field_0x3e0;  // guards the record ring
    u64 field_0x400;                // records queued so far
    UnkMovieRecordRing_71002263b0 field_0x408;
};
static_assert(__builtin_offsetof(UnkMovieStream_7100226224, field_0x408) == 0x408, "layout");

void fn_7100226224(UnkMovieStream_7100226224* self, u32 a, void* b, u32 c) {
    fn_710006691c(&self->field_0x3e0);
    UnkMovieRecordRing_71002263b0& ring = self->field_0x408;
    UnkMovieRecord record = {a, b, c};
    *ring.write = record;
    ++ring.count;
    UnkMovieRecord* next = ring.write + 1;
    ring.write = next == ring.entries + 64 ? ring.entries : next;
    ++self->field_0x400;
    fn_7100066920(&self->field_0x3e0);
}
