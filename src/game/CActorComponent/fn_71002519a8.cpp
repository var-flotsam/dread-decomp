// Lookups in parallel arrays of values and 8-byte keys (base::UnkKeyedArray): the value stored
// for a key, or null when the key is absent. With 8-byte values: fn_71002519a8, fn_71000dacdc and
// fn_71000d92a8; fn_7100251a80 is a relative. Called only through their collection function tables.

#include "base/UnkCloneCalls.h"
#include "base/global/CRntSmallDictionary.h"
#include "game/CCharClassLiquidPoolComponent.h"
#include "game/UnkRecord_0x40.h"
#include "game/UnkTU23.h"
#include "game/UnkValue_0x10.h"
#include "types.h"

struct UnkKeyedValues_71002519a8;
using UnkValuesByKey = base::UnkKeyedArray<UnkKeyedValues_71002519a8, u64, u64>;

static inline u64* FindValue(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}

u64* fn_71002519a8(UnkValuesByKey* a, const u64* key) { return FindValue(a, key); }

u64* fn_71000dacdc(UnkValuesByKey* a, const u64* key) { return FindValue(a, key); }

u64* fn_71000d92a8(UnkValuesByKey* a, const u64* key) { return FindValue(a, key); }

// Removal of the value stored for a key, when present (the removal itself is a separate function per type):
// fn_71000d9234, fn_71000dac68.
void* fn_71000d9de8(UnkValuesByKey* a, u64* value);
void* fn_71000db82c(UnkValuesByKey* a, u64* value);

void fn_71000d9234(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    if (p != a->End())
        fn_71000d9de8(a, p);
}

void fn_71000dac68(UnkValuesByKey* a, const u64* key) {
    u64* p = a->Find(*key);
    if (p != a->End())
        fn_71000db82c(a, p);
}

// The same over 4-byte values (fn_71000dedbc).
struct UnkKeyedWords_71000dedbc;
using UnkWordsByKey = base::UnkKeyedArray<UnkKeyedWords_71000dedbc, u64, u32>;

u32* fn_71000dedbc(UnkWordsByKey* a, const u64* key) {
    u32* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}

// An identical copy (fn_710079c41c).
u32* fn_710079c41c(UnkWordsByKey* a, const u64* key) {
    u32* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}

u32* fn_71000df750(UnkWordsByKey* a, u32* value);

// Removal over 4-byte values (fn_71000ded44).
void fn_71000ded44(UnkWordsByKey* a, const u64* key) {
    u32* p = a->Find(*key);
    if (p != a->End())
        fn_71000df750(a, p);
}

// The same removal in another unit, with its own out-of-line removal (fn_710079c3a4).
u32* CCharClassCollisionComponent_ColMatDictEraseAt(UnkWordsByKey* a, u32* value);
void fn_710079c3a4(UnkWordsByKey* a, const u64* key) {
    u32* p = a->Find(*key);
    if (p != a->End())
        CCharClassCollisionComponent_ColMatDictEraseAt(a, p);
}

struct UnkRecordsByKeyHolder_7100015250 {
    u64 field_0x0;
    base::UnkKeyedArray<UnkRecord_0x40, u64, UnkRecord_0x40> records;
};

static inline UnkRecord_0x40* FindRecord(UnkRecordsByKeyHolder_7100015250* self, u64 key) {
    UnkRecord_0x40* p = self->records.Find(key);
    return p == self->records.End() ? nullptr : p;
}

UnkRecord_0x40* fn_7100015250(UnkRecordsByKeyHolder_7100015250* self, u64 key) { return FindRecord(self, key); }

UnkRecord_0x40* fn_7100015508(UnkRecordsByKeyHolder_7100015250* self, u64 key) { return FindRecord(self, key); }

// Whether a key is present in the keyed array of 0x98-byte records held at 8 in the object at
// 0x80 of the object at 0x78 (fn_71001c147c).
struct UnkRecord_0x98 {
    u8 data[0x98];
};

struct UnkRecordsByKeyHolder_71001c147c {
    u64 field_0x0;
    base::UnkKeyedArray<UnkRecord_0x98, u64, UnkRecord_0x98> records;
};

bool fn_71001c147c(char* self, const u64* key) {
    char* owner = *reinterpret_cast<char**>(self + 0x78);
    auto* holder = *reinterpret_cast<UnkRecordsByKeyHolder_71001c147c**>(owner + 0x80);
    UnkRecord_0x98* p = holder->records.Find(*key);
    if (p == holder->records.End())
        p = nullptr;
    return p != nullptr;
}

struct UnkValue_0x80 {
    u64 words[16];
};
struct UnkKeyedValues_71009046bc;
struct UnkKeyedValues_7100e6095c;
using UnkValues10ByKey = base::UnkKeyedArray<UnkKeyedValues_71009046bc, u64, UnkValue_0x10>;
using UnkValues80ByKey = base::UnkKeyedArray<UnkKeyedValues_7100e6095c, u64, UnkValue_0x80>;
UnkValue_0x80* fn_7100e6164c(UnkValues80ByKey* a, UnkValue_0x80* value);

template <class A>
__attribute__((always_inline)) static inline auto FindOrNull(A* a, const u64* key) -> decltype(a->End()) {
    auto* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}

UnkValue_0x10* fn_71009046bc(UnkValues10ByKey* a, const u64* key) { return FindOrNull(a, key); }
UnkValue_0x80* fn_7100e6095c(UnkValues80ByKey* a, const u64* key) { return FindOrNull(a, key); }

void fn_7100904644(UnkValues10ByKey* a, const u64* key) {
    UnkValue_0x10* p = a->Find(*key);
    if (p != a->End())
        fn_7100901264(a, p);
}

void fn_7100e608e0(UnkValues80ByKey* a, const u64* key) {
    UnkValue_0x80* p = a->Find(*key);
    if (p != a->End())
        fn_7100e6164c(a, p);
}

// The reserve of a keyed array of 0x40-byte records with its storage (fn_7100015378): both arrays
// grow to `capacity`, the records copied word by word (the count reread for each) and the keys
// copied by a 64-bit index beside a 32-bit counter.
typedef u32 UnkAliasWord;

struct UnkWordRecord_0x40 {
    UnkAliasWord words[16];
};

using UnkRecordsByKeyStore = base::global::CRntSmallDictionary<u64, UnkWordRecord_0x40>;

void fn_7100015378(UnkRecordsByKeyStore* self, u32 capacity) {
    if (self->capacity < capacity) {
        auto* values = static_cast<UnkWordRecord_0x40*>(
            self->allocator->vf_0x40((u64)capacity * sizeof(UnkWordRecord_0x40), 8, self->flags));
        auto* keys = static_cast<u64*>(self->allocator->vf_0x40((u64)capacity * sizeof(u64), 8, self->flags));
        if (UnkWordRecord_0x40* src = self->values) {
            UnkWordRecord_0x40* dst = values;
            for (u32 i = 0; i < self->count; ++i, ++src, ++dst)
                for (u32 w = 0; w < 16; ++w)
                    dst->words[w] = src->words[w];
            self->allocator->vf_0x48(self->values);
        }
        if (u64* src = self->keys) {
            u64 j = 0;
            for (u32 i = 0; i < self->count; ++i, ++j)
                keys[j] = src[j];
            self->allocator->vf_0x48(self->keys);
        }
        self->values = values;
        self->keys = keys;
        self->capacity = capacity;
    }
}
