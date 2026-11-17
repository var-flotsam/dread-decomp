// build-flags: no-strict-aliasing
// Dropped-item manager (unit 2562, CLogicDroppedItemManager::CTunableDroppableBillboard): builds a
// record of 20 zeroed entries, each with its byte offset and a scale of 1, and registers it under the third
// billboard id. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU83.h"
#include "types.h"
#include "game/UnkTU2561.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;


struct UnkRecord_b218;

// The record's id: assigned and released with the count decremented and tested in one step.
struct UnkRecordId : base::global::UnkStrIdBase {
    using UnkStrIdBase::UnkStrIdBase;

    UnkRecordId& operator=(const CStrId& other) {
        UnkStringData* old = data;
        UnkStringData* d = other.data;
        if (old != d) {
            UnkStringPool* pool = g_unk_7101d47108;
            if (d != &pool->empty)
                ++d->ref_count;
            if (old != &pool->empty && --old->ref_count == 0)
                fn_71000803a4(pool, old);
            data = d;
        }
        return *this;
    }

    ~UnkRecordId() {
        if (UnkStringPool* pool = g_unk_7101d47108) {
            if (data && data != &pool->empty && --data->ref_count == 0) {
                fn_71000803a4(pool, data);
                pool = g_unk_7101d47108;
            }
            data = &pool->empty;
        }
    }
};

// One entry of the record, three words (moved word by word when the array grows).
struct UnkRecordOffset {
    u32 words[3];

    UnkRecordOffset(const UnkRecordOffset& other) {
        words[0] = other.words[0];
        words[1] = other.words[1];
        words[2] = other.words[2];
    }
};

// The record fn_7100e1cfec copies into the billboard's list (the layout of UnkRecord_b218).
struct UnkBillboardRecord {
    UnkRecordId id;                         // 0x0
    u8 field_0x8;                           // 0x8
    CRntVector<UnkRecordOffset> offsets;    // 0x10
    CRntVector<f32> scales;                 // 0x38
    CRntVector<u8> byte_offsets;            // 0x60
    CRntVector<u32> field_0x88;             // 0x88

    UnkBillboardRecord()
        : id(g_unk_710162e2ed, true), field_0x8(0), offsets(0xf), scales(0xf), byte_offsets(0xf), field_0x88(0xf) {}
    ~UnkBillboardRecord() {
        field_0x88.Free();
        byte_offsets.Free();
        scales.Free();
        offsets.Free();
    }
};
static_assert(sizeof(UnkBillboardRecord) == 0xb0, "record size");

static inline void* Singleton() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(p);
        g_unk_71726da048 = p;
    }
    return p;
}

void fn_7100e22a74() {
    UnkBillboardRecord record;
    record.byte_offsets.Reserve(20);
    record.offsets.Reserve(20);
    record.scales.Reserve(20);
    record.id = g_unk_71727c1080;
    record.field_0x8 = 1;
    for (u32 i = 0; i < 20; ++i) {
        UnkRecordOffset* offset = reinterpret_cast<UnkRecordOffset*>(fn_71000cc374(&record.offsets, i));
        offset->words[0] = 0;
        offset->words[1] = 0;
        offset->words[2] = 0;
        record.byte_offsets.GrowAt(i) = i * sizeof(UnkRecordOffset);
        record.scales.GrowAt(i) = 1.0f;
    }
    fn_7100e1cfec(Singleton(), &g_unk_71727c1088[2], reinterpret_cast<UnkRecord_b218*>(&record));
}
