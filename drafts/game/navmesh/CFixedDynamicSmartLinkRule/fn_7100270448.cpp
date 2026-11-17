// navmesh::CFixedDynamicSmartLinkRule's unit (tools/tu_map.py unit 354): fn_7100270448 applies the rule to a
// link between two points: when they are the rule's own (+0x20, +0x28) it runs fn_710026f790 forward; when
// they are the rule's mirrored, and the rule goes both ways (+0x81), in reverse. The cell filter (+0x18) and
// the actor's entry for the rule's name (+0x88) gate it. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkNavMeshCell.h"
#include "game/UnkTU367.h"
#include "types.h"

using base::math::CVector2D;

struct UnkSmartLinkRule_ovb0112;
struct UnkAnimActionSetsOwner;
void* fn_7101176d34(UnkAnimActionSetsOwner* self, u64 key);
void fn_710026f790(UnkSmartLinkRule_ovb0112* self, void* target, UnkNavMesh* mesh, u32 a, u16 cell, u32 b,
                   bool reverse, void* c, void* d, u32 e, u32 f);

struct UnkFixedRuleFilter_l270448 {
    u8 pad_0x0[0x64];
    u32 mask;  // 0x64
};

struct UnkFixedRule_l270448 {
    u8 pad_0x0[0x18];
    UnkFixedRuleFilter_l270448* filter;  // 0x18
    CVector2D start;  // 0x20
    CVector2D end;    // 0x28
    u8 pad_0x30[0x44];
    u32 value;  // 0x74
    u8 pad_0x78[0x9];
    bool both_ways;  // 0x81
    u8 pad_0x82[0x6];
    base::global::UnkStrId name;  // 0x88
};
static_assert(__builtin_offsetof(UnkFixedRule_l270448, name) == 0x88, "layout");

struct UnkFixedRuleActor_l270448 {
    u8 pad_0x0[0x220];
    UnkAnimActionSetsOwner* animations;  // 0x220
};

// Whether `v` is within a thousandth of `t`.
static inline bool Near_l270448(f32 t, f32 v) {
    return t - 0.001f <= v && t + 0.001f >= v;
}

static inline bool Near_l270448(const CVector2D& t, const CVector2D& v) {
    return Near_l270448(t.x, v.x) && Near_l270448(t.y, v.y);
}

static inline CVector2D Negated_l270448(const CVector2D& v) {
    CVector2D r;
    r.x = -v.x;
    r.y = -v.y;
    return r;
}

static inline CVector2D MirroredX_l270448(const CVector2D& v) {
    CVector2D r;
    r.x = -v.x;
    r.y = v.y;
    return r;
}

void fn_7100270448(UnkFixedRule_l270448* self, UnkFixedRuleActor_l270448* actor, UnkNavMesh* mesh, u32 a,
                   u16 cell, u32 b, const CVector2D* from, const CVector2D* to, void* c, void* d, u32 e, u32 f) {
    if (static_cast<u8>(b) == 0xff || cell == 0xffff || !actor || self->name.data == &g_unk_7101d47108->empty)
        return;

    bool reverse = false;
    if (!Near_l270448(self->start, *from)) {
        if (!Near_l270448(MirroredX_l270448(self->start), *from) ||
            !Near_l270448(MirroredX_l270448(self->end), *to))
            return;
        reverse = true;
    } else if (!Near_l270448(self->end, *to)) {
        if (!Near_l270448(Negated_l270448(self->end), *to))
            return;
        reverse = true;
    }
    if (reverse && !self->both_ways)
        return;

    if (UnkFixedRuleFilter_l270448* filter = self->filter) {
        u32 mask = filter->mask;
        if (mask != 0) {
            u32 kind = fn_71002b009c(fn_710027a328(mesh, cell)) & mask;
            if (self->value != kind)
                return;
        }
    }
    UnkAnimActionSetsOwner* animations = actor->animations;
    if (!animations)
        return;
    void* entry = fn_7101176d34(animations, self->name.data->Key());
    if (!entry)
        return;
    fn_710026f790(reinterpret_cast<UnkSmartLinkRule_ovb0112*>(self), entry, mesh, a, cell, b, reverse, c, d, e,
                  f);
}
