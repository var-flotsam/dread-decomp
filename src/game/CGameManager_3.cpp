// More of CGameManager::CTunableGameManager's unit: the entity the game manager's weak reference at
// +0x7e8 points to; while it points to nothing, the first entity of the lists at +0x2638 and +0x2698
// with something at +0x248 becomes the reference. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkManagedEntity : base::reflection::UnkReflObject {
    u8 pad_0x8[0x240];
    void* field_0x248;
};

struct UnkManagedEntityNodeA {
    u64 field_0x0;
    UnkManagedEntity* entity;
    UnkManagedEntityNodeA* next;
};

struct UnkManagedEntityNodeB {
    UnkManagedEntity* entity;
    UnkManagedEntityNodeB* next;
};

struct UnkEntityPickManager {
    u8 pad_0x0[0x7e8];
    UnkWeakHandle* field_0x7e8;
    u8 pad_0x7f0[0x1e48];
    UnkManagedEntityNodeA* field_0x2638;
    u8 pad_0x2640[0x58];
    UnkManagedEntityNodeB* field_0x2698;

    // The first listed entity of the entity class with something at +0x248 becomes the reference.
    template <class Node>
    __attribute__((always_inline)) void PickFrom(Node* head) {
        for (Node* n = head; n; n = n->next) {
            UnkManagedEntity* entity = n->entity;
            if (entity && fn_710009d50c(entity->vf_0x0(), fn_7100d70c20()) && entity->field_0x248) {
                AssignRef(&field_0x7e8, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(entity)));
                return;
            }
        }
    }
    bool IsSet() const { return field_0x7e8 && field_0x7e8->target; }
};
static_assert(__builtin_offsetof(UnkEntityPickManager, field_0x2698) == 0x2698, "UnkEntityPickManager layout");

UnkWeakTarget* fn_7100decab0(UnkEntityPickManager* self) {
    if (!self->IsSet()) {
        self->PickFrom(self->field_0x2638);
        if (!self->IsSet())
            self->PickFrom(self->field_0x2698);
    }
    return self->field_0x7e8 ? self->field_0x7e8->target : nullptr;
}
