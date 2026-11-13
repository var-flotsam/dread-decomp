// CActionSet's unit (unit 1729): the action set's constructor and destructor (a string id at +0x58
// and a chained hash map of actions at +0x68 with its node pool) and its file read (opens the set's
// path "rb", skips 8 bytes, reads the id at +0x58, a word, and the action count, sizes the map and
// reads each action with fn_7100b8c420). Names and layouts are placeholders.
#include <string.h>

#include "base/UnkBlockFreeList.h"
#include "base/UnkDestructors.h"
#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "types.h"
#include "base/UnkStrPair.h"

using base::global::CStrId;

extern void* g_unk_7101a9ff78[];

struct UnkActionBlockPool_b0104 {
    char* vtable;  // 0x0
    base::UnkAllocator* parent;  // 0x8
    UnkBlockFreeList blocks;  // 0x10
    u32 field_0x38;
    bool field_0x3c;
};

struct UnkActionBuckets_b0104 {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;
};

struct UnkActionSetCtor_b0104 {
    void** vtable;
    u8 base_0x8[0x50];
    CStrId name;  // 0x58
    u32 field_0x60;
    base::UnkAllocator* allocator;  // 0x68
    u32 flags;  // 0x70
    u32 count;  // 0x74
    UnkActionBuckets_b0104 buckets;  // 0x78
    void* last;  // 0xa0
    void* first;  // 0xa8
    UnkActionBlockPool_b0104 pool;  // 0xb0
};
static_assert(__builtin_offsetof(UnkActionSetCtor_b0104, pool) == 0xb0, "layout");

void fn_7100b8be4c(UnkActionSetCtor_b0104* self) {
    fn_7100016ad8(self);
    self->vtable = &g_unk_7101a9ff78[2];
    self->name.data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    self->name.data = &g_unk_7101d47108->empty;
    self->field_0x60 = 0;
    self->allocator = fn_7100083990();
    self->flags = 0x58;
    self->count = 0;
    self->buckets.data = nullptr;
    self->buckets.count = 0;
    self->buckets.capacity = 0;
    self->buckets.step = 10;
    self->buckets.field_0x14 = true;
    self->buckets.flags = 0x58;
    self->buckets.allocator = fn_7100083990();
    self->last = nullptr;
    self->first = nullptr;
    self->pool.vtable = g_unk_7101a1a4c8 + 0x10;
    self->pool.parent = nullptr;
    fn_710008377c(&self->pool.blocks);
    self->pool.field_0x38 = 0;
    self->pool.field_0x3c = false;
}

extern void* g_unk_7101a9ff78[];

struct UnkActionNode_b0104 {
    u64 key[2];
    u8 value[0x1c0];  // 0x10
    UnkActionNode_b0104* next;  // 0x1d0
};

void fn_7100b8a7d0(void* value);

// The action map: a chained hash map (base/UnkHashMap.h's layout).
struct UnkActionMap_b0104 {
    base::UnkAllocator* allocator;  // 0x0
    u32 field_0x8;
    u32 count;  // 0xc
    void* buckets;  // 0x10
    u32 field_0x18;
    u32 bucket_capacity;  // 0x1c
    u8 pad_0x20[0x18];
    UnkActionNode_b0104* last;  // 0x38
    UnkActionNode_b0104* first;  // 0x40
};

void fn_7100b8c04c(UnkActionMap_b0104* self);

struct UnkActionSetDtor_b0104 {
    void** vtable;
    u8 base_0x8[0x50];
    base::UnkStrIdMember name;  // 0x58
    u8 pad_0x60[0x8];
    UnkActionMap_b0104 actions;  // 0x68

    __attribute__((always_inline)) ~UnkActionSetDtor_b0104() {
        vtable = &g_unk_7101a9ff78[2];
        if (actions.count) {
            UnkActionNode_b0104* node = actions.first;
            while (node) {
                UnkActionNode_b0104* next = node->next;
                fn_7100b8a7d0(node->value);
                actions.allocator->vf_0x48(node);
                node = next;
            }
            memset(actions.buckets, 0, actions.bucket_capacity * u64(8));
            actions.count = 0;
            actions.last = nullptr;
            actions.first = nullptr;
        }
        fn_7100b8c04c(reinterpret_cast<UnkActionMap_b0104*>(reinterpret_cast<u8*>(this) + 0x68));
    }
};
static_assert(__builtin_offsetof(UnkActionSetDtor_b0104, actions) == 0x68, "layout");

void fn_7100b8bf6c(UnkActionSetDtor_b0104* self) {
    self->~UnkActionSetDtor_b0104();
    fn_7100016f88(self);
}

using base::global::CRntFile;

struct UnkFileReader;
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);

struct UnkActionSet_b0104 {
    u8 pad_0x0[0x10];
    base::global::CFilePathStrId path;  // 0x10
    u8 pad_0x18[0x40];
    u8 field_0x58[0x8];
    u32 field_0x60;
    u8 pad_0x64[0x4];
    char actions[0x80];  // 0x68: a hash map
};

u64 fn_7100b8a428(CRntFile* file, void* out);
void fn_7100b8c2cc(char* map, u32 buckets, u32 nodes);
void fn_7100b8c420(UnkActionSet_b0104* self, CRntFile* file);

bool fn_7100b8c1a0(UnkActionSet_b0104* self) {
    CRntFile file;
    fn_710007be8c(&file, 0x16, nullptr);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d2ec(&s, &self->path);
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    bool ok;
    if (opened) {
        u32 value;
        s32 count;
        fn_710007c00c(&file, 8, 0);
        fn_7100b8a428(&file, self->field_0x58);
        value = 0;
        fn_710007c714(&file, &value, 4, 1);
        self->field_0x60 = value;
        count = 0;
        fn_710007c714(&file, &count, 4, 1);
        fn_7100b8c2cc(self->actions, count > 4 ? static_cast<u32>(count) / 4 + 1 : 11, count);
        for (s32 i = 0; i < count; ++i)
            fn_7100b8c420(self, &file);
        ok = true;
    } else {
        ok = false;
    }
    fn_710007c0e4(&file);
    return ok;
}
