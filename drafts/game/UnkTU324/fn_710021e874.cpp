#include "base/UnkDefaultAllocator.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "game/media/CMediaStream.h"
#include "types.h"

struct UnkRefObj;
void fn_710021ea60(UnkRefObj* self);
void fn_71002204d8(void* table, u32 count);
void fn_710006686c(void* thread);

extern void* g_unk_7101a20498[];
extern void* g_unk_7101a204c0[];

struct UnkMovieTrack_b0217 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u32 refs;
};

struct UnkMovieWordRing_b0217 {
    void** vtable;
    u32* read;
    u32* write;
    u64 count;
    u32 entries[64];
};

struct UnkMediaObject_b0217 {
    void** vtable;
    u8 pad_0x8[8];
    UnkStrIdPathPair source;       // 0x10
    base::global::CStrId id;       // 0x20
    u8 pad_0x28[0x38];
    base::UnkVector<UnkMovieTrack_b0217*> tracks;  // 0x60
    u8 pad_0x88[0x648];
    base::UnkVector<u8> list_0x6d0;
    base::UnkVector<u8> list_0x6f8;
    base::UnkVector<u8> list_0x720;
    u8 thread[0x1f8];  // 0x748
    nn::os::MutexType listener_lock;  // 0x940
    nn::os::MutexType lock;           // 0x960
    u8 event_0x980[0x10];
    nn::os::MutexType command_lock;   // 0x990
    UnkMovieWordRing_b0217 commands;  // 0x9b0
};

static_assert(__builtin_offsetof(UnkMediaObject_b0217, list_0x6d0) == 0x6d0, "layout");
static_assert(__builtin_offsetof(UnkMediaObject_b0217, commands) == 0x9b0, "layout");

void fn_710021e874(UnkRefObj* obj) {
    UnkMediaObject_b0217* self = reinterpret_cast<UnkMediaObject_b0217*>(obj);
    self->vtable = g_unk_7101a20498 + 2;
    fn_710021ea60(obj);
    fn_710006686c(self->thread);
    fn_7100066918(&self->command_lock);
    fn_7100066918(&self->lock);
    fn_7100066918(&self->listener_lock);
    self->commands.vtable = g_unk_7101a204c0 + 2;
    for (u32* it = self->commands.read; it != self->commands.write;) {
        u32* first = self->commands.entries;
        u32* limit = first + 64;
        ++it;
        if (it == limit)
            it = first;
    }
    self->commands.read = self->commands.entries;
    self->commands.write = self->commands.entries;
    self->commands.count = 0;
    self->list_0x720.Free();
    self->list_0x6f8.Free();
    self->list_0x6d0.Free();
    fn_71002204d8(reinterpret_cast<u8*>(self) + 0x88, 0);
    if (self->tracks.data) {
        for (u64 i = 0; i < self->tracks.count; i++) {
            if (UnkMovieTrack_b0217* track = self->tracks.data[i]) {
                __atomic_fetch_sub(&track->refs, 1, __ATOMIC_ACQ_REL);
                if (__atomic_load_n(&track->refs, __ATOMIC_ACQUIRE) == 0)
                    track->vf_0x10();
            }
        }
        self->tracks.count = 0;
        self->tracks.allocator->vf_0x48(self->tracks.data);
        self->tracks.capacity = 0;
    }
    self->id.Reset();
    fn_710008d8e8(&self->source);
    self->vtable = reinterpret_cast<void**>(g_unk_7101a203f8 + 0x10);
}
