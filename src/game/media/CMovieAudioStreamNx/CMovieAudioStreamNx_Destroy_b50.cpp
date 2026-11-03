// media::CMovieAudioStreamNx (unit 326): the destructor. The stream keeps two arrays of movie::Buffer
// (at +0xc70 and +0xc98), each with the allocator that owns its storage; the destructor destroys the
// buffers, frees the storage and finishes as a media::CMovieStreamNx (fn_710022594c). Names and
// layouts are placeholders.
#include "types.h"

namespace movie {
class Buffer {
public:
    ~Buffer();

private:
    u8 storage[0x40];
};
}  // namespace movie

// The allocator behind a buffer array (placeholder slot names).
class UnkMovieAllocator_b50 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48(void* p);  // frees
};

namespace {

struct BufferArray_b50 {
    movie::Buffer* data;
    u32 count;
    u32 capacity;
    u8 pad_0x10[0x10];
    UnkMovieAllocator_b50* allocator;

    __attribute__((always_inline)) void Destroy() {
        if (data) {
            for (u64 i = 0; i < count; ++i)
                data[i].~Buffer();
            count = 0;
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
};

}  // namespace

struct UnkMovieStreamNx_b50;  // src/game/media/CMovieStreamNx/CMovieStreamNx_Destroy_b50.cpp
void fn_710022594c(UnkMovieStreamNx_b50* self);

struct UnkMovieAudioStreamNx_b50 {
    void** vtable;
    u8 pad_0x8[0xc68];
    BufferArray_b50 field_0xc70;
    BufferArray_b50 field_0xc98;
};
static_assert(__builtin_offsetof(UnkMovieAudioStreamNx_b50, field_0xc98) == 0xc98, "layout");

extern void* g_unk_7101a204f0[];  // the stream's vtable

void fn_7100221408(UnkMovieAudioStreamNx_b50* self) {
    self->vtable = g_unk_7101a204f0 + 2;
    self->field_0xc98.Destroy();
    self->field_0xc70.Destroy();
    fn_710022594c(reinterpret_cast<UnkMovieStreamNx_b50*>(self));
}
