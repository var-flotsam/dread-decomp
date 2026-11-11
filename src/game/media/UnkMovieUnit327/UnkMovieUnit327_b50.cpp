// Unit 327 (tools/tu_map.py: 0x7100222970-0x710022323c), next to the media::CMovie* units: an object
// with a worker thread at +0x0, a multi-wait over two events (each with a holder and a "linked"
// flag), a mutex at +0x338 and a fixed ring of 64 16-byte entries at +0x358 (with a vtable of its
// own). Names and layouts are placeholders.
#include <nn/os.h>
#include <stdint.h>

#include "base/UnkDefaultAllocator.h"
#include "base/global/timeline/CEvent.h"
#include "game/UnkTU327.h"
#include "game/msapi/res/CRenderRes.h"
#include "types.h"
#include "UnkLeftoverReg.h"

namespace nn {
namespace os {
struct MultiWaitType;
struct MultiWaitHolderType;
void UnlinkMultiWaitHolder(MultiWaitHolderType* holder);
void FinalizeMultiWaitHolder(MultiWaitHolderType* holder);
void FinalizeMultiWait(MultiWaitType* multiWait);
MultiWaitHolderType* WaitAny(MultiWaitType* multiWait);
uintptr_t GetMultiWaitHolderUserData(const MultiWaitHolderType* holder);
void LinkMultiWaitHolder(MultiWaitType* multiWait, MultiWaitHolderType* holder);
}  // namespace os
}  // namespace nn


namespace {

// What a holder's user data points to: the event's kind (what the worker does when it is signalled)
// and the event.
struct EventInfo_b50 {
    u32 kind;
    nn::os::EventType* event;
    void* field_0x10;
};

// An event waited on through a multi-wait holder (`linked` while the holder is in the multi-wait).
struct WaitedEvent_b50 {
    nn::os::EventType event;
    u8 holder[0x40];
    bool linked;
    EventInfo_b50 info;

    __attribute__((always_inline)) void Finalize() {
        nn::os::FinalizeMultiWaitHolder(reinterpret_cast<nn::os::MultiWaitHolderType*>(holder));
        nn::os::FinalizeEvent(&event);
    }
};

struct Entry_b50 {
    void* field_0x0;
    bool field_0x8;
};

}  // namespace

struct UnkMovieUnit327_b50 {
    u8 field_0x0[0x1f8];  // the worker thread
    u8 field_0x1f8[0x30];  // the multi-wait
    WaitedEvent_b50 field_0x228;
    WaitedEvent_b50 field_0x2b0;
    nn::os::MutexType field_0x338;
    void** field_0x358;  // the ring: vtable, read, write, count, entries
    Entry_b50* field_0x360;
    Entry_b50* field_0x368;
    u64 field_0x370;
    Entry_b50 field_0x378[64];
    s32 field_0x778;  // queued "on" entries minus "off" ones
};
static_assert(__builtin_offsetof(UnkMovieUnit327_b50, field_0x378) == 0x378, "layout");

// What a queued entry links or unlinks: four events, each waited on through a holder.
struct UnkMovieEventSet_b50 {
    u8 pad_0x0[0x20];
    WaitedEvent_b50 field_0x20[4];
};

// Takes the queued entries: an "on" entry links its four holders into the multi-wait, an "off"
// entry unlinks and finalizes them.
void fn_7100222d94(UnkMovieUnit327_b50* self) {
    fn_710006691c(&self->field_0x338);
    while (self->field_0x370 != 0) {
        Entry_b50* read = self->field_0x360;
        bool on = read->field_0x8;
        UnkMovieEventSet_b50* set = static_cast<UnkMovieEventSet_b50*>(read->field_0x0);
        if (on) {
            nn::os::MultiWaitType* multiWait = reinterpret_cast<nn::os::MultiWaitType*>(self->field_0x1f8);
            for (s32 i = 0; i < 4; i++) {
                nn::os::LinkMultiWaitHolder(multiWait,
                                            reinterpret_cast<nn::os::MultiWaitHolderType*>(set->field_0x20[i].holder));
                set->field_0x20[i].linked = true;
            }
        } else {
            for (s32 i = 0; i < 4; i++) {
                WaitedEvent_b50& e = set->field_0x20[i];
                if (e.linked) {
                    nn::os::UnlinkMultiWaitHolder(reinterpret_cast<nn::os::MultiWaitHolderType*>(e.holder));
                    e.linked = false;
                }
                e.Finalize();
            }
        }
        Entry_b50* next = self->field_0x360 + 1;
        self->field_0x360 = next == self->field_0x378 + 64 ? self->field_0x378 : next;
        --self->field_0x370;
    }
    fn_7100066920(&self->field_0x338);
}

// The destructor.
void fn_7100222bb0(UnkMovieUnit327_b50* self) {
    fn_7100066918(&self->field_0x338);
    fn_710006686c(self);
    if (self->field_0x2b0.linked) {
        nn::os::UnlinkMultiWaitHolder(reinterpret_cast<nn::os::MultiWaitHolderType*>(self->field_0x2b0.holder));
        self->field_0x2b0.linked = false;
    }
    self->field_0x2b0.Finalize();
    if (self->field_0x228.linked) {
        nn::os::UnlinkMultiWaitHolder(reinterpret_cast<nn::os::MultiWaitHolderType*>(self->field_0x228.holder));
        self->field_0x228.linked = false;
    }
    self->field_0x228.Finalize();
    nn::os::FinalizeMultiWait(reinterpret_cast<nn::os::MultiWaitType*>(self->field_0x1f8));
    self->field_0x358 = g_unk_7101a20570 + 2;
    for (Entry_b50* p = self->field_0x360; p != self->field_0x368;) {
        Entry_b50* next = p + 1;
        p = next == self->field_0x378 + 64 ? self->field_0x378 : next;
    }
    self->field_0x360 = self->field_0x378;
    self->field_0x368 = self->field_0x378;
    self->field_0x370 = 0;
}

// Queues an entry, signals the second event and counts `on` up or down: the first event is cleared
// (and the worker resumed) when the count reaches 1 through an "on" entry, signalled when it drops to 0.
void fn_7100222cc8(UnkMovieUnit327_b50* self, void* item, bool on) {
    fn_710006691c(&self->field_0x338);
    Entry_b50 entry = {item, on};
    *self->field_0x368 = entry;
    Entry_b50* next = self->field_0x368 + 1;
    Entry_b50* write = next == self->field_0x378 + 64 ? self->field_0x378 : next;
    self->field_0x368 = write;
    ++self->field_0x370;
    nn::os::SignalEvent(&self->field_0x2b0.event);
    s32 count = self->field_0x778 + (on ? 1 : -1);
    self->field_0x778 = count;
    if (count == 1 && on) {
        nn::os::ClearEvent(&self->field_0x228.event);
        fn_7100066828(self, 0);
    } else if (count == 0) {
        nn::os::SignalEvent(&self->field_0x228.event);
    }
    fn_7100066920(&self->field_0x338);
}

// The worker's thread: +0x1e4 is its state (3 = stopping), +0x1f0 its argument.
struct UnkMovieWorker_b50 {
    u8 pad_0x0[0x1e4];
    s32 field_0x1e4;
    u8 pad_0x1e8[0x8];
    UnkMovieUnit327_b50* field_0x1f0;
};

namespace movie {
class Buffer {
public:
    Buffer();
    ~Buffer();

private:
    u8 storage[0x40];
};
}  // namespace movie

// The decoder a stream holds at +0x50, as far as this unit calls it (placeholder slot names).
class UnkMovieDecoder_b50 {
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
    virtual s32 vf_0x48(s32* index, s32* remaining);
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual u32 vf_0x60(s32* index, s64* time, u32* flags, s32* remaining);
};

// What events of kinds 0-2 belong to (a stream): bit 1 of +0x40 holds them off.
class UnkMovieEventOwner_b50 {
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
    virtual void vf_0x48();
    virtual void vf_0x50();
    virtual void vf_0x58();
    virtual void vf_0x60();
    virtual void vf_0x68(s32 index, s64 time, u32 flags);

    u8 pad_0x8[0x38];
    u8 field_0x40;
    u8 pad_0x41[0xf];
    UnkMovieDecoder_b50 field_0x50;
};

// Their first parameter is an empty object (passed in x0 with nothing set).
struct UnkMovieEmpty_b50 : UnkLeftoverReg {};
u32 fn_7100223014(UnkMovieEmpty_b50 tag, UnkMovieEventOwner_b50* owner);
void fn_710021b9e4(UnkMovieEventOwner_b50* owner);
void fn_7100222d94(UnkMovieUnit327_b50* self);

// The worker ("MovieDecoderEventHandler"): waits for any linked event and handles it by its kind
// until the state is 3 or the stop event (kind 5) is signalled.
void fn_7100222ab4(UnkMovieWorker_b50* thread) {
    UnkMovieUnit327_b50* self = thread->field_0x1f0;
    while (__atomic_load_n(&thread->field_0x1e4, __ATOMIC_ACQUIRE) != 3) {
        nn::os::MultiWaitHolderType* holder =
            nn::os::WaitAny(reinterpret_cast<nn::os::MultiWaitType*>(self->field_0x1f8));
        EventInfo_b50* info = reinterpret_cast<EventInfo_b50*>(nn::os::GetMultiWaitHolderUserData(holder));
        UnkMovieEventOwner_b50* owner = static_cast<UnkMovieEventOwner_b50*>(info->field_0x10);
        u32 kind = info->kind;
        nn::os::ClearEvent(info->event);
        switch (kind) {
        case 0:
            if (!(owner->field_0x40 & 2))
                fn_7100222f14(UnkMovieEmpty_b50(), owner);
            break;
        case 1:
            if (!(owner->field_0x40 & 2))
                fn_7100223014(UnkMovieEmpty_b50(), owner);
            break;
        case 2:
            fn_710021b9e4(owner);
            break;
        case 4:
            fn_7100222d94(self);
            break;
        case 5:
            return;
        }
    }
}

void fn_7100225fd0(UnkMovieEventOwner_b50* owner, s32 index);
struct UnkPlayer;
void fn_7100224904(UnkPlayer* player, UnkMovieEventOwner_b50* owner);

// Kind 0: takes the decoder's ready input buffers (the first and then `remaining` more), each handed
// to the stream and reported to the player.
s32 fn_7100222f14(UnkMovieEmpty_b50, UnkMovieEventOwner_b50* owner) {
    s32 index = -1;
    movie::Buffer buffer;
    s32 remaining = 0;
    UnkMovieDecoder_b50* decoder = &owner->field_0x50;
    s32 result = decoder->vf_0x48(&index, &remaining);
    if (result == 0) {
        fn_7100225fd0(owner, index);
        fn_7100224904(g_unk_716e554378, owner);
        for (s32 i = 0; i < remaining; i++) {
            s32 more = 0;
            result = decoder->vf_0x48(&index, &more);
            if (result == 0) {
                fn_7100225fd0(owner, index);
                fn_7100224904(g_unk_716e554378, owner);
            }
        }
    }
    return result;
}

// Kind 1: takes the decoder's ready output buffers (the first and then `remaining` more), each
// handed to the stream; 0x2000 counts as success.
u32 fn_7100223014(UnkMovieEmpty_b50, UnkMovieEventOwner_b50* owner) {
    s32 index = -1;
    s64 time = 0;
    u32 flags = 0;
    s32 remaining = 0;
    UnkMovieDecoder_b50* decoder = &owner->field_0x50;
    u32 result = decoder->vf_0x60(&index, &time, &flags, &remaining);
    if ((result & ~0x2000u) == 0) {
        owner->vf_0x68(index, time, flags);
        for (s32 i = 0; i < remaining; i++) {
            s32 more = 0;
            result = decoder->vf_0x60(&index, &time, &flags, &more);
            if ((result & ~0x2000u) == 0)
                owner->vf_0x68(index, time, flags);
        }
    }
    return result;
}
