// nn::atk-ish unit 2772: sound-archive loader lookups and wave-table copy, a sample ring-buffer write and the
// voice wave queue start. Names and layouts are placeholders.
#include <stddef.h>

#include <nn/audio.h>

#include "game/UnkTU2772.h"
#include "types.h"

void* fn_71001039f8();
u32 fn_7101213d2c(char* p0);

struct UnkAtkVoice;

struct UnkAtkArchive;
struct UnkAtkArchive_b0148;

struct UnkAtkEntryIndex_b0148 {
    u32 index;  // 0x0
    u8 pad_0x4[0x1c];
};

struct UnkAtkInfo_b0150 {
    s32 field_0x0;
    u32 count;  // 0x4
    u8 flag;    // 0x8
    u8 pad_0x9[7];
};

struct UnkAtkWave_b0150 {
    u8 pad_0x0[0x18];
    u8 flag;  // 0x18
    u8 pad_0x19[7];
};

bool fn_7101206fac(UnkAtkArchive* archive, u64* out, u32 id);

// ---- 71012128f0 ----
struct UnkAtkLoaderFind_b0268 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void* Find(u32 file_id);  // 0x20
    UnkAtkArchive* archive;  // 0x8
};

void* fn_71012128f0(UnkAtkLoaderFind_b0268* self, u32 id) {
    UnkAtkArchive* archive = self->archive;
    if (!archive)
        return nullptr;
    u32 file_id;
    switch (id >> 24) {
    case 1: {
        UnkAtkEntryIndex_b0148 entry;
        fn_7101206ad8((UnkAtkArchive_b0148*)archive, &entry, id);
        file_id = entry.index;
        break;
    }
    case 3: {
        u32 out = 0xffffffff;
        fn_7101206e60(archive, &out, id);
        file_id = out;
        break;
    }
    case 5: {
        UnkAtkInfo_b0150 info;
        info.field_0x0 = -1;
        info.flag = 0;
        fn_7101206ed4(archive, id, &info);
        file_id = info.field_0x0;
        break;
    }
    case 6: {
        u64 out = 0xffffffffUL;
        fn_7101206fac(archive, &out, id);
        file_id = (u32)out;
        break;
    }
    default:
        return nullptr;
    }
    if (file_id == 0xffffffff)
        return nullptr;
    return self->Find(file_id);
}

// ---- 710121279c ----
struct UnkAtkHeap_b0268 {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void* Alloc(u64 size) = 0;  // 0x10
};

struct UnkAtkLoaderObject_b0148 {
    virtual void vf_0x0() = 0;
    virtual void vf_0x8() = 0;
    virtual void Register(u32 file_id, void* data) = 0;  // 0x10
    virtual void vf_0x18() = 0;
    virtual void* Find(u32 file_id) = 0;  // 0x20
    UnkAtkArchive* archive;  // 0x8
};

bool fn_7101213100(char* p0);

void fn_710121279c(UnkAtkLoaderObject_b0148* self, u32 id, void* arg) {
    UnkAtkInfo_b0150 info;
    info.field_0x0 = -1;
    info.flag = 0;
    bool found = fn_7101206ed4(self->archive, id, &info);
    if (!info.flag || !found)
        return;
    void* data = self->Find(info.field_0x0);
    UnkAtkWave_b0150 source;
    fn_7101212f94(&source, data, 0);
    if (fn_7101213100((char*)&source))
        return;
    u32 head = fn_7101212f5c((UnkAtkInfo_b0150*)data);
    u8* copy = (u8*)((UnkAtkHeap_b0268*)arg)->Alloc(head + info.count * 4 + 4);
    if (!copy)
        return;
    __builtin_memcpy(copy, data, head);
    *(u32*)(copy + head) = 0x54415746;
    UnkAtkWave_b0150 dest;
    fn_7101212f94(&dest, copy, 1);
    fn_7101213168((UnkAtkInfo_b0150*)&dest);
    for (u32 i = 0; i < info.count; i++)
        fn_7101213294(&dest, i, (void*)fn_71012131a0(&source, i));
    self->Register(info.field_0x0, copy);
}

// ---- 7101238990 ----
struct UnkAtkRing_b0268 {
    s16* buffer;  // 0x0
    u8 pad_0x8[4];
    u32 capacity;  // 0xc
    u32 used;  // 0x10
    u8 pad_0x14[4];
    u32 write_pos;  // 0x18
    u8 pad_0x1c[4];
    u32 frame;  // 0x20
    u32 sub_frame;  // 0x24
};

struct UnkAtkRenderer_b0268 {
    u8 pad_0x0[0x8c4];
    u32 samples_per_frame;  // 0x8c4
};

static inline u32 FramesPerChunk() {
    return fn_7101213d2c((char*)fn_71001039f8());
}

static inline UnkAtkRenderer_b0268* Renderer() {
    return (UnkAtkRenderer_b0268*)fn_71001039f8();
}

u32 fn_7101238990(void* self_, void* samples_, u32 count) {
    UnkAtkRing_b0268* self = (UnkAtkRing_b0268*)self_;
    const s16* samples = (const s16*)samples_;
    if (!count)
        return 0;
    u32 frames = FramesPerChunk();
    u32 used = Renderer()->samples_per_frame * frames + __atomic_load_n(&self->used, __ATOMIC_ACQUIRE);
    u32 free = self->capacity >= used ? self->capacity - used : 0;
    if (free < count)
        count = free;
    if (!count)
        return 0;
    frames = FramesPerChunk();
    u32 per_frame = Renderer()->samples_per_frame;
    u32 chunk = per_frame * frames;
    for (u64 i = 0; i != count; i++) {
        u32 pos = self->write_pos + self->frame + self->sub_frame * frames;
        if (pos >= self->capacity)
            pos -= self->capacity;
        // the sample store may alias the cursor fields, so they are re-read after it
        typedef s16 __attribute__((may_alias)) s16_alias;
        ((s16_alias*)self->buffer)[pos] = samples[i];
        self->sub_frame++;
        if (self->sub_frame < per_frame)
            continue;
        self->frame++;
        self->sub_frame = 0;
        if (self->frame < frames)
            continue;
        u32 next = self->write_pos + chunk;
        if (next >= self->capacity)
            next -= self->capacity;
        self->frame = 0;
        self->write_pos = next;
        __atomic_fetch_add(&self->used, chunk, __ATOMIC_ACQ_REL);
    }
    return count;
}

// ---- 710122f8a8 ----
struct UnkAtkQueuedWave_b0268 {
    const void* buffer;  // 0x0
    size_t size;  // 0x8
    s64 end;  // 0x10
    s64 start;  // 0x18
    const void* context;  // 0x20
    u8 pad_0x28[8];
    bool loop;  // 0x30
    u8 pad_0x31[3];
    u32 state;  // 0x34
    UnkAtkQueuedWave_b0268* next;  // 0x38
};

struct UnkAtkWaveParam_b0268 {
    const void* buffer;  // 0x0
    size_t size;  // 0x8
    s32 start;  // 0x10
    s32 end;  // 0x14
    bool loop;  // 0x18
    bool end_of_stream;  // 0x19
    const void* context;  // 0x20
    size_t context_size;  // 0x28
    u8 pad_0x30[0x10];
};

struct UnkAtkWaveVoice_b0268 {
    u8 pad_0x0[0x20];
    u8 voice[0x8];  // 0x20, nn::audio::VoiceType
    u8 pad_0x28[0xc];
    u8 field_0x34[0x7c];
    s32 mode;  // 0xb0
    u8 pad_0xb4[0x14];
    UnkAtkQueuedWave_b0268* waves;  // 0xc8
    u64 field_0xd0;
    UnkAtkQueuedWave_b0268* current;  // 0xd8
};

static inline nn::audio::VoiceType* Voice(UnkAtkWaveVoice_b0268* self) {
    return reinterpret_cast<nn::audio::VoiceType*>(self->voice);
}

void fn_710122f8a8(UnkAtkWaveVoice_b0268* self, u32 arg) {
    UnkAtkQueuedWave_b0268* head = self->waves;
    if (!head)
        return;
    if (nn::audio::GetVoicePlayState(Voice(self)) == nn::audio::VoiceType::PlayState_Pause) {
        nn::audio::SetVoicePlayState(Voice(self), nn::audio::VoiceType::PlayState_Play);
        return;
    }
    if (head->state == 1) {
        const void* context = nullptr;
        size_t context_size = 0;
        if (self->mode == 2) {
            context = head->context;
            context_size = 6;
        }
        UnkAtkWaveParam_b0268 buffer;
        buffer.buffer = head->buffer;
        buffer.size = head->size;
        buffer.start = head->start;
        buffer.end = head->end;
        bool loop = head->loop;
        buffer.context = context;
        buffer.context_size = context_size;
        buffer.end_of_stream = true;
        buffer.loop = loop;
        nn::audio::AppendWaveBuffer(Voice(self), (const nn::audio::WaveBuffer*)&buffer);
        self->current = head;
        nn::audio::SetVoicePlayState(Voice(self), nn::audio::VoiceType::PlayState_Play);
        fn_710122faa0((UnkAtkVoice*)self, self->field_0x34, arg);
        head->state = 2;
    } else if (head->state == 2) {
        for (UnkAtkQueuedWave_b0268* wave = self->waves; wave; wave = wave->next)
            wave->state = 3;
        self->field_0xd0 = 0;
        self->current = nullptr;
        self->waves = nullptr;
    }
}
