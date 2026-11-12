// nn::atk-ish unit 2772: voice pool release, loader helper, object creation, wave queue refill.
// Names and layouts are placeholders.
#include "game/UnkTU2772.h"
#include "game/UnkTU99.h"
#include "types.h"

struct UnkAtkVoiceItem_b0149 {
    u8 pad_0x0[0x20];
    u8 out[0x8];  // 0x20
    u8 field_0x28;
    u8 field_0x29;
    u8 pad_0x2a[0x7e];
    u32 field_0xa8;
    u8 pad_0xac[0x1c];
    struct UnkAtkWave_b0149* waves;  // 0xc8
    u64 pad_0xd0[2];
    u64 field_0xe0;
};
struct UnkAtkWave_b0149 {
    u8 pad_0x0[0x34];
    u32 state;  // 0x34
    UnkAtkWave_b0149* next;  // 0x38
};
struct UnkAtkVoicePool_b0149 {
    UnkAtkVoiceItem_b0149* items;  // 0x0
    UnkAtkVoiceItem_b0149** slots;  // 0x8
    s32 count;  // 0x10
    s32* index;  // 0x18
};
void fn_710123a680(void* sys, void* out);

void fn_7101230180(UnkAtkVoicePool_b0149* self, UnkAtkVoiceItem_b0149* item) {
    for (UnkAtkWave_b0149* w = item->waves; w; w = w->next)
        w->state = 3;
    item->pad_0xd0[0] = 0;
    item->pad_0xd0[1] = 0;
    item->waves = nullptr;
    if (item->field_0x29) {
        fn_710123a680((u8*)fn_71001039f8() + 0x18, item->out);
        item->field_0x29 = 0;
    }
    item->field_0xe0 = 0;
    item->field_0x28 = 0;
    item->field_0xa8 = 1;
    u64 base = (u64)self->items;
    u64 idx = ((u64)item - base) >> 8;
    s64 slot = self->index[idx];
    s32 last = self->count - 1;
    self->count = last;
    UnkAtkVoiceItem_b0149* moved = self->slots[last];
    __builtin_memcpy(&self->slots[slot], &moved, 8);
    __builtin_memcpy(&self->slots[last], &item, 8);
    s32 slot32 = slot;
    __builtin_memcpy(&self->index[((u64)moved - base) >> 8], &slot32, 4);
    s32 none = -1;
    __builtin_memcpy(&self->index[idx], &none, 4);
}

struct UnkAtkLoader;
struct UnkAtkArchive;
bool fn_7101206d0c(UnkAtkArchive* archive, u32 id, u32* out);

struct UnkAtkLoaderVt_b0149 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void* vf_0x18();
    virtual void* vf_0x20(u32 file);
    UnkAtkArchive* archive;  // 0x8
};

bool fn_71012117ac(UnkAtkLoaderVt_b0149* self, u32 id, void* heap, u32 flags, void* arg) {
    u32 file = fn_71012069f8(self->archive, id);
    if (flags & 2) {
        if (!fn_7101211d30((UnkAtkLoader*)self, file, heap, arg, 0))
            return false;
    }
    if (flags & 8) {
        if (!self->vf_0x20(file))
            return false;
        alignas(8) u32 out;
        if (!fn_7101206d0c(self->archive, id, &out))
            return false;
        u32 next = out;
        if (!fn_7101211d30((UnkAtkLoader*)self, fn_71012069f8(self->archive, next), heap, arg, 1))
            return false;
    }
    return true;
}


struct UnkAtkLink_b0149 {
    UnkAtkLink_b0149* prev;
    UnkAtkLink_b0149* next;
};
struct UnkAtkChain_b0149 {
    u8 pad_0x0[0x18];
    UnkAtkLink_b0149 link;  // 0x18
    u8 pad_0x28[8];
    void* field_0x30;
    void* field_0x38;
    void* field_0x40;
    u8 field_0x48[8];
};
struct UnkAtkObj_b0149 {
    u8 pad_0x0[0x130];
    void* field_0x130;
    u8 pad_0x138[0x2a0];
    UnkAtkLink_b0149 link;  // 0x3d8
};
void* fn_710122ade8(void* pool);
void fn_710122b710(void* item);
void fn_710120f188(void* a, void* b, void* c, void* d);

UnkAtkObj_b0149* fn_710122ca14(UnkAtkChain_b0149* self) {
    UnkAtkObj_b0149* obj = (UnkAtkObj_b0149*)fn_710122ade8(self);
    if (!obj)
        return nullptr;
    fn_710122b710(obj);
    u64* a = nullptr;
    if (self->field_0x30) {
        a = (u64*)fn_710122ade8(self->field_0x30);
        if (!a) {
            fn_710122b868(obj);
            fn_710122ae00(self, obj);
            return nullptr;
        }
        a[1] = 0;
        a[2] = 0;
        a[0] = 0;
        void* b = fn_710122ade8(self->field_0x38);
        if (!b) {
            fn_710122b868(obj);
            fn_710122ae00(self, obj);
            fn_710122ae00(self->field_0x30, a);
            return nullptr;
        }
        fn_710120f188(a, b, self->field_0x40, self->field_0x48);
        obj->field_0x130 = a;
    }
    UnkAtkLink_b0149* node = &self->link;
    UnkAtkLink_b0149* head = &obj->link;
    UnkAtkLink_b0149* old = head->prev;
    head->prev = node->prev;
    old->next = node;
    node->prev->next = head;
    node->prev = old;
    return obj;
}

struct UnkAtkWaveNode_b0149 {
    u64 field_0x0;
    u64 field_0x8;
    u64 field_0x10;
    u64 field_0x18;
    u64 field_0x20;
    u8 pad_0x28[8];
    u8 loop;  // 0x30
    u8 pad_0x31[3];
    u32 state;  // 0x34
    UnkAtkWaveNode_b0149* next;  // 0x38
};
struct UnkAtkWaveParam_b0149 {
    u64 buffer;
    u64 size;
    u32 start;
    u32 end;
    u8 loop;
    u8 end_of_stream;
    u8 pad_0x1a[6];
    u64 context;
    u64 context_size;
    u64 pad_0x30[2];
};
struct UnkAtkWaveVoice_b0149 {
    u8 pad_0x0[0x20];
    u8 out[0x90];  // 0x20
    u32 mode;  // 0xb0
    u8 pad_0xb4[0x14];
    UnkAtkWaveNode_b0149* head;  // 0xc8
    UnkAtkWaveNode_b0149* field_0xd0;
    UnkAtkWaveNode_b0149* cursor;  // 0xd8
};
void* fn_710123a710(void* out);
bool fn_710123a6e0(void* out, UnkAtkWaveParam_b0149* param);
void fn_710123a6d0(void* out, u32 state);

void fn_710122fce4(UnkAtkWaveVoice_b0149* self) {
    UnkAtkWaveNode_b0149* head = self->head;
    if (!head)
        return;
    void* out = self->out;
    if (fn_710123a710(out)) {
        self->head = head->next;
        u32 released = 3;
        __builtin_memcpy(&head->state, &released, 4);
        if (self->head) {
            self->head->state = 2;
        } else {
            self->field_0xd0 = nullptr;
            fn_710123a6d0(out, 1);
        }
    }
    UnkAtkWaveNode_b0149* wave = self->cursor->next;
    if (!wave)
        return;
    u64 context = 0;
    u64 context_size = 0;
    if (self->mode == 2) {
        context = wave->field_0x20;
        context_size = 6;
    }
    UnkAtkWaveParam_b0149 param;
    param.buffer = wave->field_0x0;
    param.size = wave->field_0x8;
    param.start = wave->field_0x18;
    param.end = wave->field_0x10;
    param.loop = wave->loop;
    param.context = context;
    param.context_size = context_size;
    param.end_of_stream = 1;
    if (fn_710123a6e0(out, &param))
        self->cursor = self->cursor->next;
}
