// Dropped-item manager (unit 2562, hint CLogicDroppedItemManager::CTunableDroppableBillboard): registers the
// drops' relax movement with the billboard manager (fn_7100e1cfec): one upward offset per animation frame,
// following a full sine wave scaled by the tunable's size. Names and layouts are placeholders.
#include <math.h>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/CLogicDroppedItemManager/CTunableDroppableBillboard.h"
#include "game/UnkDroppableTunable.h"
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
struct UnkRecord_e22744 {
    CStrId id;
    bool field_0x8;
    CRntVector<UnkVec3f> offsets;  // 0x10
    CRntVector<f32> scales;        // 0x38
    CRntVector<u8> flags;          // 0x60
    CRntVector<f32> alphas;        // 0x88

    UnkRecord_e22744()
        : id(g_unk_710162e2ed, true), field_0x8(false), offsets(0xf), scales(0xf), flags(0xf), alphas(0xf) {}
    ~UnkRecord_e22744() {
        alphas.Free();
        flags.Free();
        scales.Free();
        offsets.Free();
    }
};
static_assert(sizeof(UnkRecord_e22744) == 0xb0, "record layout");

static inline void* Singleton() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        ConstructBillboardManager(p);
        g_unk_71726da048 = p;
    }
    return p;
}

static inline UnkVec3f Scaled(const UnkVec3f& v, f32 k) {
    return UnkVec3f(v.x * k, v.y * k, v.z * k);
}

void fn_7100e22744() {
    UnkRecord_e22744 record;
    u32 frames = g_unk_71727c10c8.iDropsRelaxMovementAnimationFrames;
    record.offsets.Reserve(frames);
    record.id = g_unk_71727c1080;
    record.field_0x8 = true;
    for (u32 i = 0; i < frames; ++i) {
        f32 wave = sinf((f32)i / (f32)frames * 6.2831855f);
        *reinterpret_cast<UnkVec3f*>(fn_71000cc374(&record.offsets, i)) =
            Scaled(Scaled(UnkVec3f(0.0f, 1.0f, 0.0f), wave), g_unk_71727c10c8.fDropsRelaxMovementSize);
    }
    fn_7100e1cfec(Singleton(), &g_unk_71727c1088[1], reinterpret_cast<UnkRecord_b218*>(&record));
}
