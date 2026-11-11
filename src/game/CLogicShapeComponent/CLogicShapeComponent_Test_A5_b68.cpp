// CLogicShapeComponent's unit: tests whether an object's position (its matrix's translation, +0x44/+0x54/+0x64,
// projected to 2D by fn_710007b430) hits the shape at +0x30, through the lazily created shape tester
// (g_unk_71726a47a0, 8 bytes, built by fn_7100d5134c; fn_7100d572f8 with the zero vector). Names and layouts are
// placeholders.
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU2509.h"
#include "game/UnkTU365.h"
#include "types.h"

struct UnkShapeTester_A5_b68;

// Returned in memory (not trivially copyable); only bit 0 of the first word is read.
struct UnkShapeHit_A5_b68 {
    u32 flags;  // bit 0: hit
    u8 pad_0x4[0xc];
    UnkShapeHit_A5_b68(const UnkShapeHit_A5_b68& other);
};

UnkShapeHit_A5_b68 fn_7100d572f8(UnkShapeTester_A5_b68* tester, void* shape, const base::math::CVector2D* pos,
                                 const base::math::CVector3D* offset, u64 flags);

struct UnkShapeObject_A5_b68 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
};

static inline base::math::CVector3D Position_A5_b68(const UnkShapeObject_A5_b68* obj) {
    base::math::CVector3D r;
    r.x = obj->x;
    r.y = obj->y;
    r.z = obj->z;
    return r;
}

struct UnkLogicShape_A5_b68 {
    u8 pad_0x0[0x30];
    void* shape;
};

static inline UnkShapeTester_A5_b68* GetTester_A5_b68() {
    if (!g_unk_71726a47a0) {
        UnkShapeTester_A5_b68* t = static_cast<UnkShapeTester_A5_b68*>(fn_71000839a0(8, 0x3f));
        fn_7100d5134c(t);
        g_unk_71726a47a0 = t;
    }
    return g_unk_71726a47a0;
}

bool fn_710091444c(UnkLogicShape_A5_b68* self, const UnkShapeObject_A5_b68* obj) {
    if (!obj || !self->shape)
        return false;
    UnkShapeTester_A5_b68* tester = GetTester_A5_b68();
    base::math::CVector2D pos;
    {
        base::math::CVector3D v = Position_A5_b68(obj);
        fn_710007b430(&pos, &v);
    }
    return fn_7100d572f8(tester, self->shape, &pos, &g_unk_710164779c, 0).flags & 1;
}
