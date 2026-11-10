// Unit 1323: the heatable shield value record (0xd0 bytes): its default constructor and destructor
// (string ids released like CStrId, an id list freed, a record of five ids). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkHeatableValue_0xd0;
struct UnkIdRecord_0x28;
void fn_710086e288(UnkIdRecord_0x28* self);

// A string id whose release decrements and tests the count in two steps.
struct UnkTwoStepId_b260 {
    UnkStringData* data;

    ~UnkTwoStepId_b260() {
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

struct UnkHeatableValue_b260 {
    CStrId id0;                       // 0x0
    s32 field_0x8;
    CStrId id10;                      // 0x10
    CStrId id18;                      // 0x18
    base::UnkVector<CStrId> list;     // 0x20
    CStrId id48;                      // 0x48
    u64 field_0x50, field_0x58;
    u32 field_0x60;
    u8 unk_0x64[4];
    u64 field_0x68, field_0x70;
    CStrId ids78[5];                  // 0x78
    u32 field_0xa0;
    u8 unk_0xa4[4];
    u64 field_0xa8;
    s64 field_0xb0;
    u64 field_0xb8;
    s64 field_0xc0;
    u64 field_0xc8;

    UnkHeatableValue_b260()
        : id0(g_unk_7101d470e0), field_0x8(-1), id10(g_unk_7101d470e0), id18(g_unk_7101d470e0), list(0xf),
          id48(g_unk_7101d470e0), field_0x50(0), field_0x58(0), field_0x60(0), field_0x68(0), field_0x70(0), ids78{g_unk_7101d470e0, g_unk_7101d470e0, g_unk_7101d470e0, g_unk_7101d470e0,
                                         g_unk_7101d470e0},
          field_0xa0(0), field_0xa8(0), field_0xb0(-1), field_0xb8(0), field_0xc0(-1), field_0xc8(0) {}
};
static_assert(sizeof(UnkHeatableValue_b260) == 0xd0, "layout");

__attribute__((flatten)) void fn_710086df48(UnkHeatableValue_0xd0* self) {
    new (reinterpret_cast<UnkHeatableValue_b260*>(self)) UnkHeatableValue_b260;
}

void fn_710086e0ac(UnkHeatableValue_0xd0* self_) {
    UnkHeatableValue_b260* self = reinterpret_cast<UnkHeatableValue_b260*>(self_);
    fn_710086e288(reinterpret_cast<UnkIdRecord_0x28*>(self->ids78));
    self->id48.Reset();
    reinterpret_cast<base::UnkVector<UnkTwoStepId_b260>&>(self->list).Free();
    self->id18.Reset();
    self->id10.Reset();
    self->id0.Reset();
}
