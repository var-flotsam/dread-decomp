// Unit 218 of tools/tu_map.py (the slot table behind game/UnkSlots.h and the bones' 0x98-byte records keyed by
// 8-byte keys, src/game/UnkUnit222/UnkUnit222.cpp): the per-frame step of the slot table's objects, giving every
// bone record its counterpart, and erasing a node of the string-id keyed map. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

extern "C" [[noreturn]] void __assert_fail(const char* expr, const char* file, int line, const char* func);

extern u32 g_unk_7101d58e48;  // the current frame

struct UnkBoneArray;
void fn_710019b9c8(UnkBoneArray* bones);

struct UnkSlotObject_ovb0215 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(f32 dt);
};

struct UnkSlotsInfo_ovb0176 {
    u8 pad_0x0[0x30];
    u32 count;  // 0x30
    u8 pad_0x34[0x1c];
    UnkSlotObject_ovb0215** objects;  // 0x50
    u8 pad_0x58[0x28];
    UnkBoneArray* bones;  // 0x80
    u8 pad_0x88[0x48];
    u32 frame;  // 0xd0
};
static_assert(__builtin_offsetof(UnkSlotsInfo_ovb0176, frame) == 0xd0, "layout");

void fn_7100199370(UnkSlotsInfo_ovb0176* info, f32 dt) {
    fn_710019b9c8(info->bones);
    if (dt > 0.001f) {
        u32 frame = g_unk_7101d58e48;
        if (info->frame != frame) {
            info->frame = frame;
            for (u64 i = 0; i < info->count; ++i) {
                if (UnkSlotObject_ovb0215* object = info->objects[i])
                    object->vf_0x10(dt);
            }
        }
    }
}

struct UnkRecord_0x40 {
    u8 data[0x40];
};
struct UnkRecordsByKeyHolder_7100015250;
UnkRecord_0x40* fn_7100015250(UnkRecordsByKeyHolder_7100015250* self, u64 key);

struct UnkBoneRef_ovb0215 {
    u8 pad_0x0[0x30];
    u64 key;  // 0x30
};

struct UnkBoneRecord_ovb0215 {
    u8 pad_0x0[8];
    void* field_0x8;
    u8 pad_0x10[0x58];
    UnkBoneRef_ovb0215* ref;  // 0x68
    u8 pad_0x70[0x28];
};
static_assert(sizeof(UnkBoneRecord_ovb0215) == 0x98, "layout");

struct UnkBoneRecords_ovb0215 {
    u8 pad_0x0[8];
    UnkBoneRecord_ovb0215* values;  // 0x8
    u64* keys;                      // 0x10
    u32 count;                      // 0x18

    UnkBoneRecord_ovb0215* End() { return &values[count]; }
    UnkBoneRecord_ovb0215* Find(u64 key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return End();
    }
};

struct UnkBoneHost_ovb0215 {
    u8 pad_0x0[0x80];
    UnkBoneRecords_ovb0215* records;  // 0x80
};

void fn_71001952fc(UnkBoneRecord_ovb0215* record, void* other);

void fn_710019927c(UnkBoneHost_ovb0215* self, UnkRecordsByKeyHolder_7100015250* holder) {
    u64 n = self->records->count;
    for (u64 i = 0; i < n; ++i) {
        UnkBoneRecord_ovb0215* record = &self->records->values[i];
        void* other = fn_7100015250(holder, record->ref->key);
        if (!other) {
            UnkBoneRecords_ovb0215* records = self->records;
            UnkBoneRecord_ovb0215* found = records->Find(record->ref->key);
            found = found == records->End() ? nullptr : found;
            other = found->field_0x8;
        }
        fn_71001952fc(record, other);
    }
}

// The string data's hash (+0x30), as the bucket function reads it.
struct UnkStringHashView_ovb0215 {
    u8 pad_0x0[0x30];
    u64 hash;
};

struct UnkIdNode_ovb0215 {
    CStrId key;
    u64 value;
    UnkIdNode_ovb0215* next;   // 0x10: the list of all nodes
    UnkIdNode_ovb0215* prev;   // 0x18
    UnkIdNode_ovb0215* chain;  // 0x20: the next node in the bucket

    UnkIdNode_ovb0215*& Next() { return next; }
    UnkIdNode_ovb0215*& Prev() { return prev; }
};

struct UnkIdMap_ovb0215 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkIdNode_ovb0215** buckets;  // 0x10 (base::global::CRntVector)
    u32 bucket_count;             // 0x18
    u32 bucket_capacity;          // 0x1c
    u8 pad_0x20[0x18];
    UnkIdNode_ovb0215* tail;  // 0x38
    UnkIdNode_ovb0215* head;  // 0x40

    UnkIdNode_ovb0215** At(u32 i) {
        if (!(i < bucket_count))
            __assert_fail("i < m_iSize", "..\\Base/global/RntVector.h", 0x15b, "operator[]");
        return &buckets[i];
    }
};

UnkIdNode_ovb0215* fn_7100199be0(UnkIdMap_ovb0215* self, const CStrId* key) {
    u64 h = reinterpret_cast<const UnkStringHashView_ovb0215*>(key->data)->hash;
    UnkIdNode_ovb0215** bucket = self->At(((u32)(h >> 32) ^ (u32)h) % self->bucket_capacity);
    UnkIdNode_ovb0215* n = *bucket;
    if (!n)
        return nullptr;
    UnkIdNode_ovb0215* before = nullptr;
    while (n->key.data != key->data) {
        before = n;
        n = n->chain;
        if (!n)
            return nullptr;
    }
    asm("" : "+r"(before));  // keeps &before->chain out of the search loop
    *(before ? &before->chain : bucket) = n->chain;
    if (self->head == n)
        self->head = n->next;
    if (self->tail == n)
        self->tail = n->prev;
    if (n->Prev())
        n->Prev()->Next() = n->Next();
    UnkIdNode_ovb0215* r = nullptr;
    if (n->Next()) {
        n->Next()->Prev() = n->Prev();
        r = n->Next();
    }
    n->key.Reset();
    self->allocator->vf_0x48(n);
    --self->count;
    return r;
}
