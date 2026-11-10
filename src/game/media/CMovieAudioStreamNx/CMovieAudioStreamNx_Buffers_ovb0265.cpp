// media::CMovieAudioStreamNx (unit 326): re-queues the stream's input. After the base reset (fn_7100225ef8) and under
// the mutex at +0xcd8 both movie::Buffer arrays (+0xc70, +0xc98) are emptied, then the source (+0xc50) is cut into
// +0xc5c chunks of +0xc60 bytes, each appended to the first array. Names and layouts are placeholders.
#include <new>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVector.h"
#include "types.h"

namespace movie {
class Buffer {
public:
    Buffer(void* data, u64 size);
    Buffer(const Buffer& other);
    ~Buffer();

private:
    u8 storage[0x40];
};
}  // namespace movie

struct UnkMovieStreamNx_ovb0265;
void fn_7100225ef8(UnkMovieStreamNx_ovb0265* self);

struct UnkAudioStreamBuffers {
    void** vtable;
    u8 pad_0x8[0xc50 - 0x8];
    u8* source;  // 0xc50
    u8 pad_0xc58[4];
    s32 chunks;      // 0xc5c
    s32 chunk_size;  // 0xc60
    u8 pad_0xc64[0xc70 - 0xc64];
    base::UnkVector<movie::Buffer> queued;   // 0xc70
    base::UnkVector<movie::Buffer> pending;  // 0xc98
    u8 pad_0xcc0[0xcd8 - 0xcc0];
    nn::os::MutexType mutex;  // 0xcd8
};
static_assert(__builtin_offsetof(UnkAudioStreamBuffers, mutex) == 0xcd8, "UnkAudioStreamBuffers layout");

static inline void DestroyAll(base::UnkVector<movie::Buffer>& list) {
    for (u64 i = 0; i < list.count; ++i)
        list.data[i].~Buffer();
    list.count = 0;
}

void fn_7100221b3c(UnkAudioStreamBuffers* self) {
    fn_7100225ef8(reinterpret_cast<UnkMovieStreamNx_ovb0265*>(self));
    nn::os::MutexType* mutex = &self->mutex;
    fn_710006691c(mutex);
    DestroyAll(self->queued);
    DestroyAll(self->pending);
    s32 offset = 0;
    for (s32 i = 0; i < self->chunks; ++i) {
        movie::Buffer chunk(self->source + offset, self->chunk_size);
        offset += self->chunk_size;
        self->queued.EmplaceBack(static_cast<movie::Buffer&&>(chunk));
    }
    fn_7100066920(mutex);
}
