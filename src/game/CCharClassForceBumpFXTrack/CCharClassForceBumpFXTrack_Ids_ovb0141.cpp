// CCharClassForceBumpFXTrack's unit (tools/tu_map.py): sets the string id at +0x18 and refills the id list at
// +0x20 from the comma-separated pieces of the new id's text (the tokenizer fn_71000fc570 as in
// src/base/UnkTunableRegistry.cpp), unless the id is the empty one. Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkIdToken_b0141 {
    u8 text[0x148];
    UnkIdToken_b0141* next;  // 0x148
};

struct UnkIdTokens_b0141 {
    char* vtable;
    u32 count;
    UnkIdToken_b0141* head;
    UnkIdToken_b0141* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    char* pool_vtable;  // 0x30
    base::UnkAllocator* pool_parent;
    UnkBlockFreeList pool_blocks;
    u32 field_0x68;
    bool field_0x6c;
};
void fn_71000fc570(const char* text, const char* separator, UnkIdTokens_b0141* out);
void fn_71000fc7cc(UnkIdTokens_b0141* self);

union UnkIdSlot_b0141 {
    CStrId id;
    UnkIdSlot_b0141() {}
    ~UnkIdSlot_b0141() {}
};

struct UnkBumpTrack_b0141 {
    u8 pad_0x0[0x18];
    CStrId id;  // 0x18
    base::UnkStrIdVec ids;  // 0x20
};

static inline void PushBackMove(base::UnkStrIdVec* v, CStrId& id) {
    u32 n = v->count;
    u32 c = v->capacity;
    if (c <= n && v->step != 0)
        c = (n / v->step + 1) * v->step;
    if ((s32)n >= (s32)c)
        for (;;) {
        }
    v->Reserve(c);
    __builtin_memcpy(&v->data[v->count], &id.data, 8);
    id.data = &g_unk_7101d47108->empty;
    ++v->count;
}

void fn_7100c1dc78(UnkBumpTrack_b0141* self, const CStrId* src) {
    self->id = *src;
    for (u32 i = 0; i < self->ids.count; ++i)
        self->ids.data[i].Reset();
    self->ids.count = 0;
    if (src->data == &g_unk_7101d47108->empty)
        return;
    UnkIdTokens_b0141 tokens;
    tokens.count = 0;
    tokens.head = nullptr;
    tokens.tail = nullptr;
    tokens.allocator = fn_7100083990();
    tokens.flags = 0x13;
    tokens.vtable = reinterpret_cast<char*>(g_unk_7101a1bcd0 + 2);
    tokens.pool_vtable = g_unk_7101a1a4c8 + 0x10;
    tokens.pool_parent = nullptr;
    fn_710008377c(&tokens.pool_blocks);
    tokens.field_0x68 = 0;
    tokens.field_0x6c = false;
    // A store through char: the original reads the id's data pointer again after it.
    char* text = reinterpret_cast<char*>(src->data);
    u32 uses;
    __builtin_memcpy(&uses, text + 0x44, 4);
    ++uses;
    __builtin_memcpy(text + 0x44, &uses, 4);
    fn_71000fc570(*reinterpret_cast<const char* const*>(reinterpret_cast<const char*>(src->data) + 0x10), ",", &tokens);
    for (UnkIdToken_b0141* t = tokens.head; t; t = t->next) {
        UnkIdSlot_b0141 slot;
        fn_7100000850(&slot.id, *reinterpret_cast<const CRntString*>(t), true);
        PushBackMove(&self->ids, slot.id);
        slot.id.~CStrId();
    }
    fn_71000fc7cc(&tokens);
}
