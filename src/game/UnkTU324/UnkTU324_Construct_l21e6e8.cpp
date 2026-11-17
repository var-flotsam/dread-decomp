// Unit 324 (media instance): the constructor. Sets the vtable (twice, base then derived), the state
// fields, a file-path pair and a string id copy, four vectors, a listener queue and three mutexes.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkDefaultAllocator.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkTU324.h"
#include "game/media/CMediaStream.h"
#include "types.h"


struct UnkMediaListener324;

struct UnkMediaConstruct324 {
    void** vtable;                              // 0x0
    u32 field_0x8;                              // 0x8, released
    s32 field_0xc;                              // 0xc
    u8 field_0x10[0x10];                        // 0x10, a file-path pair
    base::global::CStrId id;                    // 0x20
    f32 field_0x28;                             // 0x28
    s32 state;                                  // 0x2c
    s32 previous_state;                         // 0x30
    s32 field_0x34;                             // 0x34
    u8 pad_0x38[0x8];
    bool field_0x40;                            // 0x40
    u8 pad_0x41[0xf];
    u64 field_0x50;                             // 0x50
    u64 field_0x58;                             // 0x58
    base::global::CRntVector<void*> field_0x60;  // 0x60
    u32 field_0x88;                             // 0x88
    u8 pad_0x8c[0x644];
    base::global::CRntVector<void*> lists[2];   // 0x6d0
    base::global::CRntVector<void*> field_0x720;  // 0x720
    u8 pad_0x748[0x1e4];
    u32 field_0x92c;                            // 0x92c
    u32 field_0x930;                            // 0x930
    u32 field_0x934;                            // 0x934
    u32 field_0x938;                            // 0x938
    u32 field_0x93c;                            // 0x93c
    nn::os::MutexType listener_lock;            // 0x940
    nn::os::MutexType lock;                     // 0x960
    u8 pad_0x980[0x10];
    nn::os::MutexType field_0x990;              // 0x990
    u8 pad_0x9b0[0x0];
    void** queue_vtable;                        // 0x9b0
    void* queue_head;                           // 0x9b8
    void* queue_tail;                           // 0x9c0
    u64 queue_count;                            // 0x9c8
    u8 queue_storage[0x100];                    // 0x9d0
    u32 field_0xad0;                            // 0xad0
    bool field_0xad4;                           // 0xad4
};
static_assert(__builtin_offsetof(UnkMediaConstruct324, lists) == 0x6d0, "UnkMediaConstruct324 layout");
static_assert(__builtin_offsetof(UnkMediaConstruct324, listener_lock) == 0x940, "UnkMediaConstruct324 layout");
static_assert(__builtin_offsetof(UnkMediaConstruct324, queue_vtable) == 0x9b0, "UnkMediaConstruct324 layout");

void fn_710021e6e8(UnkMediaConstruct324* self) {
    self->vtable = reinterpret_cast<void**>(g_unk_7101a203f8 + 0x10);
    __atomic_store_n(&self->field_0x8, 0, __ATOMIC_RELEASE);
    self->vtable = g_unk_7101a20498 + 2;
    self->field_0xc = 0x7fffffff;
    fn_710008d2ec(reinterpret_cast<UnkStrPair_0x10*>(self->field_0x10), &g_unk_7101d470e8);
    new (&self->id) base::global::CStrId(g_unk_7101d470e0);
    self->field_0x28 = 1.0f;
    self->state = 0;
    self->previous_state = 0;
    self->field_0x34 = 0x7fffffff;
    self->field_0x40 = false;
    new (&self->field_0x60) base::global::CRntVector<void*>(0xf);
    self->field_0x88 = 0;
    for (base::global::CRntVector<void*>& list : self->lists)
        new (&list) base::global::CRntVector<void*>(0xf);
    new (&self->field_0x720) base::global::CRntVector<void*>(0xf);
    nn::os::MutexType* locks[] = {&self->lock, &self->listener_lock, &self->field_0x990};
    self->field_0x92c = 0;
    self->field_0x930 = 0;
    self->field_0x934 = 0;
    self->field_0x938 = 0;
    self->field_0x93c = 0;
    self->queue_vtable = g_unk_7101a204c0 + 2;
    self->queue_head = self->queue_storage;
    self->queue_tail = self->queue_storage;
    self->queue_count = 0;
    self->field_0xad0 = 0;
    self->field_0xad4 = false;
    for (nn::os::MutexType* lock : locks)
        fn_71000668f8(lock);
    self->field_0x50 = 0;
    self->field_0x58 = 0;
}
