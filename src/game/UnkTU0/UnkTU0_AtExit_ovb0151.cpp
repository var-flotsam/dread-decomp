// Unit 0 (the module start): the string-id destructors the static initialisers register with
// __cxa_atexit, and the runtime's atexit table: the registration (two argument orders) and the run of
// the handlers of a DSO, which re-scans for handlers registered meanwhile. Names are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringPool;

void fn_710000051c(void* id) {
    static_cast<CStrId*>(id)->Reset();
}

void fn_7100000c7c(void* id) {
    base::global::UnkStringData*& data = *static_cast<base::global::UnkStringData**>(id);
    if (UnkStringPool* pool = g_unk_7101d47200) {
        if (data && data != &pool->empty) {
            --data->ref_count;
            if (data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47200;
            }
        }
        data = &pool->empty;
    }
}

struct UnkAtExitEntry_b0151 {
    void* field_0x0;
    void* field_0x8;
    void* dso;  // 0x10
};

extern UnkAtExitEntry_b0151 g_unk_7101d46060[];
// The table's end is a second GOT slot that holds the same address in the dump.
namespace folded {
extern UnkAtExitEntry_b0151 g_unk_7101d46060[];
}
extern u64 g_unk_7101d470d8 __attribute__((visibility("hidden")));

// Takes the next free entry; -1 when the DSO handle is null or the table is full.
static inline int AddEntry(void* first, void* second, void* dso) {
    if (!dso)
        return -1;
    u64 capacity = folded::g_unk_7101d46060 - ::g_unk_7101d46060;
    if (g_unk_7101d470d8 >= capacity)
        return -1;
    u64 index = __atomic_fetch_add(&g_unk_7101d470d8, 1, __ATOMIC_SEQ_CST);
    if (index >= capacity)
        return -1;
    UnkAtExitEntry_b0151* entry = &g_unk_7101d46060[index];
    entry->field_0x0 = first;
    entry->field_0x8 = second;
    entry->dso = dso;
    return 0;
}

int fn_71000001c0(void* arg, void* func, void* dso) {
    return AddEntry(func, arg, dso);
}

int fn_7100000250(void (*func)(void*), void* arg, void* dso) {
    return AddEntry(reinterpret_cast<void*>(func), arg, dso);
}

void fn_71000002f0(void* dso, u64 first, u64 last) {
    u64 capacity = folded::g_unk_7101d46060 - ::g_unk_7101d46060;
    u64 n = last < capacity ? last : capacity;
    while (n > first) {
        u64 count = g_unk_7101d470d8;
        --n;
        u64 next_last = count;
        if (g_unk_7101d46060[n].dso == dso) {
            reinterpret_cast<void (*)(void*)>(g_unk_7101d46060[n].field_0x0)(g_unk_7101d46060[n].field_0x8);
            next_last = g_unk_7101d470d8;
        }
        fn_71000002f0(dso, count, next_last);
    }
}

int fn_7100000190(void* dso) {
    fn_71000002f0(dso, 0, g_unk_7101d470d8);
    return 0;
}
