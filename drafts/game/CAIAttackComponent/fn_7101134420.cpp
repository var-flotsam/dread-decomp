// CAIAttackComponent unit (tools/tu_map.py unit 2705): fn_7101134420 starts an attack on the component:
// unless the owner's AI is in state 2, checks the attack's id (vf_0x170), then starts it by the given id or
// its own (vf_0x168; an empty id goes through vf_0x1c0) and tells the owner's AI. Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CEnemyMovement.h"
#include "game/UnkActor.h"
#include "types.h"

using base::global::CStrId;

struct UnkGoliathXSlamAttack_b58a3;
CStrId fn_71003627f8(UnkGoliathXSlamAttack_b58a3*, UnkActor*);

struct UnkAIAttackEntry_l134420 {
    void** vtable;

    void vf_0x28() { reinterpret_cast<void (*)(UnkAIAttackEntry_l134420*)>(vtable[0x28 / 8])(this); }
};

struct UnkAIAttackAI_l134420 {
    void** vtable;

    void vf_0x288(UnkAIAttackEntry_l134420* entry) {
        reinterpret_cast<void (*)(UnkAIAttackAI_l134420*, UnkAIAttackEntry_l134420*)>(vtable[0x288 / 8])(this,
                                                                                                         entry);
    }
};

struct UnkAIAttackComponent_l134420 {
    void** vtable;
    u8 pad_0x8[0x28];
    UnkActor* owner;  // 0x30
    u8 pad_0x38[0x10];
    bool field_0x48;
    u8 pad_0x49[0x157];
    UnkAIAttackEntry_l134420* field_0x1a0;
    bool field_0x1a8;

    bool vf_0x168(const CStrId* id, bool flag, bool arg3, u32 arg4, f32 arg5) {
        return reinterpret_cast<bool (*)(UnkAIAttackComponent_l134420*, const CStrId*, bool, bool, u32, f32)>(
            vtable[0x168 / 8])(this, id, flag, arg3, arg4, arg5);
    }
    bool vf_0x170(const CStrId* id, bool flag) {
        return reinterpret_cast<bool (*)(UnkAIAttackComponent_l134420*, const CStrId*, bool)>(
            vtable[0x170 / 8])(this, id, flag);
    }
    void vf_0x1c0() { reinterpret_cast<void (*)(UnkAIAttackComponent_l134420*)>(vtable[0x1c0 / 8])(this); }

    // Starts the attack by its id; the empty id starts it without one.
    bool StartWith(UnkAIAttackEntry_l134420* entry, const CStrId& id, bool flag, u32 arg3, f32 arg5) {
        if (id.data != &g_unk_7101d47108->empty) {
            if (!vf_0x168(&id, flag, false, arg3, arg5))
                return false;
            field_0x1a0 = entry;
            field_0x1a8 = false;
            entry->vf_0x28();
            return true;
        }
        field_0x48 = true;
        vf_0x1c0();
        field_0x1a0 = entry;
        entry->vf_0x28();
        return true;
    }
};

static inline CStrId AttackId(UnkAIAttackEntry_l134420* entry, UnkActor* owner) {
    return fn_71003627f8(reinterpret_cast<UnkGoliathXSlamAttack_b58a3*>(entry), owner);
}

bool fn_7101134420(UnkAIAttackComponent_l134420* self, UnkAIAttackEntry_l134420* entry, bool flag, u32 arg3,
                   const CStrId* name, f32 arg5) {
    if (!entry)
        return false;
    void* ai = self->owner->field_0x218;
    if (ai && fn_71010cc3a4(ai) == 2)
        return false;
    bool allowed;
    {
        CStrId id = AttackId(entry, self->owner);
        allowed = self->vf_0x170(&id, flag);
    }
    if (!allowed)
        return false;
    bool started;
    {
        CStrId id = name ? *name : AttackId(entry, self->owner);
        started = self->StartWith(entry, id, flag, arg3, arg5);
    }
    if (!started)
        return false;
    if (auto* owner_ai = static_cast<UnkAIAttackAI_l134420*>(self->owner->field_0x240))
        owner_ai->vf_0x288(entry);
    return true;
}
