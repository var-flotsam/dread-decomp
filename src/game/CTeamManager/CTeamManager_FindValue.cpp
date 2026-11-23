// CTeamManager::CTunableTeamManager unit: the value a list keeps for an entity, found through each
// node's weak reference (a block whose +0x10 is the target), or FLT_MAX when absent.
// Names and layouts are placeholders.
#include <cfloat>

#include "types.h"

struct UnkTeamRefBlock {
    u8 unk_0x0[0x10];
    void* target;
};

struct UnkTeamNode_CTeamManager_FindValue {
    UnkTeamRefBlock* ref;
    f32 value;
    UnkTeamNode_CTeamManager_FindValue* next;
};

struct UnkTeamList {
    u8 unk_0x0[0x10];
    UnkTeamNode_CTeamManager_FindValue* head;
};

f32 fn_71011a76ec(const UnkTeamList* self, const void* target) {
    for (UnkTeamNode_CTeamManager_FindValue* n = self->head; n != nullptr; n = n->next) {
        void* t = n->ref != nullptr ? n->ref->target : nullptr;
        if (t == target)
            return n->value;
    }
    return FLT_MAX;
}
