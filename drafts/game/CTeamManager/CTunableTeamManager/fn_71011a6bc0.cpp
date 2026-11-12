// CTeamManager::CTunableTeamManager: checks an actor against a request and flags its list node.
// Names and layouts are placeholders.
#include "types.h"

bool fn_710110592c(char* comp, char* request);

struct UnkTeamActor {
    char pad_0x0[0x240];
    char* field_0x240;
};

struct UnkTeamEntry {
    char pad_0x0[0x10];
    UnkTeamActor* actor;  // 0x10
};

struct UnkTeamNode {
    UnkTeamEntry* entry;  // 0x0
    char pad_0x8[4];
    u8 flag;              // 0xc
    UnkTeamNode* next;    // 0x10
};

struct UnkTeamList {
    char pad_0x0[0x10];
    UnkTeamNode* head;    // 0x10
};

struct UnkTeamRequest {
    char pad_0x0[0x20];
    void* field_0x20;
};

bool fn_71011a6bc0(UnkTeamList* self, UnkTeamActor* actor, UnkTeamRequest* request) {
    bool ok = false;
    if (request) {
        void* data = request->field_0x20;
        if (data != nullptr && actor != nullptr) {
            if (actor->field_0x240 != nullptr) {
                ok = fn_710110592c(actor->field_0x240, (char*)request);
                if (!ok)
                    return false;
                UnkTeamNode* n = self->head;
                if (!n)
                    return true;
                do {
                    UnkTeamEntry* e = n->entry;
                    UnkTeamActor* found = e ? e->actor : nullptr;
                    // keeps the node's own actor pointer distinct from the argument, as the original reloads it
                    asm("" : "+r"(found));
                    if (found == actor) {
                        char* comp = e->actor->field_0x240;
                        if (comp && *(void**)(comp + 0x1148)) {
                            n->flag = 1;
                            return ok;
                        }
                    }
                    n = n->next;
                } while (n);
            }
        }
    }
    return ok;
}
