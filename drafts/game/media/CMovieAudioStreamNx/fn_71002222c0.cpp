// media::CMovieAudioStreamNx (unit 326): under the mutex at +0xcd8, takes the decoder's oldest output buffer into the
// first queued movie::Buffer (whole, or filled up to the chunk size with the rest carried over) and moves a full
// buffer to the pending array. Names and layouts are placeholders.
#include "base/UnkDefaultAllocator.h"
#include "base/global/CRntVector.h"
#include "types.h"

namespace movie {
class Buffer {
public:
    Buffer(const Buffer& other);
    ~Buffer();
    u8* Base();
    s32 Size();
    u32 GetUint32Data();
    s64 GetInt64Data();
    u64 Capacity();
    s32 Offset();
    void SetRange(s32 offset, s32 size);
    void SetInt64Data(s64 value);
    void SetInt32Data(s32 value);
    void SetUint32Data(u32 value);

private:
    u8 storage[0x40];
};
class Decoder {
public:
    s32 GetOutputBuffer(s32 index, Buffer* out);
    s32 ReleaseOutputBufferIndex(s32 index);
};
}  // namespace movie

// One decoded output the decoder reported: its buffer index, time and flags (4: the end of the stream).
struct UnkMovieOutputEntry {
    s32 index;
    s64 time;
    u32 flags;
};

struct UnkMovieOutputRing {
    void* vtable;
    UnkMovieOutputEntry* read;
    UnkMovieOutputEntry* write;
    u64 count;
    UnkMovieOutputEntry entries[64];

    void Pop() {
        UnkMovieOutputEntry* next = read + 1;
        read = next == entries + 64 ? entries : next;
        --count;
    }
};

struct UnkMovieAudioStreamDrain {
    u8 pad_0x0[0x50];
    movie::Decoder decoder;  // 0x50
    u8 pad_0x51[0x408 - 0x51];
    UnkMovieOutputRing outputs;  // 0x408
    u8 pad_0xa28[0xc2c - 0xa28];
    s32 field_0xc2c;
    u8 pad_0xc30[0xc3c - 0xc30];
    s32 field_0xc3c;
    u8 pad_0xc40[0xc64 - 0xc40];
    s32 chunk_size;  // 0xc64
    bool chunked;    // 0xc68
    base::global::CRntVector<movie::Buffer> queued;   // 0xc70
    base::global::CRntVector<movie::Buffer> pending;  // 0xc98
    movie::Buffer* output;                            // 0xcc0
    u8 pad_0xcc8[0xcd8 - 0xcc8];
    nn::os::MutexType mutex;  // 0xcd8
    u8 pad_0xcf8[0xd28 - 0xcf8];
    u64 field_0xd28;

    // Appends the whole output buffer to `buffer`.
    void AppendOutput(movie::Buffer* buffer) {
        __builtin_memcpy(buffer->Base() + buffer->Offset(), output->Base() + output->Offset(), output->Size());
        buffer->SetRange(0, buffer->Offset() + output->Size());
    }

    void ClearOutput() {
        output->SetInt64Data(-1);
        output->SetInt32Data(-1);
        output->SetUint32Data(0);
        output->SetRange(0, 0);
    }
};
static_assert(__builtin_offsetof(UnkMovieAudioStreamDrain, outputs) == 0x408, "layout");
static_assert(__builtin_offsetof(UnkMovieAudioStreamDrain, queued) == 0xc70, "layout");
static_assert(__builtin_offsetof(UnkMovieAudioStreamDrain, mutex) == 0xcd8, "layout");
static_assert(__builtin_offsetof(UnkMovieAudioStreamDrain, field_0xd28) == 0xd28, "layout");

void fn_71002222c0(UnkMovieAudioStreamDrain* self) {
    fn_710006691c(&self->mutex);
    if (self->outputs.count != 0 && self->queued.count != 0) {
        movie::Buffer* buffer = &self->queued.data[0];
        UnkMovieOutputEntry* entry = self->outputs.read;

        // What the last output left over goes first.
        if (self->chunked) {
            if (self->output->Size() > 0 && self->output->Size() <= buffer->Capacity()) {
                self->AppendOutput(buffer);
                buffer->SetInt64Data(self->output->GetInt64Data());
            } else {
                self->output->SetInt64Data(entry->time);
            }
        }

        s32 result = self->decoder.GetOutputBuffer(entry->index, self->output);
        if ((result == 0 && self->output->Size() > 0) || entry->flags == 4) {
            buffer->SetUint32Data(entry->flags);
            buffer->SetInt64Data(entry->time);
            if (self->chunked) {
                if (buffer->Size() == 0)
                    buffer->SetInt64Data(self->output->GetInt64Data());
                s32 target = self->chunk_size;
                s32 used = buffer->Size();
                s32 capacity = buffer->Capacity();
                s32 available = self->output->Size();
                s32 limit = target > capacity ? capacity : target;
                s32 room = limit > used ? limit - used : 0;
                s32 n = 0;
                if (room > 0 && available > 0)
                    n = available < room ? available : room;
                if (n > 0) {
                    __builtin_memcpy(buffer->Base() + used, self->output->Base() + self->output->Offset(), n);
                    buffer->SetRange(0, n + used);
                    if (n < available) {
                        self->output->SetRange(self->output->Offset() + n, available - n);
                        s64 time = entry->time;
                        s64 delta = self->field_0xc3c > 0 && self->field_0xc2c > 0
                                        ? n * 1000 / self->field_0xc3c / self->field_0xc2c * 1000
                                        : 0;
                        self->output->SetInt64Data(delta + time);
                    } else {
                        self->ClearOutput();
                    }
                }
            } else {
                self->AppendOutput(buffer);
                ++self->field_0xd28;
            }
            if (!self->chunked || buffer->Size() == self->chunk_size || buffer->GetUint32Data() == 4) {
                self->pending.PushBackCopy(*buffer);
                self->queued.EraseAt(0);
                ++self->field_0xd28;
            }
        } else {
            self->ClearOutput();
        }

        self->decoder.ReleaseOutputBufferIndex(entry->index);
        self->outputs.Pop();
    }
    fn_7100066920(&self->mutex);
}
