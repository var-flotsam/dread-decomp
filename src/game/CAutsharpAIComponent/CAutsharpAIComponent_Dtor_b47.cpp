// CAutsharpAIComponent unit (tools/tu_map.py unit 572): the destructor and the deleting destructor of a
// class there: its vtable, the list link at +0x146f0, then the base destructor (fn_7100661914). Names are
// placeholders.
#include "game/CRobotAIComponent.h"
#include "types.h"

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);

// A link into a list (the links of src/game/navmesh/SNavMeshPath/fn_71002ad978.cpp): unlinked and
// cleared on destruction.
struct UnkListLink_b47 {
    UnkLinkList* list;
    void* field_0x8;
    void* field_0x10;

    ~UnkListLink_b47() {
        if (list) {
            fn_71002aeb9c(list, &list);
            list = nullptr;
        }
        field_0x10 = nullptr;
        field_0x8 = nullptr;
    }
};

struct UnkAutsharpAIBase_b47;

extern void* g_unk_7101a2d9e8[];  // the class's vtable

struct UnkAutsharpAI_b47 {
    void** vtable;
    u8 pad_0x8[0x146f0 - 0x8];
    UnkListLink_b47 link;  // 0x146f0
};

void fn_71003c8cd8(UnkAutsharpAI_b47* self) {
    self->vtable = &g_unk_7101a2d9e8[2];
    self->link.~UnkListLink_b47();
    fn_7100661914(reinterpret_cast<UnkAutsharpAIBase_b47*>(self));
}
void fn_71003c8d40(UnkAutsharpAI_b47* self) {
    fn_71003c8cd8(self);
    __builtin_operator_delete(self);
}
