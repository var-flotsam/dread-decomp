// The CMinimapData unit (unit 2563): the move assignment of a moving magnet surface record
// (CMinimapData::SMovingMagnetSurfaceData, include/game/CMinimapData.h), member by member.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

// A node of a path line: destroyed through its vtable (slot 1).
struct UnkPathNode_le51258 {
    virtual void vf_0x0();
    virtual ~UnkPathNode_le51258();
    u8 pad_0x8[0x18];
};
static_assert(sizeof(UnkPathNode_le51258) == 0x20, "UnkPathNode_le51258 layout");

// A path line (an SLogicSubPath by its reflection): a vtable, its nodes and a flag.
struct UnkPathLine_le51258 {
    void** vtable;
    CRntVector<UnkPathNode_le51258> nodes;
    u8 field_0x30;
    u8 pad_0x31[0x1f];

    UnkPathLine_le51258& operator=(UnkPathLine_le51258&& other) {
        nodes = static_cast<CRntVector<UnkPathNode_le51258>&&>(other.nodes);
        field_0x30 = other.field_0x30;
        return *this;
    }
};
static_assert(sizeof(UnkPathLine_le51258) == 0x50, "UnkPathLine_le51258 layout");

// Two arrays and four words, moved member by member.
struct UnkArrayPairRecord_le53cac {
    base::global::CRntVector<u32> first;
    base::global::CRntVector<u32> second;
    u32 field_0x50;
    u32 field_0x54;
    u32 field_0x58;
    u32 field_0x5c;

    UnkArrayPairRecord_le53cac& operator=(UnkArrayPairRecord_le53cac&& other) {
        first = static_cast<base::global::CRntVector<u32>&&>(other.first);
        second = static_cast<base::global::CRntVector<u32>&&>(other.second);
        field_0x50 = other.field_0x50;
        field_0x54 = other.field_0x54;
        field_0x58 = other.field_0x58;
        field_0x5c = other.field_0x5c;
        return *this;
    }
};
static_assert(sizeof(UnkArrayPairRecord_le53cac) == 0x60, "UnkArrayPairRecord_le53cac layout");

// The record as its move assignment sees it.
struct UnkMagnetSurface_le51258 {
    CStrId sMoveEntityID;
    u32 iEntityType;
    UnkPathLine_le51258 oPathLine;
    u8 field_0x60;
    u32 field_0x64;
    u32 field_0x68;
    u32 field_0x6c;
    u32 field_0x70;
    UnkArrayPairRecord_le53cac field_0x78;
    UnkArrayPairRecord_le53cac field_0xd8;

};
static_assert(sizeof(UnkMagnetSurface_le51258) == 0x138, "UnkMagnetSurface_le51258 layout");

// The record's move assignment: the id is released and the other's taken, the arrays taken over.
UnkMagnetSurface_le51258* fn_7100e51258(void* dst, const void* src) {
    auto* self = static_cast<UnkMagnetSurface_le51258*>(dst);
    auto* other = static_cast<UnkMagnetSurface_le51258*>(const_cast<void*>(src));
    self->sMoveEntityID.ReleaseAndTake(other->sMoveEntityID);
    self->iEntityType = other->iEntityType;
    self->oPathLine = static_cast<UnkPathLine_le51258&&>(other->oPathLine);
    self->field_0x60 = other->field_0x60;
    self->field_0x64 = other->field_0x64;
    self->field_0x68 = other->field_0x68;
    self->field_0x6c = other->field_0x6c;
    self->field_0x70 = other->field_0x70;
    self->field_0x78 = static_cast<UnkArrayPairRecord_le53cac&&>(other->field_0x78);
    self->field_0xd8 = static_cast<UnkArrayPairRecord_le53cac&&>(other->field_0xd8);
    return self;
}
