// nn::atk-ish unit 2772: sound-system helpers: the audio renderer's sink setup and update, the voice
// pool's channel lists and a few small accessors on parameter blocks. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU2772.h"
#include "types.h"

extern "C" int __cxa_guard_acquire(u64* guard);
extern "C" void __cxa_guard_release(u64* guard);

void* fn_71001039f8();
struct UnkAtkLock;

struct UnkAtkListLink_b0148 {
    UnkAtkListLink_b0148* prev;
    UnkAtkListLink_b0148* next;
};

// ---- voice arrays and the pool singleton ----
struct UnkAtkVoiceArr_b0148 {
    u8 pad_0x0[0x1c4];
    s32 count;  // 0x1c4
};


struct UnkAtkSingleton_b0148 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    UnkAtkListLink_b0148 link;  // 0x18
    u8 flag;  // 0x28
    u8 pad_0x29[3];
    u8 zeros[0x1c];  // 0x2c
};

struct UnkAtkSingletonGuarded_b0148 {
    u64 guard;
    UnkAtkSingleton_b0148 value;  // 0x8
};
extern UnkAtkSingletonGuarded_b0148 g_unk_7172a18588 __attribute__((visibility("hidden")));

UnkAtkSingleton_b0148* fn_710122c6e8() {
    if (__builtin_expect(!(__atomic_load_n(reinterpret_cast<u8*>(&g_unk_7172a18588.guard), __ATOMIC_ACQUIRE) & 1), 0)) {
        if (__cxa_guard_acquire(&g_unk_7172a18588.guard)) {
            UnkAtkSingleton_b0148* s = &g_unk_7172a18588.value;
            s->flag = 0;
            s->field_0x0 = 0;
            s->field_0x8 = 0;
            s->field_0x10 = 0;
            s->link.prev = &s->link;
            s->link.next = &s->link;
            __builtin_memset(s->zeros, 0, 8);
            __builtin_memset(s->zeros + 8, 0, 8);
            __builtin_memset(s->zeros + 16, 0, 8);
            __builtin_memset(s->zeros + 24, 0, 4);
            __cxa_guard_release(&g_unk_7172a18588.guard);
        }
    }
    return &g_unk_7172a18588.value;
}

struct UnkAtkVoiceElem_b0148 {
    u8 pad_0x0[0x7e];
    u8 flag;  // 0x7e
    u8 pad_0x7f[0x61];
};

void fn_710122eb34(UnkAtkVoiceArr_b0148* self, u8 v) {
    s32 n = self->count;
    UnkAtkVoiceElem_b0148* e = reinterpret_cast<UnkAtkVoiceElem_b0148*>(self);
    for (s64 i = 0; i < n; i++)
        e[i].flag = v;
}

void fn_710122d6a4(UnkAtkVoiceArr_b0148* self, u32 v) {
    s32 n = self->count;
    *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + 0x268) = v;
    for (s64 i = 0; i < n; i++)
        *reinterpret_cast<u32*>(reinterpret_cast<u8*>(self) + i * 0xe0 + 0x80) = v;
}

void fn_710122e04c(UnkAtkVoiceArr_b0148* self, u64 v) {
    s32 n = self->count;
    if (n < 1)
        return;
    *reinterpret_cast<u64*>(reinterpret_cast<u8*>(self) + 0x280) = v;
    for (s64 i = 0; i < n; i++)
        *reinterpret_cast<u64*>(reinterpret_cast<u8*>(self) + i * 0xe0 + 0xa8) = v;
}

struct UnkAtkRingElem_b0148 {
    void* dst;  // 0x0
    u64 field_0x8;
    u64 size;  // 0x10
};

struct UnkAtkRing_b0148 {
    UnkAtkRingElem_b0148* elems;  // 0x0
    s32 capacity;  // 0x8
    s32 head;  // 0xc
    s32 tail;  // 0x10
};

void fn_710123891c(UnkAtkRing_b0148* self, const void* src, u64 n, u64 size) {
    if (__atomic_load_n(&self->head, __ATOMIC_ACQUIRE) == __atomic_load_n(&self->tail, __ATOMIC_ACQUIRE))
        return;
    s32 i = __atomic_load_n(&self->head, __ATOMIC_ACQUIRE);
    __builtin_memcpy(&self->elems[i].size, &size, 8);
    __builtin_memcpy(self->elems[i].dst, src, n);
    __atomic_store_n(&self->head, i + 1 >= self->capacity ? 0 : i + 1, __ATOMIC_RELEASE);
}

// ---- list under the sound lock ----
void fn_7101236af4(void* object);

struct UnkAtkMgr_b0148 {
    u8 pad_0x0[0x558];
    u8 field_0x558[0x38];
    u8 pad_0x590[0x890 - 0x590];
    UnkAtkListLink_b0148 list;  // 0x890
    u8 pad_0x8a0[0x950 - 0x8a0];
    bool field_0x950;
    bool field_0x951;
};

struct UnkAtkSlotMgr_b0148 {
    u8 pad_0x0[0x5a8];
    u8 slots[0x2e8];  // 0x5a8, entries of 0xf8 bytes
    UnkAtkListLink_b0148 list;  // 0x890
    u8 pad_0x8a0[0x951 - 0x8a0];
    bool field_0x951;
};

static inline UnkAtkLock* AtkLock() {
    return reinterpret_cast<UnkAtkLock*>(static_cast<char*>(fn_71001039f8()) + 0x8a0);
}

void fn_7101215088(UnkAtkMgr_b0148* self) {
    if (!self->field_0x950)
        return;
    fn_71012392d0(AtkLock());
    for (UnkAtkListLink_b0148* node = self->list.next; node != &self->list; node = node->next)
        fn_7101236af4(reinterpret_cast<u8*>(node) - 0x40);
    fn_71012392e0(AtkLock());
    fn_7101236af4(self->field_0x558);
}


// Entry idx: the slot array's, or (when the manager is not in its fixed mode) the idx-th node of the
// list, found under the lock.
static inline u8* SlotOrNode(UnkAtkSlotMgr_b0148* self, s32 idx) {
    u8* result;
    bool useSlot = true;
    if (!self->field_0x951) {
        fn_71012392d0(AtkLock());
        UnkAtkListLink_b0148* head = &self->list;
        UnkAtkListLink_b0148* node = head->next;
        if (node != head) {
            s32 n = idx + 1;
            while (true) {
                if (--n == 0) {
                    useSlot = false;
                    result = reinterpret_cast<u8*>(node) - 0x40;
                    break;
                }
                node = node->next;
                if (node == head)
                    break;
            }
        }
        fn_71012392e0(AtkLock());
    }
    if (useSlot)
        result = self->slots + (s64)idx * 0xf8;
    return result;
}

u8* fn_71012151e4(UnkAtkSlotMgr_b0148* self, s32 idx) {
    return SlotOrNode(self, idx);
}

u8* fn_7101215288(UnkAtkSlotMgr_b0148* self, s32 idx) {
    return SlotOrNode(self, idx);
}

// ---- ramps and slots ----
struct UnkAtkRamp_b0148 {
    u8 pad_0x0[0xec];
    f32 from;  // 0xec
    f32 to;  // 0xf0
    s32 total;  // 0xf4
    s32 elapsed;  // 0xf8
    f32 base;  // 0xfc
    f32 scale;  // 0x100
    s32 total2;  // 0x104
    s32 elapsed2;  // 0x108
};

f32 fn_710121535c(UnkAtkRamp_b0148* self) {
    f32 v;
    if (self->elapsed < self->total)
        v = self->from + (self->to - self->from) * (f32)self->elapsed / (f32)self->total;
    else
        v = self->to;
    if (self->elapsed2 >= self->total2)
        return v * self->scale;
    return v * (self->base + (self->scale - self->base) * (f32)self->elapsed2 / (f32)self->total2);
}

void fn_71012382b8(void* object, s32 n, f32 v);
void fn_7101237c78(void* object, s32 n);

struct UnkAtkSlots_b0148 {
    u8 pad_0x0[0x5a8];
    u8 slots[0xf8 * 4];  // 0x5a8
};

void fn_71012153d8(UnkAtkSlots_b0148* self, f32 v, s32 index, s32 skip, s32 other) {
    f32 clamped = !(v <= -128.0f) ? v : -128.0f;
    if (v > 128.0f)
        clamped = 128.0f;
    fn_71012382b8(other != 0 ? &self->slots[0xf8] : &self->slots[0], index + 1, clamped);
    if (skip == 0)
        fn_7101237c78(self->slots + (s64)other * 0xf8, index + 1);
}

// ---- audio renderer ----
extern const s8 g_unk_710164d634[] __attribute__((visibility("hidden")));
u32 fn_710123a3c0(void* config, void* sink, void* mix, const s8* input, s32 input_count, void* buffer,
                  u64 buffer_size, s32 format);
u64 fn_710123a400(void* handle0, void* handle1, void* config);
[[noreturn]] void fn_71012390c0(const char* a, const char* b, const char* c, u32 line);

struct UnkAtkRenderer_b0148 {
    u8 pad_0x0[0x8];
    void* handle0;  // 0x8
    void* handle1;  // 0x10
    u8 config[0xb0];  // 0x18
    u64 updates;  // 0xc8
    u8 pad_0xd0[0x598 - 0xd0];
    u8 mix[0x10];  // 0x598
    u8 pad_0x5a8[0x954 - 0x5a8];
    bool field_0x954;
    u8 pad_0x955[2];
    bool field_0x957;
    u8 pad_0x958[0xa00 - 0x958];
    u8 sink[0x8];  // 0xa00
    u32 status;  // 0xa08
    u8 pad_0xa0c[0xa18 - 0xa0c];
    void* buffer;  // 0xa18
    u8 pad_0xa20[0xa30 - 0xa20];
    u64 buffer_size;  // 0xa30
    u8 pad_0xa38[0xa40 - 0xa38];
    u8 sink2[0x8];  // 0xa40
    u8 pad_0xa48[0xa50 - 0xa48];
    void* buffer2;  // 0xa50
    u64 buffer_size2;  // 0xa58
    u32 state;  // 0xa60
};

static inline UnkAtkLock* AtkTaskLock() {
    return reinterpret_cast<UnkAtkLock*>(static_cast<char*>(fn_71001039f8()) + 0x960);
}

struct UnkAtkTaskManager;

u64 fn_7101214040(UnkAtkTaskManager* manager) {
    UnkAtkRenderer_b0148* self = reinterpret_cast<UnkAtkRenderer_b0148*>(manager);
    fn_71012392d0(AtkTaskLock());
    u64 result = fn_710123a400(self->handle0, self->handle1, self->config);
    __atomic_fetch_add(&self->updates, 1, __ATOMIC_SEQ_CST);
    u32 kind;
    if ((u32)result == 0)
        goto clear;
    if (((u32)result & 0x1ff) == 0x99) {
        kind = (u32)result & 0x3ffe00;
        if (kind == 0x5400) {
            if (!self->field_0x957) {
            clear:
                result = 0;
            } else {
                fn_71012390c0(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0);
            }
        } else if (kind == 0x5200) {
            fn_71012390c0(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0);
        }
    }
    fn_71012392e0(AtkTaskLock());
    return (u32)result;
}

void fn_7101213dd8(UnkAtkRenderer_b0148* self) {
    fn_71012392d0(AtkTaskLock());
    alignas(8) u32 result = fn_710123a3c0(self->config, self->sink, self->mix, g_unk_710164d634, self->field_0x954 ? 2 : 6,
                               self->buffer, self->buffer_size, 2);
    if (result != 0)
        fn_71012390d0(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0, &result, g_unk_710162e2ed);
    fn_71012392e0(AtkTaskLock());
    self->status = 1;
}

void fn_7101213e74(UnkAtkRenderer_b0148* self, u32 flags) {
    if (!(flags & 1) && self->state != 2)
        return;
    fn_71012392d0(AtkTaskLock());
    alignas(8) u32 result = fn_710123a3c0(self->config, self->sink2, self->mix, g_unk_710164d634, self->field_0x954 ? 2 : 6,
                               self->buffer2, self->buffer_size2, 2);
    if (result != 0)
        fn_71012390d0(g_unk_710162e2ed, g_unk_710162e2ed, g_unk_710162e2ed, 0, &result, g_unk_710162e2ed);
    fn_71012392e0(AtkTaskLock());
    self->state = 1;
}

// ---- 710121050c ----
struct UnkAtkIdList_b0148 {
    u32 field_0x0;
    s32 offset;  // 0x4
};

void fn_710121050c(UnkAtkIdList_b0148* self, u32* out) {
    const u32* ids = reinterpret_cast<const u32*>(reinterpret_cast<const u8*>(self) + self->offset);
    out[0] = ids[0] >= 1 ? ids[1] : 0xffffffff;
    out[1] = ids[0] >= 2 ? ids[2] : 0xffffffff;
    out[2] = ids[0] >= 3 ? ids[3] : 0xffffffff;
    out[3] = ids[0] >= 4 ? ids[4] : 0xffffffff;
}

// ---- 7101212e14 ----
struct UnkAtkBytes_b0148 {
    u8* data;  // 0x0
    s32 size;  // 0x8
    u8 pad_0xc[4];
    u8 flag;  // 0x10
    u8 pad_0x11[3];
    u32 extra;  // 0x14
};

void fn_7101212e14(UnkAtkBytes_b0148* dst, UnkAtkBytes_b0148* src) {
    s32 n = src->size < dst->size ? src->size : dst->size;
    for (s32 i = 0; i < n; i++)
        dst->data[i] = src->data[i];
    for (s32 i = n; i < dst->size; i++)
        dst->data[i] = 0;
    dst->flag = src->flag;
    dst->extra = src->extra;
}

// ---- 71012126dc ----
struct UnkAtkArchive_b0148;
struct UnkAtkEntryIndex_b0148 {
    u32 index;  // 0x0
    u8 pad_0x4[0x1c];
};
struct UnkAtkEntryInfo_b0148 {
    u32 id;  // 0x0
    u32 field_0x4;
    u16 field_0x8;
    u8 pad_0xa[6];
};
bool fn_7101206ca0(UnkAtkArchive_b0148* archive, s32 key, UnkAtkEntryInfo_b0148* out);

struct UnkAtkBankView_b0148 {
    u8 storage[0x10];
};
void fn_71012304b4(UnkAtkBankView_b0148* view, void* data);

struct UnkAtkLookup_b0148 {
    u32 index;  // 0x0
    u32 field_0x4;
    u32 field_0x8;
    u8 field_0xc;
    u8 pad_0xd[0xb];
};
bool fn_7101230604(UnkAtkBankView_b0148* view, UnkAtkLookup_b0148* out, u32 id, u32 flags);

struct UnkAtkLoaderObject_b0148;
struct UnkAtkLoaderVtable_b0148 {
    void* slots[4];
    void* (*get_data)(UnkAtkLoaderObject_b0148* self, u32 index);  // 0x20
};
struct UnkAtkLoaderObject_b0148 {
    UnkAtkLoaderVtable_b0148* vtable;  // 0x0
    UnkAtkArchive_b0148* archive;  // 0x8
};

void fn_71012126dc(UnkAtkLoaderObject_b0148* self, s32 key, void* out) {
    if (key == -1)
        return;
    UnkAtkEntryIndex_b0148 index;
    if (!fn_7101206ad8(self->archive, &index, key))
        return;
    UnkAtkEntryInfo_b0148 info;
    info.field_0x4 = 0;
    info.field_0x8 = 0;
    if (!fn_7101206ca0(self->archive, key, &info))
        return;
    void* data = self->vtable->get_data(self, index.index);
    if (!data)
        return;
    UnkAtkBankView_b0148 view;
    fn_71012304b4(&view, data);
    UnkAtkLookup_b0148 lookup;
    lookup.field_0xc = 0;
    lookup.field_0x8 = 0;
    if (fn_7101230604(&view, &lookup, info.id, 0))
        fn_710121279c(self, lookup.index, out);
}

// ---- channel pool lists ----
struct UnkListOwner;
struct UnkListItem;

struct UnkAtkPoolItem_b0148 {
    u8 pad_0x0[0xc1];
    bool field_0xc1;
    u8 pad_0xc2[0x130 - 0xc2];
    void* field_0x130;
    u8 pad_0x138[0x190 - 0x138];
    struct UnkAtkPoolState_b0148* field_0x190;
    u8 pad_0x198[0x3d8 - 0x198];
    UnkAtkListLink_b0148 node;  // 0x3d8
};

struct UnkAtkPoolState_b0148 {
    u8 pad_0x0[0x278];
    u32 state;  // 0x278
};

struct UnkAtkPoolOwner_b0148 {
    u8 pad_0x0[0x18];
    UnkAtkListLink_b0148 items;  // 0x18
    u8 pad_0x28[8];
    void* field_0x30;
    void* field_0x38;
};

void fn_710122b9c0(UnkAtkPoolItem_b0148* item, u32 mode, s32 arg);
void* fn_710120f310(void* object);

static inline UnkAtkPoolItem_b0148* PoolItemOf(UnkAtkListLink_b0148* link) {
    return reinterpret_cast<UnkAtkPoolItem_b0148*>(reinterpret_cast<u8*>(link) - 0x3d8);
}

void fn_710122caec(UnkAtkPoolOwner_b0148* self, s32 arg) {
    UnkAtkListLink_b0148* head = &self->items;
    for (UnkAtkListLink_b0148* it = head->next; head != it;) {
        UnkAtkListLink_b0148* next = it->next;
        UnkAtkPoolItem_b0148* item = PoolItemOf(it);
        fn_710122b9c0(item, 1, arg);
        if (!item->field_0xc1) {
            fn_710122c43c(reinterpret_cast<UnkListItem*>(item), 0);
            fn_710122c958(reinterpret_cast<UnkListOwner*>(self), reinterpret_cast<UnkListItem*>(item));
        }
        it = next;
    }
}

void fn_710122cb7c(UnkAtkPoolOwner_b0148* self, s32 arg) {
    UnkAtkListLink_b0148* head = &self->items;
    for (UnkAtkListLink_b0148* it = head->next; head != it;) {
        UnkAtkPoolItem_b0148* item = PoolItemOf(it);
        UnkAtkListLink_b0148* next = it->next;
        bool done = item->field_0xc1;
        if (done) {
            UnkAtkPoolState_b0148* state = item->field_0x190;
            if (state && state->state != 0) {
                it = next;
                continue;
            }
            fn_710122b9c0(item, 1, arg);
            done = item->field_0xc1;
        }
        if (!done) {
            fn_710122c43c(reinterpret_cast<UnkListItem*>(item), 0);
            fn_710122c958(reinterpret_cast<UnkListOwner*>(self), reinterpret_cast<UnkListItem*>(item));
        }
        it = next;
    }
}

void fn_710122c958(UnkListOwner* owner_, UnkListItem* item_) {
    UnkAtkPoolOwner_b0148* owner = reinterpret_cast<UnkAtkPoolOwner_b0148*>(owner_);
    UnkAtkPoolItem_b0148* item = reinterpret_cast<UnkAtkPoolItem_b0148*>(item_);
    void* sub = owner->field_0x38;
    if (sub && owner->field_0x30) {
        void* object = item->field_0x130;
        void* related = fn_710120f310(object);
        if (related)
            fn_710122ae00(sub, related);
        if (object)
            fn_710122ae00(owner->field_0x30, object);
    }
    UnkAtkListLink_b0148* node = &item->node;
    if (&owner->items != node) {
        UnkAtkListLink_b0148* next = node->next;
        UnkAtkListLink_b0148* last = next->prev;
        __builtin_memcpy(&node->prev->next, &next, 8);
        next->prev = node->prev;
        last->next = node;
        node->prev = last;
    } else if (!item) {
        return;
    }
    fn_710122b868(item);
    fn_710122ae00(owner, item);
}

// ---- 710122ef60 ----
struct UnkAtkChannel_b0148;
typedef void (*UnkAtkChannelCallback_b0148)(UnkAtkChannel_b0148* channel, s32 event, void* user);

struct UnkAtkChannel_b0148 {
    u8 pad_0x0[0x1c8];
    UnkAtkChannelCallback_b0148 callback;  // 0x1c8
    void* user;  // 0x1d0
    u8 pad_0x1d8[0x288 - 0x1d8];
    UnkAtkListLink_b0148 link;  // 0x288
};

struct UnkAtkChannelSet_b0148 {
    bool active;  // 0x0
    u8 pad_0x1[7];
    UnkAtkListLink_b0148 running;  // 0x8
    UnkAtkListLink_b0148 stopped;  // 0x18
};

void fn_710122ceac(UnkAtkChannel_b0148* channel);
void fn_710122ce20(UnkAtkChannel_b0148* channel);
void fn_710122cca8(UnkAtkChannel_b0148* channel);

static inline UnkAtkChannel_b0148* ChannelOf(UnkAtkListLink_b0148* link) {
    return reinterpret_cast<UnkAtkChannel_b0148*>(reinterpret_cast<u8*>(link) - 0x288);
}

void fn_710122ef60(UnkAtkChannelSet_b0148* self) {
    if (!self->active)
        return;
    UnkAtkListLink_b0148* running = &self->running;
    for (UnkAtkListLink_b0148* it = self->running.next; it != running; it = self->running.next) {
        UnkAtkChannel_b0148* channel = ChannelOf(it);
        fn_710122ceac(channel);
        if (channel->callback)
            channel->callback(channel, 1, channel->user);
        fn_710122ce20(channel);
    }
    UnkAtkListLink_b0148* stopped = &self->stopped;
    for (UnkAtkListLink_b0148* it = self->stopped.next; it != stopped; it = self->stopped.next) {
        UnkAtkListLink_b0148* prev = it->prev;
        UnkAtkListLink_b0148* next = it->next;
        UnkAtkChannel_b0148* channel = ChannelOf(it);
        UnkAtkListLink_b0148* last = next->prev;
        __builtin_memcpy(&prev->next, &next, 8);
        next->prev = it->prev;
        last->next = it;
        it->prev = last;
        fn_710122cca8(channel);
    }
    self->active = false;
}
