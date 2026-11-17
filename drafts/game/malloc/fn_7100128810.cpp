// Draft (attempt 2 form; the original copies each contact word by word, this one as a block).
// malloc's unit (tools/tu_map.py unit 110): an oriented box against another, edge by edge, with a
// containment fallback. Names and layouts are placeholders.
#include "base/math/CVector.h"
#include "base/math/UnkSegment2D.h"
#include "base/spatial/COBox2D.h"
#include "game/malloc.h"
#include "types.h"

using base::math::CVector2D;
using base::math::CVector3D;
using base::spatial::COBox2D;

// What the 2D tests fill on a hit (the layout of src/game/malloc/fn_710012be70.cpp's contact): the
// point, the normal of the first shape, the normal of the second, and which of them are set.
struct UnkContact2D {
    CVector3D point;
    CVector3D normal_a;
    CVector3D normal_b;
    f32 param;  // where along the moving shape's path (0x200)
    u32 flags;  // 8 point, 0x10 normal_b, 0x20 normal_a, 0x200 param

    void SetPoint(const CVector3D& p) {
        point = p;
        flags |= 8;
    }
    void SetNormalA(const CVector3D& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = n.z;
        flags |= 0x20;
    }
    void SetNormalB(const CVector3D& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = n.z;
        flags |= 0x10;
    }
    void SetNormalA(const UnkVec2Result& n) {
        normal_a.x = n.x;
        normal_a.y = n.y;
        normal_a.z = 0.0f;
        flags |= 0x20;
    }
    void SetNormalB(const UnkVec2Result& n) {
        normal_b.x = n.x;
        normal_b.y = n.y;
        normal_b.z = 0.0f;
        flags |= 0x10;
    }
};

// A list of contacts, filled by the tests that can report several.
struct UnkContactList2D {
    u32 count;
    UnkContact2D items[1];  // the real capacity is unknown

    // Appends a cleared contact and returns its index (the count itself once it reaches ~0).
    u32 Add() {
        u32 i = count;
        if (i != 0xFFFFFFFF) {
            items[i] = UnkContact2D();
            count = i + 1;
        }
        return i;
    }
};

// A contact list on the stack with room for two contacts.
struct UnkContactList2D_Pair {
    u32 count;
    UnkContact2D items[2];
};

UnkVec2Result fn_7100110420(const COBox2D* self, u32 index);
bool fn_7100110794(const COBox2D* self, const CVector2D& point);

static inline const CVector2D& AsVec(const UnkVec2Result& v) {
    return reinterpret_cast<const CVector2D&>(v);
}

// The side of `b` from corner `from` to corner `to` against `a`: its contacts are appended to `list`.
static inline bool AddSideContacts(const COBox2D* a, const COBox2D* b, u32 from, u32 to,
                                   UnkContactList2D* list) {
    UnkVec2Result start = fn_7100110420(b, from);
    UnkContactList2D_Pair found;
    found.count = 0;
    UnkSegment2D side(AsVec(start), AsVec(fn_7100110420(b, to)));
    if (!fn_7100127bb0(reinterpret_cast<const UnkOrientedBox2D*>(a), &side,
                       reinterpret_cast<UnkContactList2D*>(&found), false))
        return false;
    for (u32 k = 0; k < found.count; k++)
        list->items[list->Add()] = found.items[k];
    return true;
}

// Each side of `b` is tested against `a` and its contacts appended to `list`. With no contact and
// `inside` set, a box holding the other's centre gives one contact at that centre.
bool fn_7100128810(const COBox2D* a, const COBox2D* b, UnkContactList2D* list, bool inside) {
    bool hit = false;
    if (AddSideContacts(a, b, 0, 1, list))
        hit = true;
    if (AddSideContacts(a, b, 1, 2, list))
        hit = true;
    if (AddSideContacts(a, b, 2, 3, list))
        hit = true;
    if (AddSideContacts(a, b, 3, 0, list))
        hit = true;
    if (list->count == 0 && inside) {
        const COBox2D* holder;
        if (fn_7100110794(a, b->center))
            holder = b;
        else if (fn_7100110794(b, a->center))
            holder = a;
        else
            return list->count != 0;
        if (list->count == 0)
            list->Add();
        list->items[0].SetPoint(CVector3D{holder->center.x, holder->center.y, 0.0f});
        return list->count != 0;
    }
    return hit;
}
