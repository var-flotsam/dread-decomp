// Unit 32 of tools/tu_map.py (0x71000255c4-0x7100027774, see src/base/UnkUnit32/UnkTransformNode.cpp):
// the transform record every node embeds at +0x28 (base/UnkInitRecord_0x5c.h) and the node's
// constructor. Names and layouts are placeholders.
#include "base/UnkInitRecord_0x5c.h"
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkOneVector3.h"
#include "base/math/UnkZeroVector3.h"
#include "game/UnkTU25.h"
#include "game/UnkTU32.h"
#include "game/UnkXformNode.h"
#include "types.h"

// A zero whose constructor isn't constexpr, so the static is guarded.
struct UnkXformStaticZero32 {
    u32 value;
    UnkXformStaticZero32() : value(0) {}
};

inline const UnkXformStaticZero32& UnkXformStaticZero() {
    static UnkXformStaticZero32 s_value asm("g_unk_7101d4ec40");
    return s_value;
}

// The complement of that zero's low four bits.
inline u32 UnkXformDefaultFlags() {
    static u32 s_mask asm("g_unk_7101d4ec30") = ~UnkXformStaticZero().value & 0xf;
    return s_mask;
}

struct UnkXformRecord32 {
    f32 m[16];
    f32 angle[3];  // 0x40
    f32 scale[3];  // 0x4c
    u32 flags;     // 0x58
};
static_assert(sizeof(UnkXformRecord32) == 0x5c, "UnkXformRecord32 layout");

void fn_7100025624(void* self) {
    UnkXformRecord32* r = static_cast<UnkXformRecord32*>(self);
    for (int i = 0; i < 16; i++)
        r->m[i] = 0.0f;
    r->m[15] = 1.0f;
    r->m[10] = 1.0f;
    r->m[5] = 1.0f;
    r->m[0] = 1.0f;
    r->angle[0] = 0.0f;
    r->angle[1] = 0.0f;
    r->angle[2] = 0.0f;
    r->scale[0] = 1.0f;
    r->scale[1] = 1.0f;
    r->scale[2] = 1.0f;
    r->flags = UnkXformDefaultFlags();
}


void fn_7100025a98(void* self) {
    UnkXform* r = static_cast<UnkXform*>(self);
    for (int i = 0; i < 16; i++)
        r->m[i / 4][i % 4] = 0.0f;
    r->m[3][3] = 1.0f;
    r->m[2][2] = 1.0f;
    r->m[1][1] = 1.0f;
    r->m[0][0] = 1.0f;
    r->angle = *reinterpret_cast<const UnkVec3f*>(&g_unk_7101647790);
    r->scale = *reinterpret_cast<const UnkVec3f*>(&g_unk_71016477a8);
    r->flags = UnkXformDefaultFlags();
}

UnkStrIdPathResource* fn_7100027118(UnkStrIdPathResource* self, UnkStrIdPathResource* other) {
    self->ids.id = other->ids.id;
    self->ids.path = other->ids.path;
    UnkCountedResource* res = other->resource;
    UnkCountedResource* old = self->resource;
    if (old != res) {
        if (res)
            ++res->ref_count;
        old = self->resource;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        self->resource = res;
    }
    return self;
}

// As src/base/UnkStrIdPathResource_Ctor_b45.cpp declares the builder.
struct UnkPathIds;
struct UnkInlineString;
struct UnkIdPair;

// The ids the builder fills in place (the members start out unset), destroyed in reverse order.
struct UnkBuiltPathIds_ovb0177 {
    union {
        base::global::CStrId id;
    };
    union {
        base::UnkStrIdMemberOf<g_unk_7101d47200> path;
    };

    explicit UnkBuiltPathIds_ovb0177(const UnkInlineString* source) {
        fn_710001f074(reinterpret_cast<UnkPathIds*>(this), source);
    }
    ~UnkBuiltPathIds_ovb0177() {}
    void Destroy() {
        path.~UnkStrIdMemberOf();
        id.~CStrId();
    }
};

UnkStrIdPathResource* fn_71000272cc(UnkStrIdPathResource* self, const UnkInlineString* other) {
    if (reinterpret_cast<const void*>(other) != reinterpret_cast<const void*>(self)) {
        UnkBuiltPathIds_ovb0177 ids(other);
        union Raw {
            UnkStrIdPathResource r;
            Raw() {}
            ~Raw() {}
        } raw;
        UnkStrIdPathResource& tmp = raw.r;
        new (&tmp.ids.id) base::global::CStrId(g_unk_7101d470e0);
        new (&tmp.ids.path) base::global::CFilePathStrId(g_unk_7101d470e8);
        tmp.resource = nullptr;
        fn_71000267f8(&tmp, reinterpret_cast<const UnkStrIdPathPair&>(ids));
        ids.Destroy();
        fn_7100027118(self, &tmp);
        fn_7100026f70(reinterpret_cast<UnkIdPair*>(&tmp));
    }
    return self;
}

// A resource as the record that holds it sees its ids: the string id at +8 and the path id at +0x10.
struct UnkResourceIds_ovb0177 {
    u8 pad_0x0[8];
    base::global::CStrId id;
    base::global::CFilePathStrId path;
};

void fn_71000269c4(UnkStrIdPathResource* self, UnkCountedResource* resource) {
    UnkCountedResource* old = self->resource;
    if (old != resource) {
        if (resource)
            ++resource->ref_count;
        old = self->resource;
        if (old && --old->ref_count == 0)
            fn_710001737c(old);
        self->resource = resource;
    }
    const UnkResourceIds_ovb0177* ids = reinterpret_cast<const UnkResourceIds_ovb0177*>(resource);
    union Temps {
        struct {
            base::global::CStrId id;
            base::global::CFilePathStrId path;
        };
        Temps() {}
        ~Temps() {}
    } t;
    if (resource) {
        new (&t.id) base::global::CStrId(ids->id);
        new (&t.path) base::global::CFilePathStrId(ids->path);
    } else {
        new (&t.id) base::global::CStrId(g_unk_7101d470e0);
        new (&t.path) base::global::CFilePathStrId(g_unk_7101d470e8);
    }
    self->ids.id = reinterpret_cast<const base::UnkStrIdMember&>(t.id);
    self->ids.path = reinterpret_cast<const base::UnkStrIdMemberOf<g_unk_7101d47200>&>(t.path);
    t.path.~CFilePathStrId();
    t.id.~CStrId();
}
