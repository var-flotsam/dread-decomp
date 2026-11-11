// Unit 324 of tools/tu_map.py: a media instance. Starts a prepared instance (state 3): tells the
// listeners (state 4), creates the tracks for its two sources and settles in state 5.
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkMovieSmartPtrVector_b0217;
struct UnkMedia324_l21f2a8;

struct UnkMediaTrack_l21f2a8 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    u32 refs;
};

struct UnkMediaTrackPtr_l21f2a8 {
    UnkMediaTrack_l21f2a8* ptr;

    explicit UnkMediaTrackPtr_l21f2a8(UnkMediaTrack_l21f2a8* p) : ptr(p) {
        if (p)
            __atomic_fetch_add(&p->refs, 1, __ATOMIC_ACQ_REL);
    }
    ~UnkMediaTrackPtr_l21f2a8() { Release(); }

    void Assign(UnkMediaTrack_l21f2a8* p) {
        if (p)
            __atomic_fetch_add(&p->refs, 1, __ATOMIC_ACQ_REL);
        Release();
        ptr = p;
    }
    void Release() {
        UnkMediaTrack_l21f2a8* old = ptr;
        if (old) {
            __atomic_fetch_sub(&old->refs, 1, __ATOMIC_ACQ_REL);
            if (__atomic_load_n(&old->refs, __ATOMIC_ACQUIRE) == 0)
                old->vf_0x10();
        }
    }
};

void fn_710022037c(UnkMovieSmartPtrVector_b0217* self, u32 n);

struct UnkMediaTrackVector_l21f2a8 {
    UnkMediaTrackPtr_l21f2a8* data;
    u32 count;
    u32 capacity;
    u32 step;

    void PushBack(const UnkMediaTrackPtr_l21f2a8& value) {
        u32 n = count;
        u32 wanted = capacity;
        if (wanted <= n && step != 0)
            wanted = (n / step + 1) * step;
        if ((s32)n >= (s32)wanted) {
            for (;;) {
            }
        }
        fn_710022037c(reinterpret_cast<UnkMovieSmartPtrVector_b0217*>(this), wanted);
        UnkMediaTrackPtr_l21f2a8* slot = &data[count];
        slot->ptr = nullptr;
        slot->Assign(value.ptr);
        count++;
    }
};

struct UnkMediaListener_l21f2a8 {
    void* context;
    void (*callback)(void* context, UnkMedia324_l21f2a8* media, u32 arg);
};

struct UnkMediaSource_l21f2a8 {
    u64 field_0x0;
    s32 kind;
};

struct UnkMedia324_l21f2a8 {
    u8 pad_0x0[0x2c];
    s32 state;           // 0x2c
    s32 previous_state;  // 0x30
    u8 pad_0x34[0x1c];
    char* video_source;                 // 0x50
    UnkMediaSource_l21f2a8* audio_source;  // 0x58
    UnkMediaTrackVector_l21f2a8 tracks;    // 0x60
    u8 pad_0x78[0x658];
    char** sources;    // 0x6d0
    u32 source_count;  // 0x6d8
    u8 pad_0x6dc[0x44];
    UnkMediaListener_l21f2a8* listeners;  // 0x720
    u32 listener_count;                   // 0x728
    u8 pad_0x72c[0x214];
    nn::os::MutexType listener_lock;  // 0x940
    nn::os::MutexType lock;           // 0x960
};

char* fn_710021bcd0(char* p0, char* p1);
char* fn_710021bd20(char* p0, char* p1);
char* fn_710021bd70(char* p0, char* p1);
void fn_710021f704(UnkMedia324_l21f2a8* self, u32 mode, void* arg);

static inline void SetState(UnkMedia324_l21f2a8* self, s32 state) {
    fn_710006691c(&self->listener_lock);
    self->previous_state = self->state;
    self->state = state;
    for (UnkMediaListener_l21f2a8* it = self->listeners, *end = it + self->listener_count; it != end; ++it)
        it->callback(it->context, self, 0);
    fn_7100066920(&self->listener_lock);
}

static inline void AddTrack(UnkMedia324_l21f2a8* self, char* created) {
    UnkMediaTrack_l21f2a8* track = reinterpret_cast<UnkMediaTrack_l21f2a8*>(created);
    track->vf_0x20();
    UnkMediaTrackPtr_l21f2a8 held(track);
    self->tracks.PushBack(held);
}

struct UnkMediaLock_l21f2a8 {
    nn::os::MutexType* mutex;

    explicit UnkMediaLock_l21f2a8(nn::os::MutexType* m) : mutex(m) { fn_710006691c(m); }
    ~UnkMediaLock_l21f2a8() { fn_7100066920(mutex); }
};

u32 fn_710021f2a8(UnkMedia324_l21f2a8* self, void* arg) {
    UnkMediaLock_l21f2a8 guard(&self->lock);
    switch (self->state) {
    case 3:
        break;
    case 4:
        return 2;
    default:
        return 3;
    }
    SetState(self, 4);
    {
        UnkMediaLock_l21f2a8 inner(&self->lock);
        if (self->source_count != 0)
            self->video_source = self->sources[0];
    }
    if (arg)
        fn_710021f704(self, 1, arg);
    if (self->video_source)
        AddTrack(self, fn_710021bcd0(self->video_source, reinterpret_cast<char*>(self)));
    if (UnkMediaSource_l21f2a8* audio = self->audio_source) {
        if (audio->kind == 5)
            AddTrack(self, fn_710021bd70(reinterpret_cast<char*>(audio), reinterpret_cast<char*>(self)));
        else
            AddTrack(self, fn_710021bd20(reinterpret_cast<char*>(audio), reinterpret_cast<char*>(self)));
    }
    SetState(self, 5);
    return self->state == 5 ? 1 : 4;
}
