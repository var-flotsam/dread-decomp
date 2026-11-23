// CFXComponent's unit (unit 2712 of tools/tu_map.py): follows the owner's FX control track. When the
// track record changes, the effects it no longer names are turned off and the newly named ones on.
// Names and layouts are placeholders.
#include "base/UnkParentPoolList.h"
#include "base/UnkCloneRecords.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "game/UnkAnimationComponent.h"
#include "game/UnkFXIdNode.h"
#include "game/UnkSwitchObject.h"
#include "game/UnkTU132.h"
#include "types.h"

#include <new>

using base::global::CStrId;

// ---- the unit's list of string ids (defined in CFXComponent_Update_ovb0052.cpp) ----

void fn_71011571f8(UnkParentPoolList* self, u32 n, u32 /*unused*/, u32 flags, base::UnkAllocator* a);
void fn_7101157530(UnkParentPoolList* self);
UnkFXIdNode* fn_7101157324(UnkParentPoolList* self, UnkFXIdNode* node);

// The list as this function uses it: built for `n` ids, filled at the back, destroyed at scope end.
struct UnkFXIdList_l14eeac {
    // Raw storage: the list is built and destroyed by the unit's out-of-line functions alone.
    alignas(8) u8 storage[sizeof(UnkParentPoolList)];

    UnkParentPoolList& list() { return *reinterpret_cast<UnkParentPoolList*>(storage); }
    const UnkParentPoolList& list() const { return *reinterpret_cast<const UnkParentPoolList*>(storage); }

    explicit UnkFXIdList_l14eeac(u32 n) { fn_71011571f8(&list(), n, 0, 0x10, nullptr); }
    ~UnkFXIdList_l14eeac() { fn_7101157530(&list()); }

    UnkFXIdNode* Head() const { return static_cast<UnkFXIdNode*>(list().head); }
    UnkFXIdNode* Erase(UnkFXIdNode* node) { return fn_7101157324(&list(), node); }

    void PushBack(const CStrId& id) {
        UnkFXIdNode* node = static_cast<UnkFXIdNode*>(list().allocator->vf_0x40(sizeof(UnkFXIdNode), 8, list().flags));
        if (!node)
            return;
        new (&node->id) CStrId(id);
        node->next = nullptr;
        node->prev = nullptr;
        if (list().count == 0) {
            list().head = node;
            list().tail = node;
            list().count = 1;
        } else {
            static_cast<UnkFXIdNode*>(list().tail)->next = node;
            node->prev = static_cast<UnkFXIdNode*>(list().tail);
            list().tail = node;
            ++list().count;
        }
    }
};

// Removes from both lists every id they share.
static inline void RemoveShared(UnkFXIdList_l14eeac& a, UnkFXIdList_l14eeac& b) {
    UnkFXIdNode* x = a.Head();
    while (x) {
        UnkFXIdNode* y = b.Head();
        while (y && !(y->id == x->id))
            y = y->next;
        if (y) {
            b.Erase(y);
            x = a.Erase(x);
        } else {
            x = x->next;
        }
    }
}

// ---- the component ----

// A named effect: the word at +0x8 holds its kind in the top byte.
struct UnkFXEffect_l14eeac {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void SetEnabled(bool enable);  // 0x40

    u32 field_0x8;
    u8 pad_0xc[0x10d];
    bool attached;  // 0x119

    u32 Kind() const { return field_0x8 >> 24; }
};
static_assert(__builtin_offsetof(UnkFXEffect_l14eeac, attached) == 0x119, "UnkFXEffect_l14eeac layout");

struct UnkFXEntry_l14eeac {
    UnkFXEffect_l14eeac* effect;
    u8 pad_0x8[0x18];
};

// The named effects, in parallel arrays.
struct UnkFXEffects_l14eeac {
    UnkFXEntry_l14eeac* values;
    CStrId* names;
    u32 count;

    UnkFXEntry_l14eeac* End() { return &values[count]; }
    UnkFXEntry_l14eeac* Find(u64 hash) {
        for (u64 i = 0; i < count; ++i)
            if (names[i].data->Key() == hash)
                return &values[i];
        return End();
    }
    UnkFXEntry_l14eeac* Get(u64 hash) {
        UnkFXEntry_l14eeac* p = Find(hash);
        return p == End() ? nullptr : p;
    }
};

struct UnkFXSceneHolder_l14eeac {
    u8 pad_0x0[0x50];
    void* scene;  // 0x50
};

struct UnkFXActor_l14eeac {
    u8 pad_0x0[0x220];
    UnkAnimationComponent* animation;        // 0x220
    UnkFXSceneHolder_l14eeac* scene_holder;  // 0x228
};

// The record the FX control track resolves to: the ids of two sets of effects.
struct UnkFXControlRecord_l14eeac {
    u8 pad_0x0[0x20];
    base::UnkStrIdVec switches;  // 0x20
    u8 pad_0x48[0x8];
    base::UnkStrIdVec effects;   // 0x50
};
static_assert(__builtin_offsetof(UnkFXControlRecord_l14eeac, effects) == 0x50, "UnkFXControlRecord_l14eeac layout");

struct UnkFXComponent_l14eeac {
    u8 pad_0x0[0x30];
    UnkFXActor_l14eeac* actor;  // 0x30
    u8 pad_0x38[0x18];
    UnkFXEffects_l14eeac effects;  // 0x50
    u8 pad_0x68[0xd0];
    UnkFXControlRecord_l14eeac* record;  // 0x138
};
static_assert(__builtin_offsetof(UnkFXComponent_l14eeac, record) == 0x138, "UnkFXComponent_l14eeac layout");

struct UnkFXComponent_b0052;

void fn_710114eeac(UnkFXComponent_b0052* self_) {
    UnkFXComponent_l14eeac* self = reinterpret_cast<UnkFXComponent_l14eeac*>(self_);
    if (!self->actor->animation)
        return;
    UnkTrackQuery query = {fn_7100cfbd38(), nullptr, -1.0f};
    if (UnkAnimationComponent* animation = self->actor->animation)
        fn_710117b08c(animation, &query);
    // The resolved record, seen as an FX control record (the query's own type is the generic one).
    UnkFXControlRecord_l14eeac* const& resolved = reinterpret_cast<UnkFXControlRecord_l14eeac* const&>(query.result);
    if (resolved == self->record)
        return;

    // The ids of the old record and of the new one.
    u32 n = self->record ? self->record->effects.count : 0;
    UnkFXIdList_l14eeac old_effects(n);
    if (n != 0) {
        for (CStrId* it = self->record->effects.data; it != self->record->effects.data + self->record->effects.count;
             ++it)
            old_effects.PushBack(*it);
    }
    n = self->record ? self->record->switches.count : 0;
    UnkFXIdList_l14eeac old_switches(n);
    if (n != 0) {
        for (CStrId* it = self->record->switches.data;
             it != self->record->switches.data + self->record->switches.count; ++it)
            old_switches.PushBack(*it);
    }
    n = query.result ? resolved->effects.count : 0;
    UnkFXIdList_l14eeac new_effects(n);
    if (n != 0) {
        for (CStrId* it = resolved->effects.data;
             it != resolved->effects.data + resolved->effects.count; ++it)
            new_effects.PushBack(*it);
    }
    n = query.result ? resolved->switches.count : 0;
    UnkFXIdList_l14eeac new_switches(n);
    if (n != 0) {
        for (CStrId* it = resolved->switches.data;
             it != resolved->switches.data + resolved->switches.count; ++it)
            new_switches.PushBack(*it);
    }

    // Effects: off for the ids only the old record names, on for those only the new one names.
    RemoveShared(old_effects, new_effects);
    for (UnkFXIdNode* node = old_effects.Head(); node; node = node->next) {
        if (UnkFXEntry_l14eeac* entry = self->effects.Get(node->id.data->Key())) {
            UnkFXEffect_l14eeac* effect = entry->effect;
            if (effect->Kind() == 4 && effect && effect->attached) {
                if (void* scene = self->actor->scene_holder->scene)
                    fn_710014580c(effect, scene);
            }
            effect->SetEnabled(false);
        }
    }
    for (UnkFXIdNode* node = new_effects.Head(); node; node = node->next) {
        if (UnkFXEntry_l14eeac* entry = self->effects.Get(node->id.data->Key())) {
            UnkFXEffect_l14eeac* effect = entry->effect;
            if (effect->Kind() == 4 && effect && !effect->attached) {
                if (void* scene = self->actor->scene_holder->scene)
                    fn_71001457fc(effect, scene, 1);
            }
            effect->SetEnabled(true);
        }
    }

    // Switches: the same, through the switch objects.
    RemoveShared(old_switches, new_switches);
    for (UnkFXIdNode* node = old_switches.Head(); node; node = node->next) {
        if (UnkFXEntry_l14eeac* entry = self->effects.Get(node->id.data->Key())) {
            UnkFXEffect_l14eeac* effect = entry->effect;
            if (effect && effect->Kind() == 6)
                fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(effect), false);
        }
    }
    for (UnkFXIdNode* node = new_switches.Head(); node; node = node->next) {
        if (UnkFXEntry_l14eeac* entry = self->effects.Get(node->id.data->Key())) {
            UnkFXEffect_l14eeac* effect = entry->effect;
            if (effect && effect->Kind() == 6)
                fn_7100146e60(reinterpret_cast<UnkSwitchObject*>(effect), true);
        }
    }

    self->record = resolved;
}
