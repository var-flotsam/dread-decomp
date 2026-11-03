// media unit 324 (see UnkMovieUnit324_Pause.cpp for the layout): state changes that keep the
// state left at +0x30 and tell the listeners under the inner lock, all under the outer lock:
// stopping (6, 7 or 9 to 10, after fn_7100224e4c) and playing (5, 7, 9, 10 or 13 to 6, after
// fn_7100224ef4; answers 0, 2 when already playing, 3 otherwise), starting (the same states to 6,
// after clearing +0xad4 and fn_7100224de4; state 4 only sets +0xad4 and answers 4) and seeking (5,
// 6, 7, 9 or 13 to 8 after pausing, keeping the state at +0x34). Names and layouts are
// placeholders.
#include "base/UnkDefaultAllocator.h"
#include "types.h"

struct UnkMedia324;

struct UnkMediaListener324 {
    void* context;
    void (*callback)(void* context, UnkMedia324* media, u32 arg);
};

// The fixed ring of 64 words of UnkMovieUnit324_Rings.cpp.
struct UnkMovieWordRing324 {
    void* vtable;
    u32* read;
    u32* write;
    u64 count;
    u32 entries[64];

    void Push(u32 value) {
        // A store the compiler can't tell apart from the ring's own fields: the position is read again.
        typedef u32 __attribute__((may_alias)) AnyWord;
        *reinterpret_cast<AnyWord*>(write) = value;
        u32* next = write + 1;
        write = next == entries + 64 ? entries : next;
        ++count;
    }

    u32 PopFront() {
        u32 value = *read;
        u32* next = read + 1;
        read = next == entries + 64 ? entries : next;
        --count;
        return value;
    }
};

struct UnkMediaTrack330;  // as src/game/media/UnkUnit330/UnkUnit330.cpp names it

struct UnkMedia324 {
    u8 pad_0x0[0x2c];
    s32 state;           // 0x2c
    s32 previous_state;  // 0x30
    s32 seek_state;      // 0x34
    u8 pad_0x38[0x28];
    UnkMediaTrack330** tracks;  // 0x60
    u32 track_count;            // 0x68
    u8 pad_0x6c[0x6b4];
    UnkMediaListener324* listeners;  // 0x720
    u32 listener_count;              // 0x728
    u8 pad_0x72c[0x1c];
    u8 worker[0x1f8];                 // 0x748, a thread (fn_7100066828 wakes it)
    nn::os::MutexType listener_lock;  // 0x940
    nn::os::MutexType lock;           // 0x960
    nn::os::LightEventType ready;     // 0x980
    u8 pad_0x98c[0x4];
    nn::os::MutexType command_lock;   // 0x990
    UnkMovieWordRing324 commands;     // 0x9b0
    u32 loop_mode;       // 0xad0
    bool start_pending;  // 0xad4
};
static_assert(__builtin_offsetof(UnkMedia324, lock) == 0x960, "UnkMedia324 layout");
static_assert(__builtin_offsetof(UnkMedia324, loop_mode) == 0xad0, "UnkMedia324 layout");

void fn_7100066828(void* thread, u32 a);
void fn_7100066698(nn::os::LightEventType* event);
void fn_710021b860(UnkMediaTrack330* track, bool a, bool b);

// As src/game/UnkUnit330/UnkUnit330Flush.cpp defines it.
struct UnkUnit330Object;
void fn_7100224e4c(UnkUnit330Object* self);
// As src/engine/render/anim/CSkeletalAnimTree/fn_710014be78.cpp defines it.
struct UnkChildOwner60;
void fn_7100224ef4(UnkChildOwner60* self);

// As src/game/media/UnkUnit330/UnkUnit330.cpp defines them.
struct UnkMedia330;
void fn_7100224de4(UnkMedia330* self);
bool fn_7100224fbc(UnkMedia330* self, void* arg);
void fn_7100224f2c(UnkMedia330* self);
// UnkMovieUnit324_Pause.cpp.
u32 fn_710021f950(UnkMedia324* self);
void fn_710021fe0c(UnkMedia324* self);
// As src/game_hi/fn_7100f00f4c.cpp declares it.
struct UnkRefObj;
void fn_710021ea60(UnkRefObj* self);

static inline void SetState(UnkMedia324* self, s32 state) {
    fn_710006691c(&self->listener_lock);
    self->previous_state = self->state;
    self->state = state;
    for (UnkMediaListener324* it = self->listeners, *end = it + self->listener_count; it != end; ++it)
        it->callback(it->context, self, 0);
    fn_7100066920(&self->listener_lock);
}

u32 fn_710021fa20(UnkMedia324* self) {
    fn_710006691c(&self->lock);
    u32 result;
    switch (self->state) {
    case 5:
    case 7:
    case 9:
    case 10:
    case 13:
        fn_7100224ef4(reinterpret_cast<UnkChildOwner60*>(self));
        SetState(self, 6);
        result = 0;
        break;
    case 6:
        result = 2;
        break;
    default:
        result = 3;
        break;
    }
    fn_7100066920(&self->lock);
    return result;
}

void fn_710021fafc(UnkMedia324* self) {
    fn_710006691c(&self->lock);
    switch (self->state) {
    case 6:
    case 7:
    case 9:
        fn_7100224e4c(reinterpret_cast<UnkUnit330Object*>(self));
        SetState(self, 10);
        break;
    }
    fn_7100066920(&self->lock);
}

u32 fn_710021f858(UnkMedia324* self) {
    fn_710006691c(&self->lock);
    u32 result;
    switch (self->state) {
    case 5:
    case 7:
    case 9:
    case 10:
    case 13:
        self->start_pending = false;
        fn_7100224de4(reinterpret_cast<UnkMedia330*>(self));
        SetState(self, 6);
        result = 1;
        break;
    case 4:
        self->start_pending = true;
        result = 4;
        break;
    case 6:
        result = 2;
        break;
    default:
        result = 3;
        break;
    }
    fn_7100066920(&self->lock);
    return result;
}

u32 fn_710021fd10(UnkMedia324* self, void* position) {
    fn_710006691c(&self->lock);
    u32 result;
    switch (self->state) {
    case 5:
    case 6:
    case 7:
    case 9:
    case 13:
        self->seek_state = self->state;
        fn_710021f950(self);
        SetState(self, 8);
        if (fn_7100224fbc(reinterpret_cast<UnkMedia330*>(self), position))
            fn_710021fe0c(self);
        result = 0;
        break;
    case 8:
        result = 2;
        break;
    default:
        result = 3;
        break;
    }
    fn_7100066920(&self->lock);
    return result;
}

// The end of the stream: finished (state 13, after stopping a playing one) when looping is set,
// and restarted when it is 1; otherwise fn_7100224f2c.
void fn_710021fbb4(UnkMedia324* self) {
    u32 loop_mode = self->loop_mode;
    fn_710006691c(&self->lock);
    if (loop_mode == 0) {
        fn_7100224f2c(reinterpret_cast<UnkMedia330*>(self));
        fn_7100066920(&self->lock);
        return;
    }
    switch (self->state) {
    case 6:
    case 7:
    case 9:
        fn_7100224e4c(reinterpret_cast<UnkUnit330Object*>(self));
        SetState(self, 10);
        break;
    }
    fn_7100066920(&self->lock);
    SetState(self, 13);
    if (self->loop_mode == 1)
        fn_710021ea60(reinterpret_cast<UnkRefObj*>(self));
}

// The end of a seek: state 9, then back to the state the seek left (+0x34): playing again through
// fn_710021fa20, or set directly, telling the listeners unless it is 1.
void fn_710021fe0c(UnkMedia324* self) {
    fn_710006691c(&self->lock);
    SetState(self, 9);
    s32 state = self->seek_state;
    if (state == 6) {
        fn_710021fa20(self);
    } else {
        fn_710006691c(&self->listener_lock);
        if (state >= 0) {
            self->previous_state = self->state;
            self->state = state;
            if (state != 1) {
                for (UnkMediaListener324* it = self->listeners, *end = it + self->listener_count; it != end;
                     ++it)
                    it->callback(it->context, self, 0);
            }
        }
        fn_7100066920(&self->listener_lock);
    }
    fn_7100066920(&self->lock);
}

// Queues command 0 for the worker and wakes it.
void fn_71002201b0(UnkMedia324* self) {
    fn_710006691c(&self->command_lock);
    fn_710006691c(&self->command_lock);
    self->commands.Push(0);
    fn_7100066920(&self->command_lock);
    fn_7100066828(self->worker, 1);
    fn_7100066920(&self->command_lock);
}

// Once every track is ready (bit 0 of its +0x40), signals the instance's event and releases each
// track (fn_710021b860 with false, false).
void fn_7100220140(UnkMedia324* self) {
    for (UnkMediaTrack330 **it = self->tracks, **end = it + self->track_count; it != end; ++it) {
        if (!(reinterpret_cast<u8*>(*it)[0x40] & 1))
            return;
    }
    fn_7100066698(&self->ready);
    for (UnkMediaTrack330 **it = self->tracks, **end = it + self->track_count; it != end; ++it)
        fn_710021b860(*it, false, false);
}

bool fn_71000666a0(nn::os::LightEventType* event, u32 milliseconds);

// Waits on the ready event (fn_71000666a0 with a time-out); the signature is the one
// src/game/media/UnkUnit330/UnkUnit330.cpp declares.
void fn_71002202d8(UnkMedia330* self, u32 milliseconds) {
    fn_71000666a0(&reinterpret_cast<UnkMedia324*>(self)->ready, milliseconds);
}

struct UnkMedia324Worker {
    u8 pad_0x0[0x1f0];
    UnkMedia324* media;  // 0x1f0
};

// The worker's command pump: command 0 means the stream ended, handled (fn_710021fbb4) once every
// track has bit 3 of its +0x40 set.
void fn_710021f1f8(UnkMedia324Worker* worker) {
    UnkMedia324* self = worker->media;
    while (self->commands.count != 0) {
        fn_710006691c(&self->command_lock);
        u32 command = self->commands.PopFront();
        fn_7100066920(&self->command_lock);
        if (command != 0)
            continue;
        bool ended = true;
        for (UnkMediaTrack330 **it = self->tracks, **end = it + self->track_count; it != end; ++it) {
            if (!(reinterpret_cast<u8*>(*it)[0x40] & 8)) {
                ended = false;
                break;
            }
        }
        if (ended)
            fn_710021fbb4(self);
    }
}
