// nn::atk-ish unit 2772: a name lookup through a bit-indexed search tree in a sound-archive table, and the
// per-channel mix refresh of a voice array. Names and layouts are placeholders.
#include <string.h>

#include "game/UnkTU99.h"
#include "types.h"

struct UnkAtkNode_o0319 {
    u8 flags;       // 0x0
    u8 pad_0x1;
    u16 bit;        // 0x2
    u32 left;       // 0x4
    u32 right;      // 0x8
    s32 str;        // 0xc
    u32 value;      // 0x10
};

struct UnkAtkTree_o0319 {
    u32 root;
    u32 count;
    UnkAtkNode_o0319 nodes[1];
};

struct UnkAtkStrEntry_o0319 {
    u32 pad_0x0[2];
    s32 off;
};

struct UnkAtkTable_o0319 {
    s32 pad_0x0;
    s32 str_off;    // 0x4
    s32 pad_0x8;
    s32 tree1_off;  // 0xc
};

s32 fn_710120fb18(UnkAtkTable_o0319* self, s32 idx, const char* name) {
    UnkAtkTree_o0319* t;
    if (idx > 1)
        t = nullptr;
    else
        t = (UnkAtkTree_o0319*)((u8*)self + *(s32*)((u8*)self + ((u64)(u32)idx << 3) + 4));
    u64 n = t->root;
    if ((u32)n >= t->count)
        return -1;
    if (!(t->nodes[n].flags & 1)) {
        s32 len = strlen(name);
        while (true) {
            u32 bit = t->nodes[n].bit;
            s32 byte = bit >> 3;
            if (byte >= len || !((u8)name[byte] & (1 << (~bit & 7))))
                n = t->nodes[n].left;
            else
                n = t->nodes[n].right;
            if (t->nodes[n].flags & 1)
                break;
        }
    }
    s32 si = t->nodes[n].str;
    const char* key = nullptr;
    if (si != -1) {
        u8* base = (u8*)self + ((s32*)self)[1];
        if (base != nullptr)
            key = (const char*)(base + ((s32*)(base + si * 12))[2]);
    }
    if (strcmp(name, key) != 0)
        return -1;
    return t->nodes[n].value;
}


struct UnkAtkMixOut_o0319 {
    u8 bytes[0x10];
};
struct UnkAtkMixSource_o0319;
struct UnkAtkMixSourceVtable_o0319 {
    void* slots[2];
    void (*get)(UnkAtkMixSource_o0319* self, UnkAtkMixOut_o0319* out, u32 mode);  // 0x10
};
struct UnkAtkMixSource_o0319 {
    UnkAtkMixSourceVtable_o0319* vtable;
};
struct UnkAtkMixTable_o0319 {
    u8 pad_0x0[0x110];
    UnkAtkMixSource_o0319* sources[8];  // 0x110
};
struct UnkAtkChannelArray_o0319 {
    u8 pad_0x0[0x1c4];
    s32 count;  // 0x1c4
    u8 pad_0x1c8[0x1ea - 0x1c8];
    u8 mode;  // 0x1ea
    u8 pad_0x1eb[0x200 - 0x1eb];
    f32 level;  // 0x200
};
void fn_7101230494(void* element, u32 enabled, UnkAtkMixOut_o0319* params);

void fn_710122d314(UnkAtkChannelArray_o0319* self) {
    for (s64 i = 0; i < self->count; i++) {
        void* element = reinterpret_cast<u8*>(self) + i * 0xe0;
        UnkAtkMixTable_o0319* table = static_cast<UnkAtkMixTable_o0319*>(fn_71001039f8());
        u64 mode = self->mode;
        UnkAtkMixSource_o0319* source = table->sources[mode];
        if (source && !(self->level <= 0.0f)) {
            UnkAtkMixOut_o0319 out;
            source->vtable->get(source, &out, (u32)mode);
            fn_7101230494(element, 1, &out);
        } else {
            fn_7101230494(element, 0, nullptr);
        }
    }
}
