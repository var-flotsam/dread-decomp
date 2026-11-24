// Dropped-item manager (unit 2562, hint CLogicDroppedItemManager::CTunableDroppableBillboard): registers
// a twenty-key billboard record with the billboard manager (fn_7100e1cfec): zero offsets, a scale that
// grows from 1 to 2 and an alpha that falls from 1 to 0. Names and layouts are placeholders.
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
struct UnkRecord_e237f4 {
    CStrId id;
    bool field_0x8;
    CRntVector<UnkVec3f> offsets;  // 0x10 (zero offsets)
    CRntVector<f32> scales;                // 0x38
    CRntVector<u8> flags;                  // 0x60
    CRntVector<f32> alphas;                // 0x88

    UnkRecord_e237f4()
        : id(g_unk_710162e2ed, true), field_0x8(false), offsets(0xf), scales(0xf), flags(0xf), alphas(0xf) {}
    ~UnkRecord_e237f4() {
        alphas.Free();
        flags.Free();
        scales.Free();
        offsets.Free();
    }
};
static_assert(sizeof(UnkRecord_e237f4) == 0xb0, "record layout");

static inline void* Singleton() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        ConstructBillboardManager(p);
        g_unk_71726da048 = p;
    }
    return p;
}

void fn_7100e237f4() {
    UnkRecord_e237f4 record;
    record.flags.Reserve(20);
    record.offsets.Reserve(20);
    record.scales.Reserve(20);
    record.alphas.Reserve(20);
    record.id = g_unk_71727c1080;
    record.field_0x8 = true;
    for (u32 i = 0; i < 20; ++i) {
        f32 t = i / 20.0f;
        *reinterpret_cast<UnkVec3f*>(fn_71000cc374(&record.offsets, i)) = UnkVec3f(0.0f, 0.0f, 0.0f);
        record.flags.GrowAt(i) = 0;
        record.scales.GrowAt(i) = t + 1.0f;
        record.alphas.GrowAt(i) = 1.0f - t;
    }
    fn_7100e1cfec(Singleton(), &g_unk_71727c1088[4], reinterpret_cast<UnkRecord_b218*>(&record));
}
