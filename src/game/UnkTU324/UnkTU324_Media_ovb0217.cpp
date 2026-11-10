// Unit 324 of tools/tu_map.py: a media instance (src/game/media/UnkMovieUnit324/*.cpp; locks at
// +0x940 and +0x960, state at +0x2c): growing the capacity of a vector of refcounted pointers
// (the elements are copied, then released) and the shutdown that runs from the finished states 10
// and 13: it tells the listeners, releases the tracks and the file table, finalizes the event and
// settles in state 1. Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkErasedVector.h"
#include "types.h"

struct UnkMovieRef_b0217 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 refs;
};

struct UnkMovieSmartPtr_b0217 {
    UnkMovieRef_b0217* ptr;

    void Assign(UnkMovieRef_b0217* p) {
        if (p)
            __atomic_fetch_add(&p->refs, 1, __ATOMIC_ACQ_REL);
        UnkMovieRef_b0217* old = ptr;
        if (old) {
            __atomic_fetch_sub(&old->refs, 1, __ATOMIC_ACQ_REL);
            if (__atomic_load_n(&old->refs, __ATOMIC_ACQUIRE) == 0)
                old->vf_0x10();
        }
        ptr = p;
    }
    void Release() {
        UnkMovieRef_b0217* old = ptr;
        if (old) {
            __atomic_fetch_sub(&old->refs, 1, __ATOMIC_ACQ_REL);
            if (__atomic_load_n(&old->refs, __ATOMIC_ACQUIRE) == 0)
                old->vf_0x10();
        }
    }
};

struct UnkMovieSmartPtrVector_b0217 {
    UnkMovieSmartPtr_b0217* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

void fn_710022037c(UnkMovieSmartPtrVector_b0217* self, u32 n) {
    if (self->capacity < n) {
        auto* p = static_cast<UnkMovieSmartPtr_b0217*>(self->allocator->vf_0x40((u64)n * 8, 8, self->flags));
        if (self->data) {
            UnkMovieSmartPtr_b0217* src = self->data;
            UnkMovieSmartPtr_b0217* dst = p;
            for (u32 i = 0; i < self->count; ++i, ++dst, ++src) {
                dst->ptr = nullptr;
                if (UnkMovieRef_b0217* held = src->ptr) {
                    dst->Assign(held);
                    src->Release();
                }
            }
            self->allocator->vf_0x48(self->data);
        }
        self->data = p;
        self->capacity = n;
    }
}

struct UnkRefObj;
struct UnkMedia324_b0217;

struct UnkMediaListener_b0217 {
    void* context;
    void (*callback)(void* context, UnkMedia324_b0217* media, u32 arg);
};

struct UnkMediaTrackV_b0217 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
};

struct UnkMediaRef_b0217 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 refs;
};

struct UnkMediaOwned_b0217 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
};

struct UnkMedia324_b0217 {
    u8 pad_0x0[0x2c];
    s32 state;           // 0x2c
    s32 previous_state;  // 0x30
    u8 pad_0x34[4];
    UnkMediaOwned_b0217* owned;  // 0x38
    bool owns;                   // 0x40
    u8 pad_0x41[0xf];
    u64 field_0x50, field_0x58;
    UnkMediaRef_b0217** tracks;  // 0x60
    u32 track_count;             // 0x68
    u8 pad_0x6c[0x1c];
    u8 table[0x698];  // 0x88
    UnkMediaListener_b0217* listeners;  // 0x720
    u32 listener_count;                 // 0x728
    u8 pad_0x72c[0x214];
    nn::os::MutexType listener_lock;  // 0x940
    nn::os::MutexType lock;           // 0x960
    nn::os::LightEventType ready;     // 0x980
};

void fn_7100224da0(UnkMedia324_b0217* self);
void fn_71002204d8(void* table, u32 count);

static inline void SetState(UnkMedia324_b0217* self, s32 state) {
    fn_710006691c(&self->listener_lock);
    self->previous_state = self->state;
    self->state = state;
    for (UnkMediaListener_b0217* it = self->listeners, *end = it + self->listener_count; it != end; ++it)
        it->callback(it->context, self, 0);
    fn_7100066920(&self->listener_lock);
}

void fn_710021ea60(UnkRefObj* obj) {
    UnkMedia324_b0217* self = reinterpret_cast<UnkMedia324_b0217*>(obj);
    fn_710006691c(&self->lock);
    if (self->state == 13 || self->state == 10) {
        SetState(self, 11);
        fn_7100224da0(self);
        for (UnkMediaRef_b0217** it = self->tracks, **end = it + self->track_count; it != end; ++it)
            reinterpret_cast<UnkMediaTrackV_b0217*>(*it)->vf_0x28();
        for (u64 i = 0; i < self->track_count; i++) {
            if (UnkMediaRef_b0217* track = self->tracks[i]) {
                __atomic_fetch_sub(&track->refs, 1, __ATOMIC_ACQ_REL);
                if (__atomic_load_n(&track->refs, __ATOMIC_ACQUIRE) == 0)
                    track->vf_0x10();
            }
        }
        self->track_count = 0;
        fn_71002204d8(self->table, 0);
        bool owns = self->owns;
        self->field_0x50 = 0;
        self->field_0x58 = 0;
        if (owns && self->owned) {
            self->owned->vf_0x18();
            self->owned = nullptr;
        }
        nn::os::FinalizeLightEvent(&self->ready);
        SetState(self, 12);
        fn_710006691c(&self->listener_lock);
        self->previous_state = self->state;
        self->state = 1;
        fn_7100066920(&self->listener_lock);
    }
    fn_7100066920(&self->lock);
}
