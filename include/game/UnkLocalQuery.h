#pragma once

#include "base/math/CVector.h"
#include "game/logic/collision/UnkColliderResult.h"
#include "types.h"

struct UnkPolygonHit;

// The query a polygon collection shape (type 0x10) runs for a shape placed in its frame: the
// translation to report the points at, the collection's data and the sink. Same type as in
// src/game_hi/fn_7100d56604.cpp (vtable at 0x7101aa8870).
// The query a polygon collection shape runs (same type as in src/game/UnkCollisionContacts.cpp and
// src/game_hi/fn_7100d56604.cpp, vtable at 0x7101aa8870).
struct UnkLocalQuery {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual UnkColliderResult vf_0x10(UnkPolygonHit hit);
    base::math::CVector3D offset;
    u16 field_0x14;
    u8 pad_0x16[0x2];
    const void* field_0x18;
    void* field_0x20;
    UnkLocalQuery(const base::math::CVector3D& o, const void* p, void* a)
        : offset(o), field_0x14(0), field_0x18(p), field_0x20(a) {}
};
