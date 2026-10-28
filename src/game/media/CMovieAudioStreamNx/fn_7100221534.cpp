// Set-up and shutdown of the "MovieAudioStreamNx" player. fn_7100221534 runs fn_7100225b84,
// initialises the lock at 0xcd8, creates the thread at 0xa28 (entry fn_710022162c), allocates the
// stream state at 0xc20 and opens it with the format of the description at 0x38 (fn_7100221698).
// fn_7100221938 finishes the stream (fn_71002210f0), frees its buffers under the lock, joins the
// thread, destroys the movie::Buffer arrays at 0xc70 and 0xc98, frees the stream state and
// finishes in fn_7100225d2c.
#include <new>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkMovieBuffer_7100221938 {
    u8 data[0x40];
};

void fn_7101239ab0(UnkMovieBuffer_7100221938* buffer);  // movie::Buffer::~Buffer

struct UnkBufferArray_7100221938 {
    UnkMovieBuffer_7100221938* data;
    u32 count;

    __attribute__((always_inline)) void Clear() {
        for (u64 i = 0; i < count; ++i)
            fn_7101239ab0(&data[i]);
        count = 0;
    }
};

struct UnkStreamTail_7100221534 {
    u64 field_0x0;
    bool field_0x8;
    bool field_0x9;
    bool field_0xa;

    __attribute__((always_inline)) void Reset() {
        field_0x0 = 0;
        field_0x8 = false;
        field_0x9 = false;
        field_0xa = false;
    }
};

struct UnkAudioStream_7100221938 {
    u32 field_0x0;
    s32 field_0x4;
    s32 field_0x8;
    u32 field_0xc;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u64 field_0x28;
    u64 field_0x30[0x12];
    u64 field_0xc0;
    u8 unk_0xc8[0x190];
    u64 field_0x258;
    u64 field_0x260;
    u64 field_0x268;
    u64 field_0x270;
    u64 field_0x278;
    u64 field_0x280;
    u64 field_0x288;
    u64 field_0x290;
    u64 field_0x298;
    u32 field_0x2a0;
    u32 field_0x2a4;
    UnkStreamTail_7100221534 field_0x2a8;
};
void fn_71002210f0(UnkAudioStream_7100221938* stream);

struct UnkDescription_7100221534 {
    u8 unk_0x0[0x8];
    u32 field_0x8;
    u8 unk_0xc[0x1c];
    u32 field_0x28;
    u32 field_0x2c;
};

struct UnkPlayer_7100221938 {
    u8 unk_0x0[0x38];
    UnkDescription_7100221534* field_0x38;
    u8 unk_0x40[0x9e8];
    u8 field_0xa28[0x1f8];
    UnkAudioStream_7100221938* field_0xc20;
    u8 unk_0xc28[0x28];
    u8* field_0xc50;
    u8 unk_0xc58[0x18];
    UnkBufferArray_7100221938 field_0xc70;
    u8 unk_0xc80[0x18];
    UnkBufferArray_7100221938 field_0xc98;
    u8 unk_0xca8[0x18];
    UnkMovieBuffer_7100221938* field_0xcc0;
    u8 unk_0xcc8[0x10];
    nn::os::MutexType field_0xcd8;
    u8 unk_0xcf8[0x20];
    void* field_0xd18;
};

void fn_710006686c(void* thread);
extern "C" void free(void* p);
u64 fn_7100225d2c(UnkPlayer_7100221938* self);

u64 fn_7100221938(UnkPlayer_7100221938* self) {
    fn_71002210f0(self->field_0xc20);
    fn_710006691c(&self->field_0xcd8);
    if (self->field_0xd18) {
        free(self->field_0xd18);
        self->field_0xd18 = nullptr;
    }
    if (self->field_0xc50) {
        delete[] self->field_0xc50;
        self->field_0xc50 = nullptr;
    }
    if (UnkMovieBuffer_7100221938* buffer = self->field_0xcc0) {
        fn_7101239ab0(buffer);
        operator delete(buffer);
        self->field_0xcc0 = nullptr;
    }
    fn_7100066920(&self->field_0xcd8);
    fn_710006686c(self->field_0xa28);
    fn_7100066918(&self->field_0xcd8);
    self->field_0xc70.Clear();
    self->field_0xc98.Clear();
    if (self->field_0xc20) {
        operator delete(self->field_0xc20);
        self->field_0xc20 = nullptr;
    }
    return fn_7100225d2c(self);
}

void fn_7100225b84();
void fn_7100066744(void* thread, const char* name, void (*entry)(void*), u32 a, u32 b, void* arg);
void fn_710022162c(void* arg);
u64 fn_7100221698(UnkPlayer_7100221938* self, u32 a, u32 b, bool c);

u64 fn_7100221534(UnkPlayer_7100221938* self) {
    fn_7100225b84();
    fn_71000668f8(&self->field_0xcd8);
    fn_7100066744(self->field_0xa28, "MovieAudioStreamNx Thread", fn_710022162c, 0, 0x10, self);
    UnkAudioStream_7100221938* stream = new UnkAudioStream_7100221938;
    stream->field_0x4 = -1;
    stream->field_0x8 = -1;
    stream->field_0x20 = 0x8000;
    stream->field_0x28 = 0;
    for (u64 i = 0; i < 0x12; ++i)
        stream->field_0x30[i] = 0;
    stream->field_0xc0 = 0;
    stream->field_0x258 = 0;
    stream->field_0x260 = 0;
    stream->field_0x268 = 0;
    stream->field_0x270 = 0;
    stream->field_0x278 = 0;
    stream->field_0x280 = 0;
    stream->field_0x288 = 0;
    stream->field_0x290 = 0;
    stream->field_0x298 = 0;
    stream->field_0x2a0 = 48000;
    stream->field_0x2a4 = 2;
    stream->field_0x2a8.Reset();
    self->field_0xc20 = stream;
    stream->field_0x0 = 0;
    stream->field_0xc = 2;
    stream->field_0x10 = 0;
    stream->field_0x18 = 0x10;
    UnkDescription_7100221534* d = self->field_0x38;
    return fn_7100221698(self, d->field_0x28, d->field_0x2c, d->field_0x8 == 4);
}
