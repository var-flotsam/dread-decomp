// base::tunable::CTunable's unit: erases one element of an array of record vectors with a string id
// per element alongside: the element is freed and the last one moved into its place. Returns the
// element's slot. Names and layouts are placeholders.
#include <new>

#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

struct UnkIdsString;
void fn_7100133be4(UnkIdsString* self);

// Three string ids and a string with inline storage (destroyed by fn_7100133be4).
struct UnkTunableRecord_l133780 {
    u8 pad_0x0[0x160];
    ~UnkTunableRecord_l133780() { fn_7100133be4(reinterpret_cast<UnkIdsString*>(this)); }
};
using UnkTunableRecordVec_l133780 = base::global::CRntVector<UnkTunableRecord_l133780>;

struct UnkRecordVecArray_l133780 {
    UnkTunableRecordVec_l133780* records;
    base::global::CStrId* ids;
    u32 count;
    u32 capacity;
};

void* fn_7100133780(char* array, void* it) {
    UnkRecordVecArray_l133780* self = reinterpret_cast<UnkRecordVecArray_l133780*>(array);
    u32 index = static_cast<UnkTunableRecordVec_l133780*>(it) - self->records;
    self->records[index].Free();
    self->ids[index].~CStrId();
    if (self->count - 1 != index) {
        new (&self->records[index])
            UnkTunableRecordVec_l133780(static_cast<UnkTunableRecordVec_l133780&&>(self->records[self->count - 1]));
        self->records[self->count - 1].Free();
        base::global::CStrId::MoveConstructReloading(&self->ids[index], self->ids[self->count - 1]);
        self->ids[self->count - 1].~CStrId();
    }
    --self->count;
    return &self->records[index];
}
