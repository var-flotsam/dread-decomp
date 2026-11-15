// CPlayerInfo unit: a constructor of the class whose destructor is fn_7100f0f698: sets the vtable, builds the
// string id at +8 and a list of 0x10-byte polymorphic elements at +0x10 (allocator flags 5) with room for twenty.
// Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CPlayerInfo.h"
#include "types.h"

struct UnkPlayerInfoElem_lf0f574 {
    virtual ~UnkPlayerInfoElem_lf0f574();
    void* field_0x8;
    UnkPlayerInfoElem_lf0f574(UnkPlayerInfoElem_lf0f574&& other) : field_0x8(other.field_0x8) {}
};

struct UnkPlayerInfo_lf0f574 {
    void** vtable;
    base::global::CStrId field_0x8;
    base::global::CRntVector<UnkPlayerInfoElem_lf0f574> field_0x10;
};

void fn_7100f0f574(UnkPlayerInfo_lf0f574* self) {
    self->vtable = g_unk_7101caa4b0 + 2;
    new (&self->field_0x8) base::global::CStrId(g_unk_710162e2ed, true);
    new (&self->field_0x10) base::global::CRntVector<UnkPlayerInfoElem_lf0f574>(5);
    self->field_0x10.Reserve(20);
}
