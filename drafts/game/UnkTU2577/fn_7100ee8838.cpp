// CSubAreaManager::CTunableSubAreaManager unit (tools/tu_map.py unit 2577): fn_7100ee8838: fills two lists with
// what the previous owner's list at +0xa0 has and the current one's does not, and the reverse (or with both
// whole lists when everything is reloaded). Names and layouts are placeholders.
#include <new>
#include "base/UnkResPathVec.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;

struct CSubAreaManager;
struct UnkIdList;

UnkResPathVec* fn_71000d0958(UnkResPathVec* self, const UnkResPathVec* other);

// What a list entry points at, seen as a counted object.
struct UnkCountedItem_lee8838 {
    u8 pad_0x0[0x30];
    u64 field_0x30;
    u8 pad_0x38[0xc];
    union {
        u32 refs;  // 0x44
    };
};

// The id table (fn_7100031894 builds it); the slot at 0x5f8 is the "none" item.
struct UnkGameManagerIdTable {
    u8 pad_0x0[0x5f8];
    UnkCountedItem_lee8838* field_0x5f8;
    u8 pad_0x600[0x1f78];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};
inline UnkGameManagerIdTable& GameManagerIdTable() {
    static UnkGameManagerIdTable s_table;
    return s_table;
}

// An owner of a list at +0xa0.
struct UnkListOwner_lee8838 {
    u8 pad_0x0[0xa0];
    UnkResPathVec list;  // 0xa0
};

static inline UnkCountedItem_lee8838* Item(const CFilePathStrId* id) {
    return *reinterpret_cast<UnkCountedItem_lee8838* const*>(id);
}

// Gives the id back to its pool (the count is read again after the decrement) and leaves the empty id.
static inline void ResetId(CFilePathStrId* id) {
    if (base::global::UnkStringPool* pool = g_unk_7101d47200) {
        if (id->data && id->data != &pool->empty) {
            --id->data->ref_count;
            if (id->data->ref_count == 0) {
                fn_71000803a4(pool, id->data);
                pool = g_unk_7101d47200;
            }
        }
        id->data = &pool->empty;
    }
}

static inline void Clear(UnkResPathVec* v) {
    for (u64 i = 0; i < v->count; ++i)
        ResetId(&v->data[i]);
    v->count = 0;
}

static inline void Release(UnkResPathVec* v) {
    if (v->data) {
        Clear(v);
        v->allocator->vf_0x48(v->data);
        v->capacity = 0;
    }
}

static inline void Append(UnkResPathVec* v, const CFilePathStrId& id) {
    u32 n = v->count;
    u32 c = v->capacity;
    if (c <= n && v->step != 0)
        c = (n / v->step + 1) * v->step;
    if ((s32)c <= (s32)n)
        for (;;) {
        }
    fn_7100018450(v, c);
    new (&v->data[v->count]) CFilePathStrId(id);
    ++v->count;
}

static inline CFilePathStrId* Find(UnkResPathVec* v, const CFilePathStrId* id) {
    for (u32 i = 0; i < v->count; ++i)
        if (v->data[i].data == id->data)
            return &v->data[i];
    return &v->data[v->count];
}

// The owner's list, or nothing without an owner.
static inline void ListOf(void* owner, UnkResPathVec* out) {
    Clear(out);
    if (owner)
        fn_71000d0958(out, &static_cast<UnkListOwner_lee8838*>(owner)->list);
}

// True for the "none" item (both sides are counted while they are compared).
static inline bool IsNone(const CFilePathStrId* it) {
    UnkGameManagerIdTable& table = GameManagerIdTable();
    table.field_0x5f8->refs++;
    Item(it)->refs++;
    UnkCountedItem_lee8838* none = table.field_0x5f8;
    Item(it)->refs++;
    return Item(it)->field_0x30 == none->field_0x30;
}

// Appends to `out` each entry of `from` that is not the "none" item and is not in `other`.
static inline void AppendMissing(UnkResPathVec* out, UnkResPathVec* from, UnkResPathVec* other) {
    for (CFilePathStrId* it = from->data; it != from->data + from->count; ++it) {
        if (IsNone(it))
            continue;
        if (Find(other, it) != &other->data[other->count])
            continue;
        Append(out, *it);
    }
}

void fn_7100ee8838(CSubAreaManager* self, void* prev, void* cur, UnkIdList* a, UnkIdList* b, bool all) {
    UnkResPathVec* out_a = reinterpret_cast<UnkResPathVec*>(a);
    UnkResPathVec* out_b = reinterpret_cast<UnkResPathVec*>(b);
    Clear(out_a);
    Clear(out_b);
    UnkResPathVec prev_list(0xf);
    ListOf(prev, &prev_list);
    UnkResPathVec cur_list(0xf);
    ListOf(cur, &cur_list);
    if (all) {
        fn_71000d0958(out_a, &prev_list);
        fn_71000d0958(out_b, &cur_list);
    } else {
        AppendMissing(out_b, &cur_list, &prev_list);
        AppendMissing(out_a, &prev_list, &cur_list);
    }
    Release(&cur_list);
    Release(&prev_list);
}
