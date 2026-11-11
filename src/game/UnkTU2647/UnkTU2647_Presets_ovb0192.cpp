// A unit of tools/tu_map.py (2647, hint sound::CAudioPreset; no reliable name): a bank of sixteen presets, each
// two lists of file-path ids (0x50 bytes), with the bank's string ids, defaults and copy. Names and layouts
// are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/core/CAsset.h"
#include "base/global/CStrId.h"
#include "base/global/CFilePathStrId.h"
#include "game/UnkTU2647.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkPresetList_b0192 {
    CFilePathStrId* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    void Init() {
        data = nullptr;
        count = 0;
        capacity = 0;
        step = 10;
        field_0x14 = true;
        flags = 0xf;
        allocator = fn_7100083990();
    }
    // The id's destructor, with the decrement and the test in two steps.
    static void Release(CFilePathStrId& id) {
        base::global::UnkStringPool* pool = g_unk_7101d47200;
        if (pool) {
            if (id.data && id.data != &pool->empty) {
                --id.data->ref_count;
                if (id.data->ref_count == 0) {
                    fn_71000803a4(pool, id.data);
                    pool = g_unk_7101d47200;
                }
            }
            id.data = &pool->empty;
        }
    }
    void Clear() {
        for (u32 i = 0; i < count; ++i)
            Release(data[i]);
        count = 0;
    }
    void Free() {
        if (data) {
            Clear();
            allocator->vf_0x48(data);
            capacity = 0;
        }
    }
    void MoveFrom(UnkPresetList_b0192& other) {
        if (this != &other) {
            Clear();
            allocator = other.allocator;
            flags = other.flags;
            step = other.step;
            field_0x14 = other.field_0x14;
            count = other.count;
            capacity = other.capacity;
            data = other.data;
            other.data = nullptr;
            other.count = 0;
            other.capacity = 0;
        }
    }
};

struct UnkAudioPresetPair_b0192 {
    UnkPresetList_b0192 first;
    UnkPresetList_b0192 second;
    UnkAudioPresetPair_b0192() {
        __builtin_memset(this, 0, sizeof(*this));
        first.Init();
        second.Init();
    }
};

void fn_710103a0e0(UnkAudioPresetPair_b0192* self) {
    new (self) UnkAudioPresetPair_b0192();
}

__attribute__((noinline)) void fn_71010396a8(UnkAudioPresetPair_b0192* self) {
    self->second.Free();
    self->first.Free();
}

__attribute__((noinline)) UnkAudioPresetPair_b0192* fn_710103a414(UnkAudioPresetPair_b0192* self, UnkAudioPresetPair_b0192* other) {
    self->first.MoveFrom(other->first);
    self->second.MoveFrom(other->second);
    return self;
}

struct UnkAudioPresetSet_b0192 {
    UnkAudioPresetPair_b0192 items[16];
};

void fn_710103c1f4(UnkAudioPresetSet_b0192* self) {
    for (u32 i = 0; i < 16; ++i) {
        UnkAudioPresetPair_b0192 empty;
        fn_710103a414(&self->items[i], &empty);
        fn_71010396a8(&empty);
    }
}

struct UnkList_e {
    void* data;
    u32 count;
    u32 capacity;
    u32 step;
    u8 field_0x14;
    u32 flags;
    void* allocator;

    UnkList_e(UnkList_e&& o)
        : data(o.data), count(o.count), capacity(o.capacity), step(o.step), field_0x14(o.field_0x14),
          flags(o.flags), allocator(o.allocator) {
        o.data = nullptr;
        o.count = 0;
        o.capacity = 0;
    }
};

struct UnkListPair_e {
    UnkList_e first;
    UnkList_e second;
    UnkListPair_e(UnkListPair_e&& o) : first(static_cast<UnkList_e&&>(o.first)), second(static_cast<UnkList_e&&>(o.second)) {}
};

struct UnkPresetRef_e {
    union {
        u64 head;
        u64* head_ptr;
    };
    union {
        UnkListPair_e tail;
        UnkListPair_e* tail_ptr;
    };
    u32 state;
};

void fn_710103beec(UnkPresetRef_e* self, UnkPresetRef_e* other) {
    self->state = 0;
    switch (other->state) {
    case 1:
        self->state = other->state;
        self->head = other->head;
        new (&self->tail) UnkListPair_e(static_cast<UnkListPair_e&&>(other->tail));
        break;
    case 2: {
        self->state = 1;
        u64* head = other->head_ptr;
        UnkListPair_e* tail = other->tail_ptr;
        self->head = *head;
        new (&self->tail) UnkListPair_e(static_cast<UnkListPair_e&&>(*tail));
        break;
    }
    }
}

extern char g_unk_7101cac748[];
void fn_7101036efc(void* items);

struct UnkAudioPresetBank_h {
    void* vtable;
    u8 unk_0x8[0x50];
    CStrId id_a;  // 0x58
    CStrId id_b;  // 0x60
    u8 items[0x500];  // 0x68

};

// The id's destructor, with the decrement and the test in two steps.
static inline void Release(CStrId& id) {
    base::global::UnkStringPool* pool = g_unk_7101d47108;
    if (pool) {
        if (id.data && id.data != &pool->empty) {
            --id.data->ref_count;
            if (id.data->ref_count == 0) {
                fn_71000803a4(pool, id.data);
                pool = g_unk_7101d47108;
            }
        }
        id.data = &pool->empty;
    }
}

void fn_7101036e34(UnkAudioPresetBank_h* self) {
    self->vtable = g_unk_7101cac748 + 0x10;
    fn_7101036efc(self->items);
    Release(self->id_b);
    Release(self->id_a);
    fn_7100016f88(self);
}

struct UnkAudioPresetBank_i {
    void* vtable;
    u8 unk_0x8[0x50];
    CStrId id_a;       // 0x58
    CStrId id_b;       // 0x60
    u8 items[0x500];   // 0x68
    u32 words[16];     // 0x568
    u32 word_0x5a8;
    u32 word_0x5ac;
    u32 word_0x5b0;
    u32 word_0x5b4;
    u32 word_0x5b8;
    u32 word_0x5bc;
    u32 word_0x5c0;
    u8 flags[6];       // 0x5c4
};
static_assert(__builtin_offsetof(UnkAudioPresetBank_i, flags) == 0x5c4, "layout");

struct UnkAssetRecordHolder_b45a1;
UnkAssetRecordHolder_b45a1* fn_7100016e14(UnkAssetRecordHolder_b45a1* self, UnkAssetRecordHolder_b45a1* other);
void fn_71010372dc(void* items, const void* other_items);

UnkAudioPresetBank_i* fn_7101037484(UnkAudioPresetBank_i* self, UnkAudioPresetBank_i* other) {
    if (other != self) {
        fn_7100016e14(reinterpret_cast<UnkAssetRecordHolder_b45a1*>(self), reinterpret_cast<UnkAssetRecordHolder_b45a1*>(other));
        self->id_a = other->id_a;
        self->id_b = other->id_b;
        const void* src_items = other->items;
        fn_71010372dc(self->items, src_items);
        for (u32 i = 0; i < 16; ++i)
            self->words[i] = other->words[i];
        self->word_0x5ac = other->word_0x5ac;
        self->word_0x5a8 = other->word_0x5a8;
        self->word_0x5b0 = other->word_0x5b0;
        self->word_0x5b4 = other->word_0x5b4;
        self->word_0x5b8 = other->word_0x5b8;
        self->word_0x5bc = other->word_0x5bc;
        for (u32 i = 0; i < 6; ++i)
            self->flags[i] = other->flags[i];
    }
    return self;
}
