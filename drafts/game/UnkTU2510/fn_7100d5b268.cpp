// Unit 2510: queries a collision tree along the segment between two 3D points (flattened to 2D) and,
// when the query reports a hit, copies the hit's track out. Names and layouts are placeholders.
#include "base/UnkMatrix44.h"
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "game/UnkTU2510.h"
#include "game/UnkTU2517.h"
#include "game/UnkTU365.h"
#include "types.h"

// A collision shape (0xc0 bytes) built from a segment (fn_7100d693ac) and destroyed by
// fn_7100d69400.
struct UnkSegmentShape {
    u8 data[0xc0];
};

struct UnkRefCounted_ld5b268 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void Delete();
    s32 ref_count;  // 0x8
};

// A counted reference; assigned by fn_71002a5d1c.
struct UnkHitHandle_ld5b268 {
    UnkRefCounted_ld5b268* ptr;

    UnkHitHandle_ld5b268() : ptr(nullptr) {}
    UnkHitHandle_ld5b268& operator=(const UnkHitHandle_ld5b268& o) {
        fn_71002a5d1c(reinterpret_cast<UnkAssignedSlot*>(this), reinterpret_cast<const UnkAssignedSlot*>(&o));
        return *this;
    }
    ~UnkHitHandle_ld5b268() {
        UnkRefCounted_ld5b268* old = ptr;
        if (old && --old->ref_count == 0)
            old->Delete();
    }
};

// What the query found: two references, an index and twelve words.
struct UnkHitTrack_ld5b268 {
    UnkHitHandle_ld5b268 a;
    UnkHitHandle_ld5b268 b;
    s32 field_0x10;
    u32 field_0x14[12];

    UnkHitTrack_ld5b268() : field_0x10(-1) {
        for (int i = 0; i < 12; i++)
            field_0x14[i] = 0;
    }
    UnkHitTrack_ld5b268& operator=(const UnkHitTrack_ld5b268& o) {
        if (&o != this) {
            a = o.a;
            b = o.b;
            field_0x10 = o.field_0x10;
            for (int i = 0; i < 12; i++)
                field_0x14[i] = o.field_0x14[i];
        }
        return *this;
    }
};

// The query's result object (vtable g_unk_7101a20fa8 while alive, g_unk_7101a20ed8 when destroyed).
struct UnkTreeHit_ld5b268 {
    void** vtable;
    UnkHitTrack_ld5b268 track;  // 0x8
    bool field_0x50;
    u32 field_0x54;  // left unset by the constructor, as in the original
    base::math::CVector3D origin;  // 0x58
    u64 field_0x68;

    explicit UnkTreeHit_ld5b268(const base::math::CVector3D* from)
        : field_0x50(false), origin(*from), field_0x68(0) {
        vtable = g_unk_7101a20fa8 + 2;
    }
    ~UnkTreeHit_ld5b268() { vtable = g_unk_7101a20ed8 + 2; }
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
                   UnkHitTrack_ld5b268* out) {
    UnkSegmentShape shape;
    {
        base::math::CVector2D a;
        fn_710007b430(&a, from);
        base::math::CVector2D b;
        fn_710007b430(&b, to);
        UnkSegment2D seg(a, b);
        fn_7100d693ac(&shape, &seg);
    }
    bool hit;
    {
        UnkTreeHit_ld5b268 result(from);
        UnkMatrix44 identity;
        SetIdentity(&identity);
        hit = QueryCollisionTreeForTransformedShapeOverlaps(tree, mask, reinterpret_cast<UnkShape*>(&shape),
                            reinterpret_cast<const UnkMtx44*>(&identity), &result, 0);
        if (out && hit)
            *out = result.track;
    }
    fn_7100d69400(&shape);
    return hit;
}
