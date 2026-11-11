// Unit 32 of tools/tu_map.py (0x71000255c4-0x7100027774, see src/base/UnkUnit32/UnkTransformNode.cpp):
// the transform record every node embeds at +0x28 (base/UnkInitRecord_0x5c.h) and the node's
// constructor. Names and layouts are placeholders.
#include "base/UnkStrIdPathPair.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/math/UnkZeroVector3.h"
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

extern const base::math::CVector3D g_unk_7101647790;
extern const base::math::CVector3D g_unk_71016477a8;

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
void fn_710001f074(UnkPathIds* out, const UnkInlineString* path);
void fn_7100026f70(UnkIdPair* pair);

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

// ---- re-parenting a node ----

struct UnkTransformOwner32 {
    void** vtable;
};

struct UnkTransformNode {
    UnkTransformNode* parent;  // 0x0
    UnkTransformNode* first;   // 0x8
    UnkTransformNode* prev;    // 0x10
    UnkTransformNode* next;    // 0x18
    UnkTransformOwner32* owner;  // 0x20
    UnkXform xform;            // 0x28
    u32 local[16];             // 0x84
    s32 slot;                  // 0xc4
    s32 count;                 // 0xc8
    bool attached;             // 0xcc
    u8 field_0xcd;
    u8 field_0xce;
};
static_assert(__builtin_offsetof(UnkTransformNode, slot) == 0xc4, "UnkTransformNode layout");

bool fn_7100025bd0(UnkTransformNode* node);
void fn_7100025d10(void* node);
bool fn_7100076a5c(const UnkMatrix44* m);

// Links are written as raw words: the original reloads the node's own links after each one.
static inline void StoreLink32(UnkTransformNode** link, UnkTransformNode* value) {
    __builtin_memcpy(link, &value, sizeof(value));
}

static inline void Clear32(UnkTransformNode* node) {
    node->parent = nullptr;
    node->attached = false;
    node->prev = nullptr;
    node->next = nullptr;
    node->slot = -1;
}

struct UnkLinkTarget127_b66a1;

bool fn_710002589c(void* link, UnkLinkTarget127_b66a1* target, u32 mode, void* extra) {
    UnkTransformNode* self = static_cast<UnkTransformNode*>(link);
    UnkTransformNode* parent = reinterpret_cast<UnkTransformNode*>(target);
    s32 slot = mode;
    if (self->parent == parent)
        return false;
    if (self->parent) {
        UnkTransformNode* prev = self->prev;
        UnkTransformNode* next = self->next;
        if (prev) {
            StoreLink32(&prev->next, next);
            if (UnkTransformNode* after = self->next) {
                UnkTransformNode* before = self->prev;
                StoreLink32(&after->prev, before);
            }
        } else {
            if (next)
                StoreLink32(&next->prev, prev);
            UnkTransformNode* first = self->next;
            StoreLink32(&self->parent->first, first);
        }
        fn_7100025a98(&self->xform);
        fn_7100025bd0(self);
        if (self->slot < 0)
            --self->parent->count;
        Clear32(self);
    }
    if (!parent) {
        s32 none = -1;
        self->parent = nullptr;
        self->attached = false;
        self->slot = none;
        self->prev = nullptr;
        self->next = nullptr;
        return true;
    }
    self->parent = parent;
    UnkTransformNode* first = parent->first;
    self->prev = nullptr;
    self->next = first;
    StoreLink32(&parent->first, self);
    if (UnkTransformNode* after = self->next)
        after->prev = self;
    const UnkMatrix44* matrix = static_cast<const UnkMatrix44*>(extra);
    if (matrix) {
        bool identity = fn_7100076a5c(matrix);
        self->attached = !identity;
        if (__builtin_expect(!identity, 0)) {
            UnkCopyMatrix(reinterpret_cast<UnkMatrix44*>(self->local), *matrix);
            self->slot = slot;
        } else {
            self->slot = slot;
        }
    } else {
        self->attached = false;
        self->slot = slot;
    }
    if (slot < 0)
        ++parent->count;
    if (parent->field_0xcd)
        fn_7100025d10(parent);
    return true;
}

