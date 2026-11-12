// Unit 2510: queries a collision tree along the segment between two 3D points (flattened to 2D) and,
// when the query reports a hit, copies the first hit record out. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2517.h"
#include "game/UnkTU365.h"
#include "types.h"

struct UnkSegmentShape {
    u8 data[0xc0];
};

struct UnkRefCounted_ld5b268 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    s32 ref_count;  // 0x8
};

struct UnkAssignedSlot {
    UnkRefCounted_ld5b268* ref;
};

struct UnkHitOut_ld5b268 {
    UnkAssignedSlot first;
    UnkAssignedSlot second;
    s32 words[13];
};

struct UnkTreeHit_ld5b268 {
    void** vtable;
    UnkAssignedSlot first;
    UnkAssignedSlot second;
    s32 words[13];
    u32 field_0x4c;
    u8 flag;
    u64 field_0x58;
    u32 field_0x60;
    u64 field_0x68;

    explicit UnkTreeHit_ld5b268(const base::math::CVector3D* origin) {
        vtable = g_unk_7101a20fa8 + 2;
        first.ref = nullptr;
        second.ref = nullptr;
        words[0] = -1;
        for (int i = 1; i < 13; i++)
            words[i] = 0;
        flag = 0;
        field_0x68 = 0;
        field_0x58 = *reinterpret_cast<const u64*>(origin);
        field_0x60 = reinterpret_cast<const u32*>(origin)[2];
    }
    ~UnkTreeHit_ld5b268() {
        vtable = g_unk_7101a20ed8 + 2;
        Release(second.ref);
        Release(first.ref);
    }
    static void Release(UnkRefCounted_ld5b268* p) {
        if (p && --p->ref_count == 0)
            p->vf_0x10();
    }
};

static inline void SetIdentity(UnkMatrix44* m) {
    *m = UnkMatrix44{};
    m->m[15] = 1.0f;
    m->m[10] = 1.0f;
    m->m[5] = 1.0f;
    m->m[0] = 1.0f;
}

struct UnkMtx44;
struct UnkShape;

bool fn_7100d5b268(UnkTree* tree, u32 mask, const base::math::CVector3D* from, const base::math::CVector3D* to,
                   UnkHitOut_ld5b268* out) {
    UnkSegmentShape shape;
    base::math::CVector2D b;
    {
        base::math::CVector2D a;
        fn_710007b430(&a, from);
        fn_710007b430(&b, to);
        UnkSegment2D seg(a, b);
        fn_7100d693ac(&shape, &seg);
    }
    bool hit;
    {
        UnkTreeHit_ld5b268 result(from);
        UnkMatrix44 identity;
        SetIdentity(&identity);
        hit = fn_7100d5b484(tree, mask, reinterpret_cast<UnkShape*>(&shape),
                            reinterpret_cast<const UnkMtx44*>(&identity), &result, 0);
        if (&result.first != &out->first && out && hit) {
            fn_71002a5d1c(&out->first, &result.first);
            fn_71002a5d1c(&out->second, &result.second);
            for (int i = 0; i < 13; i++)
                out->words[i] = result.words[i];
        }
    }
    fn_7100d69400(&shape);
    return hit;
}
