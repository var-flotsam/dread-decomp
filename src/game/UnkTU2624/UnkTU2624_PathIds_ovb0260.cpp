// Unit 2624: reading the id lists of a drop-target record (fn_7101025974) and handing a record's ids over
// (fn_7101025984), moving them out of the record. Names and layouts are placeholders.
#include "base/UnkResPathVec.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkDropTarget;
extern UnkDropTarget* g_unk_7101d47318;

struct UnkPathHolder_b260 {
    u8 unk_0x0[0x10];
    CFilePathStrId path;  // 0x10
};

struct UnkHolderVec_b260 {
    UnkPathHolder_b260** data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    explicit UnkHolderVec_b260(u32 flags)
        : data(nullptr), count(0), capacity(0), step(10), field_0x14(true), flags(flags),
          allocator(fn_7100083990()) {}
};

void fn_7101025974(UnkDropTarget* target, UnkHolderVec_b260* out);

UnkResPathVec fn_7100fc85d4() {
    UnkHolderVec_b260 tmp(0xf);
    UnkResPathVec result(0xf);
    fn_7101025974(g_unk_7101d47318, &tmp);
    for (UnkPathHolder_b260** p = tmp.data, **end = tmp.data + tmp.count; p != end; ++p) {
        UnkPathHolder_b260* holder = *p;
        u32 n = result.count;
        u32 new_capacity = result.capacity;
        if (new_capacity <= n && result.step != 0)
            new_capacity = (n / result.step + 1) * result.step;
        if ((s32)n >= (s32)new_capacity)
            for (;;) {
            }
        fn_7100018450(&result, new_capacity);
        CFilePathStrId* slot = &result.data[result.count];
        base::global::UnkStringData* value = holder->path.data;
        __builtin_memcpy(slot, &value, sizeof(value));
        if (&g_unk_7101d47200->empty != value)
            ++value->ref_count;
        ++result.count;
    }
    if (tmp.data) {
        tmp.count = 0;
        tmp.allocator->vf_0x48(tmp.data);
        tmp.capacity = 0;
    }
    return result;
}

using base::global::UnkStringPool;

struct UnkIdRecord_b260 {
    u8 unk_0x0[8];
    CStrId name;       // 0x8
    CFilePathStrId path;  // 0x10
};

// CFilePathStrId over the file-path pool, released in two steps by the destructor below.
static inline void ResetPath(base::global::UnkFilePathStrIdBase& id);

struct UnkIdPair_b260 {
    CStrId name;
    base::global::UnkFilePathStrIdBase path;
    UnkIdPair_b260(const CStrId& n, const CFilePathStrId& p) : name(n), path(p) {
        if (&g_unk_7101d47200->empty != path.data)
            ++path.data->ref_count;
    }
    ~UnkIdPair_b260() { ResetPath(path); }
};

void fn_7101025984(UnkDropTarget* target, UnkIdPair_b260* pair);

// CStrId::Reset on the file-path pool.
static inline void ResetPath(base::global::UnkFilePathStrIdBase& id) {
    if (UnkStringPool* pool = g_unk_7101d47200) {
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

void fn_7100fc8430(void* self, UnkIdRecord_b260* rec) {
    if (!rec)
        return;
    UnkIdPair_b260 pair(rec->name, rec->path);
    rec->name.Reset();
    ResetPath(rec->path);
    fn_7101025984(g_unk_7101d47318, &pair);
}
