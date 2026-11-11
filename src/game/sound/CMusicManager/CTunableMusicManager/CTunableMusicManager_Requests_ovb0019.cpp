// sound::CMusicManager::CTunableMusicManager's unit (tools/tu_map.py, unit 2637; the manager's layout
// is game/sound/CMusicManager.h): lookups in the tune table, the request array at +0xb8 (holding,
// enabling and re-sending requests), the slot lists at +0xe0 (erasing and stopping nodes) and the two
// string-id sets at +0x130 and +0x178. Names and layouts are placeholders inferred from the code.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "base/snd/CSoundSystem.h"
#include "game/UnkTU99.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "types.h"

struct UnkSoundMgr;
struct UnkMgr;
struct UnkHandle;
struct UnkArray;
struct UnkRequest;
struct UnkMgr_0xf18;
struct UnkTunable;

extern UnkSoundMgr* g_unk_7101d4d820;
extern UnkTunable g_unk_7101d68580;  // base::snd::CSoundSystemATK's tunable

void fn_7100108a84(UnkMgr*, UnkHandle*, f32);

struct UnkMask {
    u32 bits;
    UnkMask(u32 b) : bits(b) {}
    UnkMask(const UnkMask& o) : bits(o.bits) {}
};

struct UnkKind {
    u32 value;
    UnkKind(u32 v) : value(v) {}
    UnkKind(const UnkKind& o) : value(o.value) {}
};

// The kind is passed by address and not kept (the unit defines it, so the original can tail-call
// after passing it).
void fn_7100fd4544(UnkMgr_0xf18*, u32, UnkRequest*, __attribute__((noescape)) const UnkKind* kind);

static inline const UnkKind* AddressOf(const UnkKind& kind) { return &kind; }

struct UnkTuneRange {
    s32 first;
    s32 second;
};

struct UnkTuneItem_0x18 {
    u64 id;
    UnkTuneRange range;  // 0x8
    u64 field_0x10;
};

struct UnkTuneEntry_0x50 {
    u8 pad_0x0[0x8];
    UnkTuneItem_0x18* items;  // 0x8
    u32 count;                // 0x10
    u8 pad_0x14[0x24];
    f32 field_0x38;
    f32 field_0x3c;
    u8 pad_0x40[0x10];

    UnkTuneItem_0x18* Find(u64 id) {
        for (u64 j = 0; j < count; ++j)
            if (items[j].id == id)
                return &items[j];
        return nullptr;
    }
};

struct UnkTuneArray {
    UnkTuneEntry_0x50* data;
    u32 count;
    UnkTuneEntry_0x50& operator[](u32 i) { return data[i]; }
};

struct UnkTuneNode {
    u8 pad_0x0[0x8];
    UnkTuneArray* array;  // 0x8
    UnkTuneNode* next;    // 0x10
};

struct UnkTuneMgrView_b0019 {
    u8 pad_0x0[0x170];
    UnkTuneNode* tunes;  // 0x170
};

f32 fn_7100fdc468(UnkTuneMgrView_b0019* self, const u64* id) {
    UnkTuneNode* n = self->tunes;
    if (n) {
        u64 key = *id;
        do {
            UnkTuneArray& a = *n->array;
            for (u64 i = 0; i < a.count; ++i) {
                UnkTuneEntry_0x50& e = a.data[i];
                for (u64 j = 0; j < e.count; ++j)
                    if (e.items[j].id == key)
                        return a[i].field_0x38;
            }
            n = n->next;
        } while (n);
    }
    return 1.0f;
}

f32 fn_7100fdc500(UnkTuneMgrView_b0019* self, const u64* id) {
    u64 key;
    for (UnkTuneNode* n = self->tunes; n; n = n->next) {
        key = *id;
        UnkTuneArray& a = *n->array;
        for (u64 i = 0; i < a.count; ++i) {
            UnkTuneEntry_0x50& e = a.data[i];
            for (u64 j = 0; j < e.count; ++j)
                if (e.items[j].id == key)
                    return a[i].field_0x3c;
        }
    }
    return -1.0f;
}

void fn_7100fdc3c4(UnkTuneMgrView_b0019* self, const u64* id, s32* a_out, s32* b_out) {
    *a_out = -1;
    *b_out = -1;
    UnkTuneNode* n = self->tunes;
    if (!n)
        return;
    u64 key = *id;
    for (; n; n = n->next) {
        UnkTuneArray& a = *n->array;
        for (u64 i = 0; i < a.count; ++i) {
            UnkTuneEntry_0x50& e = a.data[i];
            for (u64 j = 0; j < e.count; ++j) {
                if (e.items[j].id == key) {
                    UnkTuneRange& r = e.items[j].range;
                    *a_out = r.first;
                    *b_out = r.second;
                    return;
                }
            }
        }
    }
}

struct UnkReqEntry_0x60 {
    u8 pad_0x0[0xc];
    f32 field_0xc;
    u8 pad_0x10[0x14];
    bool flag;     // 0x24
    u8 pad_0x25[3];
    u64* handle;   // 0x28
    s32 active;    // 0x30
    u8 pad_0x34[0x2c];
};

struct UnkReqMgrView_b0019 {
    u8 pad_0x0[0xb8];
    UnkReqEntry_0x60* entries;  // 0xb8
    u32 count;                  // 0xc0
};

bool fn_7100fd6308(UnkReqMgrView_b0019* self, bool flag, UnkReqEntry_0x60* other) {
    if (other->active == 0)
        return false;
    u64* h = other->handle;
    for (u64 i = 0; i < self->count; ++i) {
        UnkReqEntry_0x60& e = self->entries[i];
        if (e.active == 0)
            continue;
        if (*e.handle != *h)
            continue;
        if (e.flag == flag)
            continue;
        e.flag = flag;
        fn_7100108a98(g_unk_7101d4d820, (u64)e.handle, flag, e.field_0xc);
        return true;
    }
    return false;
}

struct UnkTuneStr_b0019 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
    u8 pad_0x38[0xc];
    union {
        u32 uses;  // 0x44 (a union member, so the entry's pointer is reloaded after the increment)
    };
};

struct UnkPathRecord;

void fn_7100456ac0(UnkPathRecord* self);

struct UnkPathRecord_b0019 {
    u8 pad_0x0[0x18];
};

// The entry's file-path list; fn_7100456980 is its out-of-line reserve.
struct UnkPathVec_b0019 {
    UnkPathRecord_b0019* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;
    u32 flags;
    void* allocator;

    UnkPathRecord_b0019& operator[](u32 i);
};

void fn_7100456980(UnkPathVec_b0019* v, u32 n);

inline UnkPathRecord_b0019& UnkPathVec_b0019::operator[](u32 i) {
    if (autoGrow && count <= i) {
        u32 n = capacity;
        if (capacity <= i && step != 0)
            n = (i / step + 1) * step;
        fn_7100456980(this, n);
        fn_7100456980(this, i + 1);
        for (u32 j = count; j < i + 1; ++j)
            fn_7100456ac0(reinterpret_cast<UnkPathRecord*>(&data[j]));
        count = i + 1;
    }
    return data[i];
}

struct UnkMusicEntry_b0019 {
    u8 pad_0x0[0x28];
    UnkPathVec_b0019 paths;  // 0x28
    u8 pad_0x50[0x8];
    UnkTuneStr_b0019* name;  // 0x58
};

struct UnkEntryArray_b0019 {
    UnkMusicEntry_b0019* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    UnkMusicEntry_b0019& operator[](u64 i) {
        if (autoGrow && i >= count) {
            u32 n = capacity;
            if (i >= capacity && step != 0)
                n = ((u32)i / step + 1) * step;
            fn_7100fddcb4(reinterpret_cast<base::UnkVector<UnkMusicEntry_0x60>*>(this), n);
            fn_7100fde4d8(reinterpret_cast<UnkArray*>(this), i + 1);
        }
        return data[i];
    }
};

struct UnkMusicMgr_b0019 {
    u8 pad_0x0[0xaa];
    bool enabled;  // 0xaa
    u8 pad_0xab[0x1];
    s32 hold_count;  // 0xac
    u8 pad_0xb0[0x8];
    UnkEntryArray_b0019 entries;  // 0xb8
    u8 pad_0xd0[0x180];
    u32 slot_count;  // 0x250
};

static inline UnkRequest* Request(UnkMusicMgr_b0019* self, u64 i) {
    return reinterpret_cast<UnkRequest*>(&self->entries[i]);
}

void fn_7100fd7b44(UnkMusicMgr_b0019* self, UnkKind kind, UnkMask mask, f32 time) {
    for (u64 i = 0; i < self->slot_count; ++i)
        fn_7100fd7c3c(reinterpret_cast<UnkMgr_0xf18*>(self), i, Request(self, i), time, kind, mask);
}

void fn_7100fd4430(UnkMusicMgr_b0019* self, bool enabled) {
    if (self->enabled == enabled)
        return;
    self->enabled = enabled;
    if (!enabled) {
        fn_7100108b0c(g_unk_7101d4d820, 1, 0.2f);
        return;
    }
    for (u64 i = 0; i < self->slot_count; ++i)
        fn_7100fd4544(reinterpret_cast<UnkMgr_0xf18*>(self), i, Request(self, i), AddressOf(UnkKind(2)));
}

struct UnkTuneKey_b0019 {
    u8 pad_0x0[0x20];
    u64 id;  // 0x20
};

void fn_7100fd82dc(UnkMusicMgr_b0019* self, UnkTuneKey_b0019* key, UnkKind kind, UnkMask mask, f32 time) {
    for (u64 i = 0; i < self->slot_count; ++i) {
        UnkMusicEntry_b0019& e = self->entries[i];
        ++e.name->uses;
        if (e.name->id == key->id)
            fn_7100fd7c3c(reinterpret_cast<UnkMgr_0xf18*>(self), i, reinterpret_cast<UnkRequest*>(&e), time, kind, mask);
    }
}

static inline void SetAllHeld(UnkMusicMgr_b0019* self, bool held) {
    for (u64 i = 0; i < self->slot_count; ++i) {
        if (self->entries[i].paths.count == 0)
            continue;
        fn_7100108a98(g_unk_7101d4d820, (u64)&self->entries[i].paths[0], held, 0.2f);
    }
}

// Holds every active request; nested calls are counted (fn_7100fdbbd0 releases them).
void fn_7100fdba30(UnkMusicMgr_b0019* self) {
    if (self->hold_count++ != 0)
        return;
    SetAllHeld(self, true);
}

// A string id over the given pool whose destructor decrements and then re-tests the count.
template <base::global::UnkStringPool** Pool>
struct UnkPooledId {
    base::global::UnkStringData* data;

    ~UnkPooledId() {
        if (base::global::UnkStringPool* pool = *Pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = *Pool;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkPathEntry_0x18 {
    UnkPooledId<&g_unk_7101d47200> field_0x0;
    u64 field_0x8;
    u64 field_0x10;
};

struct UnkPathList : base::UnkVector<UnkPathEntry_0x18> {
    ~UnkPathList() { Free(); }
};

// A node of a slot's list (0x70 bytes).
struct UnkNode_0x68 {
    u8 pad_0x0[0x28];
    UnkPathList paths;  // 0x28
    u64 field_0x50;
    UnkPooledId<&g_unk_7101d47108> name;  // 0x58
    UnkNode_0x68* next;  // 0x60
    UnkNode_0x68* prev;  // 0x68
};

// The links are reached through these, so they carry no struct-path alias info (the original
// reloads them after each store).
static inline UnkNode_0x68*& Next(UnkNode_0x68* n) { return n->next; }
static inline UnkNode_0x68*& Prev(UnkNode_0x68* n) { return n->prev; }

// The node destructor (kept out of line, as the list's erase calls it).
__attribute__((noinline)) void fn_7100fde6c4(UnkNode_0x68* node) {
    node->~UnkNode_0x68();
}

struct UnkNodeList_b0019 {
    char* vtable;
    union {
        u32 count;  // a union member: the original reloads the links after updating it
    };
    UnkNode_0x68* head;
    UnkNode_0x68* tail;
    u32 flags;
    base::UnkAllocator* allocator;
    void* field_0x30;

    void Destroy(UnkNode_0x68* node) {
        fn_7100fde6c4(node);
        allocator->vf_0x48(node);
    }
};

// Unlinks and frees the node, returning the node after it.
__attribute__((noinline)) UnkNode_0x68* fn_7100fd8f98(UnkNodeList_b0019* list, UnkNode_0x68* node) {
    if (list->head == node) {
        if (!node)
            return nullptr;
        list->head = node->next;
        if (list->head)
            list->head->prev = nullptr;
        list->Destroy(node);
        if (--list->count == 0)
            list->tail = list->head;
        return list->head;
    }
    if (list->tail == node) {
        if (!node)
            return nullptr;
        list->tail = node->prev;
        if (list->tail)
            list->tail->next = nullptr;
        list->Destroy(node);
        if (--list->count == 0)
            list->tail = list->head;
        return list->tail;
    }
    for (UnkNode_0x68* p = list->head; p;) {
        UnkNode_0x68* after = p->next;
        if (p == node) {
            Next(Prev(node)) = after;
            if (Next(node))
                Prev(Next(node)) = Prev(node);
            --list->count;
            UnkNode_0x68* next = Next(node);
            list->Destroy(node);
            return next;
        }
        p = after;
    }
    return nullptr;
}

struct UnkSlotArray_b0019 {
    UnkMusicSlot_0x38* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool autoGrow;

    __attribute__((always_inline)) UnkMusicSlot_0x38& operator[](u32 index) {
        if (autoGrow && count <= index) {
            u32 n = capacity;
            if (capacity <= index && step != 0)
                n = (index / step + 1) * step;
            auto* v = reinterpret_cast<base::UnkVector<UnkMusicSlot_0x38>*>(this);
            fn_7100fde040(v, n);
            fn_7100fde040(v, index + 1);
            for (u32 i = count; i < index + 1; ++i)
                new (&data[i]) UnkMusicSlot_0x38();
            count = index + 1;
        }
        return data[index];
    }
};

struct UnkMusicSlots_b0019 {
    u8 pad_0x0[0xe0];
    UnkSlotArray_b0019 slots;  // 0xe0
};

// Erases every node of the slot whose name is the given id.
void fn_7100fd5d7c(UnkMusicSlots_b0019* self, u32 index, const u64* id) {
    UnkNodeList_b0019* list = reinterpret_cast<UnkNodeList_b0019*>(&self->slots[index]);
    UnkNode_0x68* next;
    for (UnkNode_0x68* n = list->head; n; n = next) {
        if ((u64)n->name.data == *id)
            next = fn_7100fd8f98(list, n);
        else
            next = n->next;
    }
}


struct UnkNodeKind_b0019 {
    u32 kind;  // 0x0: the node's bit in a mask
};

// Stops and erases every node of the slot whose kind is in the mask.
__attribute__((noinline)) void fn_7100fd5a90(UnkMusicSlots_b0019* self, u32 index, UnkMask mask) {
    UnkNodeList_b0019* list = reinterpret_cast<UnkNodeList_b0019*>(&self->slots[index]);
    UnkNode_0x68* next;
    for (UnkNode_0x68* n = list->head; n; n = next) {
        u32 bit = 1 << reinterpret_cast<UnkNodeKind_b0019*>(n)->kind;
        if (mask.bits & bit) {
            if (n->paths.count)
                fn_7100108a84(reinterpret_cast<UnkMgr*>(g_unk_7101d4d820), reinterpret_cast<UnkHandle*>(n->paths.data), 0.0f);
            next = fn_7100fd8f98(list, n);
        } else {
            next = n->next;
        }
    }
}

struct UnkMusicRequest_b0019 {
    u32 kind;  // 0x0
    u8 pad_0x4[0x2c];
    u32 active;  // 0x30
};

void fn_7100fd8a60(UnkNodeList_b0019* list, UnkMusicRequest_b0019* request);

// Adds the request to the slot's list, first stopping the slot's nodes of the same kind when asked.
void fn_7100fd5c10(UnkMusicSlots_b0019* self, u32 index, UnkMusicRequest_b0019* request, void* unused, bool replace) {
    if (request->active == 0)
        return;
    if (replace)
        fn_7100fd5a90(self, index, UnkMask(1 << request->kind));
    fn_7100fd8a60(reinterpret_cast<UnkNodeList_b0019*>(&self->slots[index]), request);
}


// Stops the slot's last node and erases it; with no node in slot 0, fades the sound system instead.
void fn_7100fd613c(UnkMusicSlots_b0019* self, u32 index, bool immediate) {
    if (index >= self->slots.count)
        return;
    UnkNode_0x68* last = reinterpret_cast<UnkNodeList_b0019*>(&self->slots.data[index])->tail;
    if (last) {
        {
            UnkKind kind(immediate ? 1 : 0x100);
            fn_7100fd4544(reinterpret_cast<UnkMgr_0xf18*>(self), index, reinterpret_cast<UnkRequest*>(last), &kind);
        }
        fn_7100fd8f98(reinterpret_cast<UnkNodeList_b0019*>(&self->slots[index]), last);
    } else if (index == 0 && immediate) {
        UnkSoundMgr* sound = g_unk_7101d4d820;
        f32 volume = *reinterpret_cast<f32*>(reinterpret_cast<char*>(&g_unk_7101d68580) + 0x3c);
        fn_71000fbe1c(reinterpret_cast<char*>(sound) + 8, 5, volume, 0.5f);
    }
}

// A string id whose destructor decrements and then re-tests the count.
struct UnkSetKey_b0019 {
    base::global::UnkStringData* data;

    ~UnkSetKey_b0019() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    fn_71000803a4(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
            data = &pool->empty;
        }
    }
};

struct UnkIdSetNode_0x28 {
    UnkSetKey_b0019 key;
    void* value;
    UnkIdSetNode_0x28* next;
    UnkIdSetNode_0x28* prev;
    UnkIdSetNode_0x28* chain;
};

// A chained hash map keyed by string id, with its nodes also kept in insertion order.
struct UnkIdSet_0x48 {
    base::UnkAllocator* allocator;
    u32 flags;
    u32 count;
    UnkIdSetNode_0x28** buckets;
    u32 bucket_count;
    u32 bucket_capacity;
    u8 pad_0x18[0x18];
    UnkIdSetNode_0x28* last;   // 0x38
    UnkIdSetNode_0x28* first;  // 0x40

    void Clear() {
        if (count == 0)
            return;
        for (UnkIdSetNode_0x28* n = first; n;) {
            UnkIdSetNode_0x28* next = n->next;
            n->~UnkIdSetNode_0x28();
            allocator->vf_0x48(n);
            n = next;
        }
        __builtin_memset(buckets, 0, (u64)bucket_capacity * sizeof(UnkIdSetNode_0x28*));
        count = 0;
        last = nullptr;
        first = nullptr;
    }
};

struct UnkMusicSets_b0019 {
    u8 pad_0x0[0x130];
    UnkIdSet_0x48 sources;  // 0x130
    UnkIdSet_0x48 owners;   // 0x178
};

// Empties both sets.
void fn_7100fd5348(void* manager) {
    UnkMusicSets_b0019* self = static_cast<UnkMusicSets_b0019*>(manager);
    self->sources.Clear();
    self->owners.Clear();
}
