// The play report (unit 2569 of tools/tu_map.py; src/game/UnkPlayReport.cpp): its copy constructor
// (fn_7100ec536c): the two ids built empty and the record array empty (flags 0xf), then the other
// report assigned over them (the word, the ids, and the records through fn_7100ec55ac). Names and
// layouts are placeholders. Also the string record's constructor (fn_7100ec4240).
#include <new>

#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A report record (0x20 bytes, as UnkPlayReport.cpp has it); only copied as a whole here.
struct UnkPlayReportRecord_b49 {
    u8 data[0x20];
};

// Assigns another record array (out of line).
void* fn_7100ec55ac(base::UnkVector<UnkPlayReportRecord_b49>* self, const base::UnkVector<UnkPlayReportRecord_b49>* other);

// An empty id: the string id constructor for "" (the length folded) inlined.
__attribute__((always_inline)) static inline void InitEmptyId(CStrId* id) {
    id->data = nullptr;
    if (!g_unk_7101d47108)
        base::global::UnkEnsureStringPool();
    id->data = &g_unk_7101d47108->empty;
}

struct UnkPlayReportCopy {
    u32 field_0x0;
    CStrId field_0x8;
    CStrId field_0x10;
    base::UnkVector<UnkPlayReportRecord_b49> records;  // 0x18
};

void fn_7100ec536c(UnkPlayReportCopy* self, const UnkPlayReportCopy* other) {
    InitEmptyId(&self->field_0x8);
    InitEmptyId(&self->field_0x10);
    new (&self->records) base::UnkVector<UnkPlayReportRecord_b49>(0xf);
    self->field_0x0 = other->field_0x0;
    self->field_0x8 = other->field_0x8;
    self->field_0x10 = other->field_0x10;
    fn_7100ec55ac(&self->records, &other->records);
}

// A report record built from a key and a string value (fn_7100ec4240; UnkPlayReport.cpp declares it
// as the constructor taking both ids by value, which pass by address): the ids built empty, then
// assigned; no integer value (-1) and a kind of -1.0f.
struct UnkPlayReportStrRecord_b49 {
    CStrId field_0x0;
    CStrId field_0x8;
    s64 field_0x10;
    f32 field_0x18;
};

void fn_7100ec4240(UnkPlayReportStrRecord_b49* self, const CStrId* name, const CStrId* value) {
    InitEmptyId(&self->field_0x0);
    InitEmptyId(&self->field_0x8);
    self->field_0x0 = *name;
    self->field_0x8 = *value;
    self->field_0x10 = -1;
    self->field_0x18 = -1.0f;
}
