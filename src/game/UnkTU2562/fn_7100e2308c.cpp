// Dropped-item manager (unit 2562, hint CLogicDroppedItemManager::CTunableDroppableBillboard): registers
// a ten-key billboard record with the billboard manager (fn_7100e1cfec): zero offsets and a scale that
// swells over half a sine wave. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU2561.h"
#include "game/UnkTU83.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

struct UnkRecord_b218;

// The record fn_7100e1cfec copies in: an id, a flag and one value per key in four arrays.
struct UnkRecord_e2308c {
    CStrId id;
    bool field_0x8;
    CRntVector<UnkVec3f> offsets;  // 0x10 (zero offsets)
    CRntVector<f32> scales;                // 0x38
    CRntVector<u8> flags;                  // 0x60
    CRntVector<f32> alphas;                // 0x88

    UnkRecord_e2308c()
        : id(g_unk_710162e2ed, true), field_0x8(false), offsets(0xf), scales(0xf), flags(0xf), alphas(0xf) {}
    ~UnkRecord_e2308c() {
        alphas.Free();
        flags.Free();
        scales.Free();
        offsets.Free();
    }
};
static_assert(sizeof(UnkRecord_e2308c) == 0xb0, "record layout");

static inline void* Singleton() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(p);
        g_unk_71726da048 = p;
    }
    return p;
}

void fn_7100e2308c() {
    UnkRecord_e2308c record;
    record.flags.Reserve(10);
    record.offsets.Reserve(10);
    record.scales.Reserve(10);
    record.id = g_unk_71727c1080;
    record.field_0x8 = true;
    for (u32 i = 0; i < 10; ++i) {
        f32 t = i / 10.0f;
        *reinterpret_cast<UnkVec3f*>(fn_71000cc374(&record.offsets, i)) = UnkVec3f(0.0f, 0.0f, 0.0f);
        record.flags.GrowAt(i) = 0;
        record.scales.GrowAt(i) = sinf(t * 3.14159265f) * 0.1f + 1.0f;
        record.alphas.GrowAt(i) = 1.0f;
    }
    fn_7100e1cfec(Singleton(), &g_unk_71727c1088[3], reinterpret_cast<UnkRecord_b218*>(&record));
}
