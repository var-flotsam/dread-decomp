// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): the destructor of the result a scenario
// load reports (src/game/CGameManager.cpp's UnkLoadResult): finalizes its mutex and frees its entries and names.
// Names and layouts are placeholders.
#include <nn/os.h>

#include "base/UnkDefaultAllocator.h"
#include "base/UnkVtableList.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;

// A list of pointers (vtable g_unk_7101aa9ac0), its nodes from `allocator`; emptied when destroyed.
struct UnkResultListNode_de8ac4 {
    void* value;
    UnkResultListNode_de8ac4* next;
};
struct UnkResultList_de8ac4 : base::UnkVtableList<UnkResultListNode_de8ac4> {
    ~UnkResultList_de8ac4() {
        vtable = reinterpret_cast<void**>(g_unk_7101aa9ac0 + 0x10);
        Clear();
    }
};
static_assert(sizeof(UnkResultList_de8ac4) == 0x30, "layout");

// One entry of the result (0x208 bytes).
struct UnkResultEntry_de8ac4 {
    CRntString field_0x0;
    CRntString field_0x30;
    CRntString field_0x60;
    CRntString field_0x90;
    CRntString field_0xc0;
    UnkResultList_de8ac4 field_0xf0;
    CRntString field_0x120;
    u8 pad_0x150[0x88];
    CRntString field_0x1d8;
};
static_assert(sizeof(UnkResultEntry_de8ac4) == 0x208, "layout");

struct UnkLoadResult_de8ac4 {
    u8 pad_0x0[0x18];
    CRntVector<CRntString> field_0x18;
    u8 pad_0x40[0x18];
    CRntVector<UnkResultEntry_de8ac4> field_0x58;
    u8 pad_0x80[0x8];
    nn::os::MutexType field_0x88;
};
static_assert(__builtin_offsetof(UnkLoadResult_de8ac4, field_0x88) == 0x88, "layout");

void fn_7100de8ac4(UnkLoadResult_de8ac4* self) {
    fn_7100066918(&self->field_0x88);
    self->field_0x58.Free();
    self->field_0x18.Free();
}
