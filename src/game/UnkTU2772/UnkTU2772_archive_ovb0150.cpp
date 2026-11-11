// nn::atk-ish unit 2772: sound-archive loading (items, groups and their waves), the audio renderer work-buffer
// size, a flag-copy helper and the mixer's per-frame lock/update. Names and layouts are placeholders.
#include <nn/os.h>

#include "game/UnkTU2772.h"
#include "types.h"

struct UnkAtkSrc_b0150 {
    u32 flags;                // 0x0
    u32 v4, v8, vc, v10, v14; // 0x4
    u8 blk18[0x20];           // 0x18
    u8 blk38[0x40];           // 0x38
    u8 blk78[0x90];           // 0x78
    u8 blk108[0x10];          // 0x108
    u8 blk118[0x18];          // 0x118
    u8 blk130[0x18];          // 0x130
    u32 v148, v14c, v150, v154; // 0x148
    u64 v158;                 // 0x158
    u8 v160;                  // 0x160
};

struct UnkAtkDst_b0150 {
    u32 f0, f4, f8, fc, f10, f14, f18, f1c; // 0x0
    void* p20;                // 0x20
    void* p28;
    void* p30;
    void* p38;
    void* p40;
    void* p48;
    u32 f50;
    u32 pad_0x54;
    u64 f58;
    u8 f60;
};

#define SYNC() asm volatile("" ::: "memory")

// Stores into the destination go through an untyped copy so the flag word is re-read after each one.
template <class T>
static inline void Put(void* dst, T v) {
    __builtin_memcpy(dst, &v, sizeof(T));
}

void fn_7101212c28(UnkAtkDst_b0150* d, UnkAtkSrc_b0150* s) {
    if (!s)
        return;
    if (s->flags & 1) {
        Put(&d->f0, s->v4);
        Put(&d->f4, s->v8);
    }
    if (s->flags & 4)
        Put(&d->f14, s->v10);
    if (s->flags & 2)
        Put(&d->f18, s->vc);
    if (s->flags & 8)
        Put(&d->f1c, s->v14);
    if (s->flags & 0x10)
        Put(&d->p20, (void*)s->blk18);
    if (s->flags & 0x20)
        Put(&d->p28, (void*)s->blk38);
    if (s->flags & 0x400)
        Put(&d->p30, (void*)s->blk78);
    if (s->flags & 0x800)
        Put(&d->p38, (void*)s->blk108);
    if (s->flags & 0x40)
        Put(&d->p40, (void*)s->blk118);
    if (s->flags & 0x1000)
        Put(&d->f8, s->v148);
    if ((s->flags & 0x3000) == 0x2000)
        Put(&d->fc, s->v14c);
    if (s->flags & 0x4000)
        Put(&d->f10, s->v150);
    if (s->flags & 0x8000)
        Put(&d->f50, s->v154);
    if (s->flags & 0x10000)
        Put(&d->f58, s->v158);
    if (s->flags & 0x20000)
        Put(&d->p48, (void*)s->blk130);
    if (s->flags & 0x40000)
        Put(&d->f60, s->v160);
}

struct UnkAtkArchive;
struct UnkAtkInfo_b0150;

struct UnkAtkHeap_b0150 {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void* Alloc(u64 size) = 0;  // 0x10
};

struct UnkAtkLoader {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void Register(u32 file_id, void* data) = 0;  // 0x10
    virtual void vf_0x18() = 0;
    virtual void* Find(u32 file_id) = 0;  // 0x20
    UnkAtkArchive* archive;  // 0x8
};

struct UnkAtkInfo_b0150 {
    s32 field_0x0;
    u32 count;  // 0x4
    u8 flag;    // 0x8
    u8 pad_0x9[7];
};

struct UnkAtkInfoFull_b0150 {
    UnkAtkInfo_b0150 head;
    u8 pad_0x10[0x1c];
};

u64 fn_710121212c(UnkAtkLoader* self, u32 file_id, void* dst, u64 size, u32 flag, void* arg);
u32 fn_7101212f04(UnkAtkInfo_b0150* info);
struct UnkAtkWave_b0150;

void* fn_7101211fc8(UnkAtkLoader* self, u32 id, UnkAtkHeap_b0150* heap, void* arg) {
    u32 file_id = fn_71012069f8(self->archive, id);
    void* found = self->Find(file_id);
    if (found)
        return found;
    UnkAtkInfoFull_b0150 full;
    UnkAtkInfo_b0150& info = full.head;
    info.field_0x0 = -1;
    info.flag = 0;
    if (!fn_7101206ed4(self->archive, id, &info))
        return nullptr;
    u32 count = info.count;
    if (count == 0)
        return nullptr;
    if (fn_710121212c(self, file_id, &info, 0x2c, 0, arg) != 0x2c)
        return nullptr;
    u32 size32 = fn_7101212f5c(&info);
    if (fn_7101212f04(&info) > size32)
        return nullptr;
    u64 head = (u64)count << 3;
    // keeps the shift and the low-bit set as two instructions
    asm("" : "+r"(head));
    u64 size = size32;
    head |= 4;
    u8* buf = (u8*)heap->Alloc(head + size);
    if (!buf)
        return nullptr;
    if (fn_710121212c(self, file_id, buf, size, 0, arg) != size)
        return nullptr;
    *(u32*)(buf + size) = 0x54415746;
    fn_7101212f94(reinterpret_cast<UnkAtkWave_b0150*>(&info), buf, 1);
    fn_7101213168(&info);
    self->Register(file_id, buf);
    return buf;
}

struct UnkAtkWave_b0150 {
    u8 pad_0x0[0x18];
    u8 flag;  // 0x18
};

struct UnkAtkWaveBody_b0150 {
    u32 pad_0x0[2];
    u8 body[0x34];  // 0x8
};

struct UnkAtkWaveHeader_b0150 {
    u32 magic;     // 0x0
    u32 file_id;   // 0x4
    u32 index;     // 0x8
    u8 body[0x34]; // 0xc
};

u32 fn_7101213218(UnkAtkWave_b0150* w, u32 index);
u32 fn_710121323c(UnkAtkWave_b0150* w, u32 index);
void fn_7101215460(void* data, u64 size);

bool fn_7101211e48(UnkAtkLoader* self, u32 id, u32 index, UnkAtkHeap_b0150* heap, void* arg) {
    u32 file_id = fn_71012069f8(self->archive, id);
    void* data = self->Find(file_id);
    if (!data) {
        data = fn_7101211fc8(self, id, heap, arg);
        if (!data)
            return false;
    }
    UnkAtkWave_b0150 wave;
    fn_7101212f94(&wave, data, 1);
    if (wave.flag) {
        if (fn_71012131a0(&wave, index))
            return true;
    }
    u32 size = fn_7101213218(&wave, index);
    u8* raw = (u8*)heap->Alloc(((u64)size + 0x107f) & 0x1fffff000);
    if (!raw)
        return false;
    UnkAtkWaveBody_b0150 tmp;
    UnkAtkWaveHeader_b0150* hdr = (UnkAtkWaveHeader_b0150*)(((u64)raw + 0x3f) & ~0x3full);
    u8* body = (u8*)hdr + 0x40;
    hdr->magic = 0x56574946;
    hdr->file_id = file_id;
    hdr->index = index;
    __builtin_memcpy(hdr->body, tmp.body, sizeof(tmp.body));
    u32 flag = fn_710121323c(&wave, index);
    if (fn_710121212c(self, file_id, body, size, flag, arg) != size)
        return false;
    fn_7101213294(&wave, index, body);
    fn_7101215460(body, size);
    return true;
}

struct UnkAtkRefs_b0150 {
    u32 count;
    u32 ids[1];  // 0x4
};

struct UnkAtkGroupEntry_b0150 {
    u32 id;     // 0x0
    u32 index;  // 0x4
};

struct UnkAtkGroup_b0150 {
    u32 count;  // 0x0
    UnkAtkGroupEntry_b0150 entries[1];  // 0x4
};

struct UnkAtkCursor_b0150 {
    u8 pad_0x0[0x18];
};

void fn_7101215788(UnkAtkCursor_b0150* cursor, void* data);
UnkAtkGroup_b0150* fn_7101215a38(UnkAtkCursor_b0150* cursor);
UnkAtkRefs_b0150* fn_7101207090(UnkAtkArchive* archive, u32 id);

bool fn_7101211a64(UnkAtkLoader* self, u32 id, void* heap, u32 flags, void* arg) {
    u32 file_id = fn_71012069f8(self->archive, id);
    if (flags & 4) {
        if (!fn_7101211d30(self, file_id, heap, arg, 0))
            return false;
    }
    if (!(flags & 8))
        return true;
    UnkAtkInfo_b0150 info;
    void* data = self->Find(file_id);
    if (data) {
        UnkAtkCursor_b0150 cursor;
        fn_7101215788(&cursor, data);
        UnkAtkGroup_b0150* group = fn_7101215a38(&cursor);
        if (!group)
            return false;
        for (u64 i = 0; i < group->count; i++) {
            u32 sub = group->entries[i].id;
            u32 index = group->entries[i].index;
            info.field_0x0 = -1;
            info.flag = 0;
            if (!fn_7101206ed4(self->archive, sub, &info))
                return false;
            if (info.flag) {
                if (!fn_7101211e48(self, sub, index, (UnkAtkHeap_b0150*)heap, arg))
                    return false;
            } else {
                if (!fn_7101211d30(self, fn_71012069f8(self->archive, sub), heap, arg, 1))
                    return false;
            }
        }
    } else {
        UnkAtkRefs_b0150* refs = fn_7101207090(self->archive, id);
        for (u64 i = 0; i < refs->count; i++) {
            u32 sub = refs->ids[i];
            if (!fn_7101211d30(self, fn_71012069f8(self->archive, sub), heap, arg, 1))
                return false;
        }
    }
    return true;
}

struct UnkAtkItemRefs;
struct UnkAtkLoaderObject_b0148;

struct UnkAtkItemRefs {
    u32 field_0x0;
    u32 ids[4];  // 0x4
    u32 field_0x14;
    u16 field_0x18;
};

void fn_71012124d8(UnkAtkLoader* self, u32 id, void* arg) {
    alignas(8) u32 file_id;
    UnkAtkCursor_b0150 cursor;
    UnkAtkItemRefs refs;
    refs.field_0x0 = 0;
    refs.field_0x14 = 0;
    refs.field_0x18 = 0;
    refs.ids[0] = refs.ids[1] = refs.ids[2] = refs.ids[3] = 0xffffffff;
    if (!fn_7101206b4c(self->archive, &refs, id))
        return;
    for (int i = 0; i < 4; i++) {
        u32 item = refs.ids[i];
        if (item == 0xffffffff)
            continue;
        file_id = 0xffffffff;
        if (!fn_7101206e60(self->archive, &file_id, item))
            continue;
        void* data = self->Find(file_id);
        if (!data)
            continue;
        fn_7101215788(&cursor, data);
        UnkAtkGroup_b0150* group = fn_7101215a38(&cursor);
        if (group && group->count)
            fn_710121279c((UnkAtkLoaderObject_b0148*)self, group->entries[0].id, arg);
    }
}

struct UnkAtkListLink_b0150 {
    UnkAtkListLink_b0150* prev;
    UnkAtkListLink_b0150* next;
};

struct UnkAtkRegistry_b0147;
struct UnkAtkFlaggedList_b47a3;

void* fn_71001039f8();
u32 fn_7101236944(void* bank, u32 index);
void fn_7101238380(void* bank, u32 index, u32 value);
void fn_7101237944(void* item, s32 arg);
UnkAtkRegistry_b0147* fn_710122edb0();
void fn_710122f510(UnkAtkFlaggedList_b47a3* self, u32 mask);

struct UnkAtkMixer_b0150 {
    u8 pad_0x0[0xf4];
    s32 limit_a;   // 0xf4
    s32 cur_a;     // 0xf8
    u8 pad_0xfc[8];
    s32 limit_b;   // 0x104
    s32 cur_b;     // 0x108
    u8 pad_0x10c[0x5a8 - 0x10c];
    u8 bank_a[0xf8];            // 0x5a8
    u8 bank_b[0x1f0];  // 0x6a0
    UnkAtkListLink_b0150 items;  // 0x890
    u8 pad_0x8a0[0x950 - 0x8a0];
    bool on_950;   // 0x950
    bool on_951;
    bool on_952;
    u8 pad_0x953[0x980 - 0x953];
    nn::os::MutexType mutex;     // 0x980
};

static inline nn::os::MutexType* AtkLock_b0150() {
    return reinterpret_cast<nn::os::MutexType*>(static_cast<char*>(fn_71001039f8()) + 0x8a0);
}

static inline void Toggle(void* bank, u32 index) {
    u32 v = fn_7101236944(bank, index);
    fn_7101238380(bank, index, ~v & 1);
}

void fn_7101214eec(UnkAtkMixer_b0150* self, s32 amount) {
    nn::os::LockMutex(&self->mutex);
    if (self->on_950 && self->on_951 && *((u8*)self + 0xa64)) {
        void* a = self->bank_a;
        Toggle(a, 1);
        Toggle(a, 2);
        Toggle(a, 3);
        if (self->on_952) {
            void* b = self->bank_b;
            Toggle(b, 1);
            Toggle(b, 2);
        }
    }
    nn::os::LockMutex(AtkLock_b0150());
    for (UnkAtkListLink_b0150* it = self->items.next; it != &self->items; it = it->next)
        fn_7101237944((u8*)it - 0x40, amount);
    nn::os::UnlockMutex(AtkLock_b0150());
    if (self->cur_a < self->limit_a) {
        s32 v = self->cur_a + amount;
        if (v > self->limit_a)
            v = self->limit_a;
        self->cur_a = v;
        fn_710122f510((UnkAtkFlaggedList_b47a3*)fn_710122edb0(), 0x40);
    }
    if (self->cur_b < self->limit_b) {
        s32 v = self->cur_b + amount;
        if (v > self->limit_b)
            v = self->limit_b;
        self->cur_b = v;
        fn_710122f510((UnkAtkFlaggedList_b47a3*)fn_710122edb0(), 0x40);
    }
    nn::os::UnlockMutex(&self->mutex);
}

namespace nn::audio {
struct AudioRendererParameter;
void InitializeAudioRendererParameter(AudioRendererParameter* parameter);
u64 GetAudioRendererWorkBufferSize(const AudioRendererParameter& parameter);
u64 GetAudioRendererConfigWorkBufferSize(const AudioRendererParameter& parameter);
}  // namespace nn::audio

u64 fn_7101236518(bool flag);
u64 fn_710123733c(u32 a, u32 b, u32 c, u32 d, bool e, u32 f);

struct UnkAtkRendererConfig_b0150 {
    u32 sample_rate;     // 0x0
    u32 field_0x4;
    u32 field_0x8;
    u8 pad_0xc[4];
    u32 field_0x10;
    u32 field_0x14;
    u8 b18;              // 0x18
    u8 b19;
    u8 b1a;
    u8 b1b;
    u8 b1c;
    u8 b1d;
    u8 b1e;
    u8 b1f;
    u8 pad_0x20;
    u8 b21;              // 0x21
    u8 pad_0x22[5];
    u8 b27;              // 0x27
};

struct UnkAtkRendererSize_b0150 {
    u32 sample_rate;     // 0x0
    u32 sample_count;    // 0x4
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    u8 field_0x20;
    u8 pad_0x21[0xb];
    u32 field_0x2c;
    u32 field_0x30;
    u8 pad_0x34[4];
};

u64 fn_71012139fc(void* self, UnkAtkRendererConfig_b0150* cfg) {
    bool flag_b1b = cfg->b1b;
    UnkAtkRendererSize_b0150 p;
    nn::audio::InitializeAudioRendererParameter(reinterpret_cast<nn::audio::AudioRendererParameter*>(&p));
    u32 rate = cfg->sample_rate;
    p.sample_rate = rate;
    p.sample_count = rate * 5 / 1000;
    p.field_0x8 = cfg->field_0x14;
    p.field_0xc = cfg->field_0x10;
    p.field_0x10 = cfg->field_0x8;
    p.field_0x14 = 1;
    p.field_0x18 = cfg->field_0x4 + 4;
    p.field_0x1c = 0;
    p.field_0x20 = cfg->b21;
    if (cfg->b18)
        p.field_0x1c = 3;
    if (!cfg->b1b)
        p.field_0x18 = 0;
    u32 v = 2;
    if (cfg->b1c) {
        p.field_0x14 = 2;
        v = 3;
    }
    if (cfg->b1d)
        p.field_0x14 = v;
    if (cfg->b27) {
        p.field_0x2c = 1;
        p.field_0x30 = 1;
    }
    bool flag = flag_b1b;
    u64 size = nn::audio::GetAudioRendererWorkBufferSize(*reinterpret_cast<nn::audio::AudioRendererParameter*>(&p));
    size += nn::audio::GetAudioRendererConfigWorkBufferSize(*reinterpret_cast<nn::audio::AudioRendererParameter*>(&p));
    size += fn_7101236518(flag);
    if (cfg->b1e) {
        u32 first = cfg->b1b ? 4 : 1;
        u32 n = cfg->b1f ? 2 : 6;
        bool b1a = cfg->b1a;
        size += fn_710123733c(first, n, 1, n, flag, 1);
        if (b1a)
            size += fn_710123733c(1, n, 1, n, flag, 1) + 8;
        if (cfg->b19)
            size += fn_710123733c(3, 2, 1, n, flag, 1) + 8;
    }
    return size;
}
