// Dropped-item manager (unit 2562, hint CLogicDroppedItemManager::CTunableDroppableBillboard): registers
// a fifty-key billboard record with the billboard manager (fn_7100e1cfec): zero offsets, a flag byte
// that ramps up, a scale that grows from 0.75 and an alpha that falls from 1 to 0. Names and layouts are
// placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkRecord_b218;

extern CStrId g_unk_71727c1080;
extern CStrId g_unk_71727c1088[];  // the billboards' default ids

void fn_7100e1cfec(void* self_, CStrId* key, UnkRecord_b218* record);

// A key's offset: three words, moved one by one when the array grows.
struct UnkOffset_e23fa8 {
    u32 v[3];

    UnkOffset_e23fa8() : v{0, 0, 0} {}
    UnkOffset_e23fa8(const UnkOffset_e23fa8& other) {
        v[0] = other.v[0];
        v[1] = other.v[1];
        v[2] = other.v[2];
    }
};

// The record fn_7100e1cfec copies in: an id, a flag and one value per key in four arrays.
struct UnkRecord_e23fa8 {
    CStrId id;
    bool field_0x8;
    CRntVector<UnkOffset_e23fa8> offsets;  // 0x10
    CRntVector<f32> scales;                // 0x38
    CRntVector<u8> flags;                  // 0x60
    CRntVector<f32> alphas;                // 0x88

    UnkRecord_e23fa8()
        : id(g_unk_710162e2ed, true), field_0x8(false), offsets(0xf), scales(0xf), flags(0xf), alphas(0xf) {}
    ~UnkRecord_e23fa8() {
        alphas.Free();
        flags.Free();
        scales.Free();
        offsets.Free();
    }
};
static_assert(sizeof(UnkRecord_e23fa8) == 0xb0, "record layout");

static inline void* Singleton() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(p);
        g_unk_71726da048 = p;
    }
    return p;
}

void fn_7100e23fa8() {
    UnkRecord_e23fa8 record;
    record.flags.Reserve(50);
    record.offsets.Reserve(50);
    record.scales.Reserve(50);
    record.alphas.Reserve(50);
    record.id = g_unk_71727c1080;
    record.field_0x8 = true;
    for (u32 i = 0; i < 50; ++i) {
        f32 key = i;
        f32 t = key / 50.0f;
        *reinterpret_cast<UnkOffset_e23fa8*>(fn_71000cc374(&record.offsets, i)) = UnkOffset_e23fa8();
        record.flags.GrowAt(i) = key * 2.55f;
        record.scales.GrowAt(i) = t + 0.75f;
        record.alphas.GrowAt(i) = 1.0f - t;
    }
    fn_7100e1cfec(Singleton(), &g_unk_71727c1088[5], reinterpret_cast<UnkRecord_b218*>(&record));
}
