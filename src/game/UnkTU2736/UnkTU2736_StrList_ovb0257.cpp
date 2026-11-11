// Unit 2736 of tools/tu_map.py (no name hint). fn_71011a4f1c: a destructor of an object made of a list of
// string-id nodes (base part, allocator at +0x20) and an embedded list of strings (vtable at +0x30,
// allocator at +0x50): the strings are destroyed and freed, then the ids released and freed. Names and
// layouts are placeholders.
#include "base/UnkVector.h"
#include "base/file/nx/CDeviceNX.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CGameManager/CTunableGameManager.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkStrNode_b257b {
    CRntString str;
    UnkStrNode_b257b* next;  // 0x30
};

struct UnkIdNode_b257b {
    CStrId id;
    UnkIdNode_b257b* next;  // 0x8
};

struct UnkStrList_b257b {
    void** vtable;
    u32 count;  // 0x8
    UnkIdNode_b257b* head;  // 0x10
    UnkIdNode_b257b* tail;  // 0x18
    base::UnkAllocator* allocator;  // 0x20
    u8 pad_0x28[8];
    void** vtable_0x30;
    u32 count_0x38;
    UnkStrNode_b257b* head_0x40;  // 0x40
    UnkStrNode_b257b* tail_0x48;
    base::UnkAllocator* allocator_0x50;
};

void fn_71011a4f1c(UnkStrList_b257b* self) {
    self->vtable_0x30 = g_unk_7101a1a070 + 2;
    UnkStrNode_b257b* str_node = self->head_0x40;
    while (str_node) {
        UnkStrNode_b257b* next = str_node->next;
        fn_710007ecd4(&str_node->str);
        self->allocator_0x50->vf_0x48(str_node);
        str_node = next;
    }
    self->count_0x38 = 0;
    self->head_0x40 = nullptr;
    self->tail_0x48 = nullptr;
    self->vtable = g_unk_7101aa9ae0 + 2;
    UnkIdNode_b257b* id_node = self->head;
    while (id_node) {
        UnkIdNode_b257b* next = id_node->next;
        id_node->id.Reset();
        self->allocator->vf_0x48(id_node);
        id_node = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
}
