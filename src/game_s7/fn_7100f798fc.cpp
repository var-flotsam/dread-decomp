// The end iterators of the string-id hash maps of fn_7100f79844 (src/game_7100ece7fc/fn_7100f79844.cpp), one after each
// begin iterator there (as fn_71000d7024 in src/game_s1/fn_71000d6844.cpp): the same, from no node. The types below are
// the begin iterators' file's.
// A temporary iterator is advanced (fn_7100f5aff0 and its copy), the result starts at the node it reached and is
// advanced in turn, and the temporary's cached entry (kind 1: a key and a value) is destroyed.
#include "base/global/CStrId.h"
#include "types.h"

using base::global::UnkStringData;
using base::global::UnkStringPool;

// A string id whose release decrements and tests the count in two steps.
struct UnkMapKey {
    UnkStringData* data;

    ~UnkMapKey() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
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

template <u32 ValueSize, void (*Destroy)(void*)>
struct UnkMapIter {
    void* node;
    union {
        struct {
            UnkMapKey key;
            u8 value[ValueSize];  // destroyed by Destroy
        } entry;  // kind 1
    };
    u32 kind;

    void Advance();

    __attribute__((always_inline)) ~UnkMapIter() {
        switch (kind) {
        case 1:
            Destroy(entry.value);
            entry.key.~UnkMapKey();
            kind = 0;
            break;
        case 2:
            kind = 0;
            break;
        }
    }
};

struct UnkMapHead {
    u8 unk_0x0[0x40];
    void* head;
};

template <u32 ValueSize, void (*Destroy)(void*)>
__attribute__((always_inline)) inline void End(UnkMapIter<ValueSize, Destroy>* out) {
    UnkMapIter<ValueSize, Destroy> it;
    it.node = nullptr;
    it.kind = 0;
    it.Advance();
    out->node = it.node;
    out->Advance();
}

void fn_7100d8f9e4(void* value);
void fn_7101036e34(void* value);
void fn_7100d90304(void* value);
void fn_7100f8da4c(void* value);
void fn_7100fee034(void* value);

void fn_7100f798fc(const UnkMapHead* map, UnkMapIter<0x3a8, fn_7100d8f9e4>* out) {
    End(out);
}

void fn_71010427e0(const UnkMapHead* map, UnkMapIter<0x5d0, fn_7101036e34>* out) {
    End(out);
}

void fn_7100f876c4(const UnkMapHead* map, UnkMapIter<0x68, fn_7100d90304>* out) {
    End(out);
}

void fn_7100f97328(const UnkMapHead* map, UnkMapIter<0x78, fn_7100f8da4c>* out) {
    End(out);
}

void fn_7101005cd8(const UnkMapHead* map, UnkMapIter<0x50, fn_7100fee034>* out) {
    End(out);
}
