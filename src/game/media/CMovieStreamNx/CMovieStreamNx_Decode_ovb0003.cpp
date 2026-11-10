// media::CMovieStreamNx (unit 332): the stream's initialisation (fn_7100225b84: the base part, the two
// mutexes, the four decoder events, then the decoder is registered and configured with the source
// track's format) and feeding the decoder the input buffers whose indices the stream has queued
// (fn_7100226068: the ring of words at +0x2c0, under the mutex at +0x2a0). Names and layouts are
// placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

namespace movie {
class Buffer {
public:
    ~Buffer();

private:
    u8 storage[0x40];
};
class MediaData {
public:
    MediaData();
    ~MediaData();
    void Clear();

private:
    u8 storage[0x8];
};
class DecoderEvents;
class Decoder {
public:
    s32 RegisterClientEvents(DecoderEvents* events);
    s32 Configure(MediaData* format);
};
}  // namespace movie

namespace nn::os {
struct MultiWaitHolderType;
void InitializeMultiWaitHolder(MultiWaitHolderType* holder, EventType* event);
void SetMultiWaitHolderUserData(MultiWaitHolderType* holder, u64 user_data);
}  // namespace nn::os

template <class T>
struct UnkMovieRing_ovb0003 {
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
};

// The reader the stream takes its data from (placeholder slot names).
class UnkMovieReader_ovb0003 {
public:
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28(u32 track, movie::MediaData* out);  // the track's format
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual s32 vf_0x50(movie::Buffer* out);  // reads the next chunk
    virtual void vf_0x58();                   // advances
    virtual void vf_0x60(s64* time_us);       // the chunk's time
};

struct UnkMovieOwner_ovb0003 {
    u8 pad_0x0[0xad8];
    UnkMovieReader_ovb0003* reader;  // 0xad8
};

struct UnkMovieStreamNx_ovb0003;

// One of the four decoder events: the event, the multi-wait holder watching it, and what the
// holder's user data points at (the event's kind, the event and the stream).
struct UnkMovieEventSlot_ovb0003 {
    nn::os::EventType event;
    u8 holder[0x48];
    s32 kind;  // 0x70
    nn::os::EventType* event_ptr;
    UnkMovieStreamNx_ovb0003* stream;

    void Init(s32 k, UnkMovieStreamNx_ovb0003* owner) {
        kind = k;
        event_ptr = &event;
        stream = owner;
        nn::os::InitializeEvent(&event, false, nn::os::EventClearMode_ManualClear);
        auto* h = reinterpret_cast<nn::os::MultiWaitHolderType*>(holder);
        nn::os::InitializeMultiWaitHolder(h, &event);
        nn::os::SetMultiWaitHolderUserData(h, reinterpret_cast<u64>(&kind));
    }
};
static_assert(sizeof(UnkMovieEventSlot_ovb0003) == 0x88, "UnkMovieEventSlot_ovb0003 layout");

struct UnkMovieStreamNx_ovb0003 {
    u8 pad_0x0[0x30];
    UnkMovieOwner_ovb0003* owner;  // 0x30
    u32* source;                   // 0x38: its first word is the track
    u8 pad_0x40[0x10];
    movie::Decoder decoder;  // 0x50
    u8 pad_0x51[0xf];
    nn::os::EventType* events[4];  // 0x60, the movie::DecoderEvents
    UnkMovieEventSlot_ovb0003 slots[4];  // 0x80
    nn::os::MutexType mutex;  // 0x2a0
    UnkMovieRing_ovb0003<s32> indices;  // 0x2c0
    nn::os::MutexType mutex_0x3e0;
};
static_assert(__builtin_offsetof(UnkMovieStreamNx_ovb0003, mutex_0x3e0) == 0x3e0, "UnkMovieStreamNx_ovb0003 layout");

void fn_710021b7c4(UnkMovieStreamNx_ovb0003* self);

// Initialises the stream: the base part, the two mutexes, the four decoder events, then registers
// the events with the decoder and configures it with the source track's format.
void fn_7100225b84(UnkMovieStreamNx_ovb0003* self) {
    fn_710021b7c4(self);
    fn_71000668f8(&self->mutex);
    fn_71000668f8(&self->mutex_0x3e0);
    for (s32 i = 0; i < 4; ++i)
        self->slots[i].Init(i, self);
    self->events[0] = &self->slots[0].event;
    self->events[3] = &self->slots[3].event;
    self->events[2] = &self->slots[2].event;
    self->events[1] = &self->slots[1].event;
    self->decoder.RegisterClientEvents(reinterpret_cast<movie::DecoderEvents*>(self->events));
    movie::MediaData format;
    self->owner->reader->vf_0x28(*self->source, &format);
    self->decoder.Configure(&format);
    format.Clear();
}

// Fills the decoder's input buffer `index` with the reader's next chunk; 1 when it was sent.
s32 fn_7100226104(UnkMovieStreamNx_ovb0003* self, s32 index);

// Takes the next queued input buffer index and fills it; true when one was sent.
bool fn_7100226068(UnkMovieStreamNx_ovb0003* self) {
    fn_710006691c(&self->mutex);
    if (self->indices.count != 0) {
        s32 index = self->indices.PopFront();
        fn_7100066920(&self->mutex);
        if (index != -1 && fn_7100226104(self, index) == 1)
            return true;
    } else {
        fn_7100066920(&self->mutex);
    }
    return false;
}
