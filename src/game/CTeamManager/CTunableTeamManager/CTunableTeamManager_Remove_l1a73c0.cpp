// CTunableTeamManager's unit (tools/tu_map.py unit 2738): removes the member `target` from the team's
// member list, dropping its reference and freeing its node. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "base/UnkVtableList.h"

// A counted reference to a weak handle.
struct UnkTeamRef_l1a73c0 {
    UnkWeakHandle* ptr;

    explicit UnkTeamRef_l1a73c0(UnkWeakHandle* p) : ptr(nullptr) { AssignRef(&ptr, p); }
    ~UnkTeamRef_l1a73c0() {
        if (UnkWeakHandle* p = ptr) {
            if (--p->ref_count == 0)
                p->vf_0x10();
        }
    }
    UnkWeakTarget* Target() const { return ptr ? ptr->target : nullptr; }
};

struct UnkTeamNode_l1a73c0 {
    UnkTeamRef_l1a73c0 handle;
    f32 value;
    bool flag;
    UnkTeamNode_l1a73c0* next;
};

// The member list (UnkTeamList in the unit's other file).
struct UnkTeamMembers_l1a73c0 : base::UnkVtableList<UnkTeamNode_l1a73c0> {
    UnkTeamNode_l1a73c0* Find(UnkWeakTarget* actor) {
        UnkTeamRef_l1a73c0 key(actor ? fn_7100d78378(actor) : nullptr);
        for (UnkTeamNode_l1a73c0* n = head; n; n = n->next) {
            UnkWeakTarget* t = n->handle.Target();
            if (t && t == key.Target())
                return n;
        }
        return nullptr;
    }

    void Erase(UnkTeamNode_l1a73c0* node) {
        UnkTeamNode_l1a73c0* prev = nullptr;
        UnkTeamNode_l1a73c0* cur = head;
        UnkTeamNode_l1a73c0* next;
        UnkTeamNode_l1a73c0* last = nullptr;
        for (;;) {
            if (!cur)
                return;
            prev = last;
            next = cur->next;
            if (cur == node)
                break;
            last = cur;
            cur = next;
        }
        if (prev) {
            prev->next = next;
            if (tail == node)
                tail = prev;
        } else {
            head = next;
        }
        if (--count == 0) {
            head = nullptr;
            tail = nullptr;
        }
        node->handle.~UnkTeamRef_l1a73c0();
        allocator->vf_0x48(node);
    }
};

void fn_71011a73c0(UnkTeamMembers_l1a73c0* self, UnkWeakTarget* target) {
    if (UnkTeamNode_l1a73c0* node = self->Find(target))
        self->Erase(node);
}
