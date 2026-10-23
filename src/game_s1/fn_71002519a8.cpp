// Lookups in parallel arrays of values and 8-byte keys (base::UnkKeyedArray): the value stored
// for a key, or null when the key is absent. With 8-byte values: fn_71002519a8, fn_71000dacdc and
// fn_71000d92a8; fn_7100251a80 is a relative. Called only through their collection function tables.

#include "base/UnkCloneCalls.h"
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

// The same over 4-byte values (fn_71000dedbc).
struct UnkKeyedWords_71000dedbc;
using UnkWordsByKey = base::UnkKeyedArray<UnkKeyedWords_71000dedbc, u64, u32>;

u32* fn_71000dedbc(UnkWordsByKey* a, const u64* key) {
    u32* p = a->Find(*key);
    return p == a->End() ? nullptr : p;
}

// And over 0x40-byte records held at 8 in an object, with the key passed by value
// (fn_7100015250, fn_7100015508).
struct UnkRecord_0x40 {
    u8 data[0x40];
};

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
