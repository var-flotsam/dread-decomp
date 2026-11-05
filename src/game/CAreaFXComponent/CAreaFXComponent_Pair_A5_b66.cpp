// CAreaFXComponent's unit: builds a pair of results from one 16-byte record (copied field by field into
// each by-value argument) and two different third arguments (fn_71007fdc30). Names are placeholders.
#include "types.h"

struct UnkAreaFXRecord_A5_b66 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;

    UnkAreaFXRecord_A5_b66(const UnkAreaFXRecord_A5_b66& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc) {}
};

struct UnkAreaFXResult_A5_b66 {
    u64 field_0x0;

    UnkAreaFXResult_A5_b66(const UnkAreaFXResult_A5_b66& other);
};

struct UnkAreaFXResultPair_A5_b66 {
    UnkAreaFXResult_A5_b66 first;
    UnkAreaFXResult_A5_b66 second;
};

UnkAreaFXResult_A5_b66 fn_71007fdc30(UnkAreaFXRecord_A5_b66 record, void* a, void* b);

UnkAreaFXResultPair_A5_b66 fn_71007fdff0(const UnkAreaFXRecord_A5_b66* self, void* a, void* b, void* c) {
    return {fn_71007fdc30(*self, a, b), fn_71007fdc30(*self, a, c)};
}
