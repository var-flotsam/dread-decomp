// Unit 100's counters (tools/tu_map.py; no class hint): a limit and an array of counters keyed by id,
// each with its own limit and a count of uses. The array's element accessor (grows on demand), the
// use counting (adds a counter with the shared limit for a new key) and the load from a file (the
// shared limit, then the counters with their limits and keys by name hash). Names and layouts are
// placeholders.
#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))
#include <string.h>

#include "base/UnkHashStream.h"
#include "base/UnkStrIdPathPair.h"
#include "base/UnkVector.h"
#include "base/file/CThreadSafeDevice.h"
#include "base/global/CRntFile.h"
#include "base/global/CRntString.h"
#include "game/UnkTU102.h"
#include "game/UnkTU56.h"
#include "types.h"
#include "base/UnkStrPair.h"

using base::global::CRntFile;

struct UnkCounter_ovb0259 {
    u32 key;
    u32 limit;
    u32 count;
};

struct UnkCounters_ovb0259 {
    u32 limit;
    u32 pad_0x4;
    base::UnkVector<UnkCounter_ovb0259> items;  // 0x8
};

struct UnkFileReader;
bool fn_710007c1ac(UnkFileReader* self, const UnkStrPair_0x10* path, const base::global::CRntString* mode);
void fn_710007c144(UnkHashStream_0x38* s);
extern const char* g_unk_7101a1bd30;

__attribute__((noinline)) UnkCounter_ovb0259* fn_71001047e0(base::UnkVector<UnkCounter_ovb0259>* v, u32 index) {
    return &v->GrowAt(index);
}

// Counts one use of a key against its limit.
bool fn_7100104940(UnkCounters_ovb0259* self, const u32* key) {
    base::UnkVector<UnkCounter_ovb0259>& v = self->items;
    UnkCounter_ovb0259* entry = nullptr;
    if (v.count) {
        UnkCounter_ovb0259* p = v.data;
        for (u64 n = v.count; n; ++p, --n) {
            if (p->key == *key) {
                // the original tests the found pointer even though it was just read through
                asm("" : "+r"(p));
                entry = p;
                break;
            }
        }
    }
    if (!entry) {
        u32 n = v.count + 1;
        v.Reserve(n);
        __builtin_memset(&v.data[v.count], 0, (u64)(n - v.count) * sizeof(UnkCounter_ovb0259));
        v.count = n;
        UnkCounter_ovb0259* end = &v.data[n];
        entry = n ? end - 1 : end;
        entry->count = 0;
        entry->limit = self->limit;
        entry->key = *key;
    }
    if (entry->count >= entry->limit)
        return false;
    ++entry->count;
    return true;
}

void fn_7100104600(void* self_) {
    UnkCounters_ovb0259* self = static_cast<UnkCounters_ovb0259*>(self_);
    CRntFile file;
    fn_710007be8c(&file, 0xb, nullptr);
    bool opened;
    {
        UnkStrPair_0x10 s;
        fn_710008d680(&s, g_unk_7101a1bd30);
        {
            base::global::CRntString mode("rb");
            opened = fn_710007c1ac(reinterpret_cast<UnkFileReader*>(&file), &s, &mode);
        }
        fn_710008d8e8(reinterpret_cast<UnkStrIdPathPair*>(&s));
    }
    if (opened) {
        fn_710007c00c(&file, 8, 0);
        fn_710007c714(&file, self, 4, 1);
        u32 count;
        fn_710007c714(&file, &count, 4, 1);
        base::UnkVector<UnkCounter_ovb0259>& v = self->items;
        u32 n = count;
        if (v.count < n) {
            v.Reserve(n);
            __builtin_memset(&v.data[v.count], 0, (u64)(n - v.count) * sizeof(UnkCounter_ovb0259));
            v.count = n;
        } else if (v.count > n) {
            v.count = n;
        }
        for (u32 i = 0; i < count; ++i) {
            const char* name;
            UnkCounter_ovb0259* e = fn_71001047e0(&v, i);
            fn_710007cee4(&file, &name);
            fn_710007c714(&file, &e->limit, 4, 1);
            e->count = 0;
            e->key = fn_7100106820(name);
        }
        fn_710007c144(&file);
    }
    fn_710007c0e4(&file);
}
