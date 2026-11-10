// base::animation::CAnimationBlendSkeletonTransformer's unit (tools/tu_map.py unit 10):
// - fn_7100003448: a blend state record's constructor (weights 0/1, three empty arrays, 0x3f).
// - fn_7100003344: frees the owned buffers of the 0x68-byte entries at +0x48, clears every node
//   (+0x38) that points into those entries, then frees the entry array.
// - fn_7100003afc: the blend step: brings the target pose (+0x60) up to date with the source
//   (+0x28), applies a pending pose (+0x100, the shared pose at +0xd0, fn_7100005ebc) to the blend
//   table (+0x98, a keyed table of 0x40-byte entries), runs the base transform (fn_7100005e4c),
//   then blends the table's entries into the output's matrices by the weight at +0xe4 (only the
//   bones the mask at +0xf0 selects, when +0xf8 says to) and keeps the output as the target pose.
// - fn_71000040e4: the transformer's copy assignment (the layout of UnkBlendTransformerCopy_b59.cpp):
//   the vtable is left alone, the members at +0x28, +0x60 and +0x98 are assigned out of line
//   (fn_7100014e5c), the words at +0xf0 and +0xf8 one at a time.
// Names and layouts are placeholders.
#include <memory>
#include <new>

#include "base/UnkMatrix44.h"
#include "types.h"

void fn_7100002cd0(void* p);
void fn_7100014e5c(void* self, const void* other);

struct UnkBlendArray_ovb0003 {
    u32 count;
    u32 capacity;
    void* data;

    UnkBlendArray_ovb0003() : count(0), capacity(0), data(nullptr) {}
    ~UnkBlendArray_ovb0003() {
        if (data)
            fn_7100002cd0(data);
    }
};

struct UnkBlendPair_ovb0003 {
    u32 id;
    f32 weight;
};

struct UnkBlendRange_ovb0003 {
    f32 min;
    f32 max;
};

struct UnkBlendState_ovb0003 {
    UnkBlendPair_ovb0003 field_0x0;
    UnkBlendRange_ovb0003 field_0x8;
    UnkBlendArray_ovb0003 array_0x10;
    UnkBlendArray_ovb0003 array_0x20;
    UnkBlendArray_ovb0003 array_0x30;
    u64 field_0x40;
    u32 field_0x48;
    u16 field_0x4c;
    u8 field_0x4e[0x1e];

    UnkBlendState_ovb0003() {
        field_0x4c = 0x3f;
        field_0x40 = 0;
        field_0x48 = 0;
        for (u8& b : field_0x4e)
            b = 0;
        field_0x8 = {0.0f, 1.0f};
        field_0x0 = {0, 1.0f};
    }
};

void fn_7100003448(UnkBlendState_ovb0003* self) { new (self) UnkBlendState_ovb0003(); }

struct UnkBlendEntry_ovb0003 {
    void* buffer;
    u8 pad_0x8[4];
    bool owned;  // 0xc
    u8 pad_0xd[0x5b];
};
static_assert(sizeof(UnkBlendEntry_ovb0003) == 0x68, "UnkBlendEntry_ovb0003 layout");

struct UnkBlendNode_ovb0003 {
    u8 pad_0x0[0x4e];
    u8 field_0x4e;
    u8 field_0x4f;
    UnkBlendEntry_ovb0003* entry;  // 0x50
};

template <typename T>
struct UnkBlendList_ovb0003 {
    s32 count;
    s32 capacity;
    T* data;

    void Clear() {
        if (data) {
            count = 0;
            capacity = 0;
            fn_7100002cd0(data);
            data = nullptr;
        }
    }
};

struct UnkBlendOwner_ovb0003 {
    u8 pad_0x0[0x30];
    UnkBlendList_ovb0003<UnkBlendNode_ovb0003*> nodes;  // 0x30
    UnkBlendList_ovb0003<UnkBlendEntry_ovb0003> entries;  // 0x40
};

// Stores through a plain pointer (no member access path): the original reloads the arrays after them.
template <class T>
static inline void ClearPointer(T** slot) {
    *slot = nullptr;
}

void fn_7100003344(UnkBlendOwner_ovb0003* self) {
    for (s32 i = 0; i < self->entries.count; ++i) {
        if (self->entries.data[i].buffer && self->entries.data[i].owned) {
            fn_7100002cd0(self->entries.data[i].buffer);
            ClearPointer(&self->entries.data[i].buffer);
        }
    }
    for (s32 i = 0; i < self->nodes.count; ++i) {
        UnkBlendNode_ovb0003* node = self->nodes.data[i];
        if (node->entry >= self->entries.data && node->entry < self->entries.data + self->entries.count) {
            ClearPointer(&node->entry);
            __builtin_memset(&self->nodes.data[i]->field_0x4e, 0, 2);
        }
    }
    self->entries.Clear();
}

struct UnkKeyedEntry {
    u8 data[0x40];
};

struct UnkKeyedTable {
    void* vtable;
    UnkKeyedEntry* entries;  // 0x8
    void** keys;             // 0x10
    u32 count;               // 0x18
    u8 pad_0x1c[0x14];
    UnkKeyedEntry* cursor;   // 0x30
};

struct UnkU64Array_b47;

struct UnkBlendSkeleton_ovb0003 {
    u8 pad_0x0[0x80];
    UnkU64Array_b47* bones;  // 0x80
};

bool fn_7100015570(UnkKeyedTable* self, void** key, UnkKeyedEntry** entry);
UnkMatrix44* fn_7100015508(void* matrices, u64 id);
s32 fn_710019ba50(UnkU64Array_b47* self, const u64* key);
void fn_7100015094(void* self, const void* other);
void fn_7100015124(void* self, const void* other);
UnkMatrix44 fn_7100078834(const UnkKeyedEntry* entry, const UnkMatrix44* matrix, f32 weight);

struct UnkBlendTransformer_ovb0003 {
    char* vtable;
    std::shared_ptr<void> shared;
    UnkBlendSkeleton_ovb0003* skeleton;  // 0x18
    u8 pad_0x20[0x8];
    UnkKeyedTable source;  // 0x28
    UnkKeyedTable target;  // 0x60
    UnkKeyedTable table;   // 0x98
    std::shared_ptr<void> pending;  // 0xd0
    f32 field_0xe0;
    f32 weight;  // 0xe4
    u8 pad_0xe8[0x8];
    u8* mask;  // 0xf0
    void* use_mask;  // 0xf8
    bool has_pending;  // 0x100
};
static_assert(__builtin_offsetof(UnkBlendTransformer_ovb0003, has_pending) == 0x100, "layout");

void fn_7100005ebc(UnkBlendTransformer_ovb0003* self, void* pose, UnkKeyedTable* table);
void fn_7100005e4c(UnkBlendTransformer_ovb0003* self, void* out);

void fn_7100003afc(UnkBlendTransformer_ovb0003* self, void* out) {
    if (self->source.count != 0 && self->target.count != self->source.count)
        fn_7100014e5c(&self->target, &self->source);
    if (self->has_pending) {
        self->has_pending = false;
        if (self->pending) {
            if (self->table.count == 0)
                fn_7100015094(&self->table, &self->target);
            fn_7100005ebc(self, self->pending.get(), &self->table);
            self->pending.reset();
        } else {
            const UnkKeyedTable* target = &self->target;
            fn_7100014e5c(&self->table, target);
        }
    }
    fn_7100005e4c(self, out);
    if (self->weight == 0.0f) {
        fn_7100015124(out, &self->table);
    } else if (self->weight < 1.0f) {
        UnkU64Array_b47* bones = self->skeleton->bones;
        void* key = reinterpret_cast<void*>(~0ull);
        UnkKeyedEntry* entry = nullptr;
        self->table.cursor = self->table.entries;
        while (fn_7100015570(&self->table, &key, &entry)) {
            UnkMatrix44* m = fn_7100015508(out, reinterpret_cast<u64>(key));
            if (!m)
                continue;
            if (self->use_mask) {
                s32 bone = fn_710019ba50(bones, reinterpret_cast<u64*>(&key));
                if (self->mask[bone] != 1)
                    continue;
            }
            UnkCopyMatrix(m, fn_7100078834(entry, m, self->weight));
        }
        self->table.cursor = self->table.entries + self->table.count;
    }
    if (self->source.count != 0)
        fn_7100015124(&self->target, out);
    else
        fn_7100014e5c(&self->target, out);
}

struct UnkBlendMember_ovb0003 {
    alignas(8) u8 data[0x38];

    UnkBlendMember_ovb0003& operator=(const UnkBlendMember_ovb0003& other) {
        fn_7100014e5c(this, &other);
        return *this;
    }
};

struct UnkBlendSkeletonBase_ovb0003 {
    char* vtable;

    UnkBlendSkeletonBase_ovb0003& operator=(const UnkBlendSkeletonBase_ovb0003&) { return *this; }
};

struct UnkBlendSkeletonData_ovb0003 : UnkBlendSkeletonBase_ovb0003 {
    std::shared_ptr<void> shared;
    u64 field_0x18;
    u16 field_0x20;
    u8 field_0x22;
    UnkBlendMember_ovb0003 member_0x28;

    __attribute__((always_inline)) UnkBlendSkeletonData_ovb0003& operator=(const UnkBlendSkeletonData_ovb0003&) = default;
};

struct UnkBlendWords_ovb0003 {
    u64 field_0x0;
    u64 field_0x8;
};

// A word assigned on its own (not part of the neighbouring fields' block copy).
struct UnkBlendWord_ovb0003 {
    u64 value;

    UnkBlendWord_ovb0003& operator=(const UnkBlendWord_ovb0003& other) {
        value = other.value;
        return *this;
    }
};

struct UnkBlendTransformerAssign_ovb0003 : UnkBlendSkeletonData_ovb0003 {
    UnkBlendMember_ovb0003 member_0x60;
    UnkBlendMember_ovb0003 member_0x98;
    std::shared_ptr<void> shared_0xd0;
    UnkBlendWords_ovb0003 field_0xe0;
    UnkBlendWord_ovb0003 field_0xf0;
    UnkBlendWord_ovb0003 field_0xf8;
    u8 field_0x100;

    __attribute__((always_inline)) UnkBlendTransformerAssign_ovb0003& operator=(const UnkBlendTransformerAssign_ovb0003&) = default;
};
static_assert(sizeof(UnkBlendTransformerAssign_ovb0003) == 0x108, "UnkBlendTransformerAssign_ovb0003 layout");

UnkBlendTransformerAssign_ovb0003* fn_71000040e4(UnkBlendTransformerAssign_ovb0003* self, const UnkBlendTransformerAssign_ovb0003* other) {
    return &(*self = *other);
}
