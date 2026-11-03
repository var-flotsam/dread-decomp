// Copies of fn_7100eb1d08 / fn_7100eb1d6c (src/game/CMinimapData_KeyedIter_b45.cpp) in CPlaythrough's
// unit: the Begin and End slots of a reflected keyed collection, whose keys are destroyed by
// fn_71002edacc. Names and layouts are placeholders.
#include "../../base/reflection/UnkTempIterValue_b44.h"
#include "types.h"

// Signature as src/game/CPlaythrough/fn_71002edacc.cpp defines it.
struct UnkHolder;
void fn_71002edacc(UnkHolder* self);

struct UnkKey_b46a1 {
    u64 a;
    ~UnkKey_b46a1() { fn_71002edacc(reinterpret_cast<UnkHolder*>(this)); }
};

struct UnkBlob1_b46a1 {
    u64 w[1];
};

struct UnkKeyedArr_b46a1 {
    UnkBlob1_b46a1* values;
    UnkKey_b46a1* keys;
    u32 count;
};

void fn_71002ef4e4(UnkKeyedArr_b46a1* self, UnkTempIterValue<UnkBlob1_b46a1, UnkKey_b46a1>* out) {
    *out = UnkTempIterValue<UnkBlob1_b46a1, UnkKey_b46a1>(self->keys, self->values);
}

void fn_71002ef548(UnkKeyedArr_b46a1* self, UnkTempIterValue<UnkBlob1_b46a1, UnkKey_b46a1>* out) {
    *out = UnkTempIterValue<UnkBlob1_b46a1, UnkKey_b46a1>(self->keys + self->count, self->values + self->count);
}
