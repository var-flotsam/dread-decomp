// CCooldownXBossAIComponent's unit (tools/tu_map.py): the reset. After the base reset, clears the state and
// the keyed records at +0x15658, then for each of the four slots spawns the actor named by g_unk_71726d8710
// and, when it is an entity, records it under the slot's six ids (keyed by the first) with a weak handle to
// it; a weak-point life component gets the slot's seventh id's hash. Then fn_71004d0cf4. Names and layouts
// are placeholders.
#include <new>

#include "base/global/CRntSmallDictionary.h"
#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CCooldownXBossAIComponent.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2729.h"
#include "game/UnkTU344.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStringData;

struct UnkObject_71003ad278;
void fn_71003ad278(UnkObject_71003ad278* self);

struct UnkCooldownXRecord_l4d559c;
void fn_71004d559c(base::global::CRntSmallDictionary<CStrId, UnkCooldownXRecord_l4d559c>* self, u32 capacity);

// The actor each slot spawns.
extern CStrId g_unk_71726d8710 __attribute__((visibility("hidden")));

// The unit's static ids, laid out as one block; only the slot table is named here: seven ids for each of
// the four slots (the seventh names the weak point).
struct UnkCooldownXIdBlock_l4d048c {
    u8 pad_0x0[0xa0];
    CStrId slots[7][4];  // 0xa0
};
extern UnkCooldownXIdBlock_l4d048c g_unk_71726d86d8 __attribute__((visibility("hidden")));

// A record's id: copied and assigned as a CStrId, released by the record's destructor (fn_71004d0b00).
struct UnkPartId_l4d048c : base::global::UnkStrIdBase {
    UnkPartId_l4d048c(const UnkStrIdBase& other) : UnkStrIdBase(other) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkPartId_l4d048c(const UnkPartId_l4d048c& other) : UnkPartId_l4d048c(static_cast<const UnkStrIdBase&>(other)) {}
    CStrId& Id() { return static_cast<CStrId&>(static_cast<UnkStrIdBase&>(*this)); }
};

struct UnkCooldownXIds_l4d048c {
    UnkPartId_l4d048c ids[6];
};

// A spawned part: six ids, a weak handle to it and whether it is set.
struct UnkCooldownXPart_l4d048c {
    UnkCooldownXIds_l4d048c names;
    UnkWeakHandle* handle;  // 0x30
    bool active;            // 0x38

    UnkCooldownXPart_l4d048c()
        : names{{CStrId::Empty(), CStrId::Empty(), CStrId::Empty(), CStrId::Empty(), CStrId::Empty(),
                 CStrId::Empty()}},
          handle(nullptr), active(false) {}
    UnkCooldownXPart_l4d048c(const UnkCooldownXPart_l4d048c& other) : names(other.names), handle(nullptr) {
        AssignRef(&handle, other.handle);
        active = other.active;
    }
    ~UnkCooldownXPart_l4d048c() { CCooldownXBossAIComponent_DestroyRecordRefAndIds(reinterpret_cast<UnkCooldownXRecord_ovb0088*>(this)); }
};
static_assert(sizeof(UnkCooldownXPart_l4d048c) == 0x40, "size");

using UnkCooldownXParts_l4d048c = base::global::CRntSmallDictionary<CStrId, UnkCooldownXPart_l4d048c>;

struct UnkCooldownXDefType_l4d048c {
    u32 id;
};

struct UnkCooldownXWeakPoint_l4d048c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x278];
    u64 hash;  // 0x280
};

struct UnkCooldownXSpawned_l4d048c : base::reflection::UnkReflObject {
    u8 pad_0x8[0x1b0];
    UnkCooldownXWeakPoint_l4d048c* life;  // 0x1b8
};

struct UnkCooldownXAI_l4d048c {
    u8 pad_0x0[0x15650];
    u32 field_0x15650;
    bool field_0x15654;
    UnkCooldownXParts_l4d048c parts;  // 0x15658
    u8 pad_after_parts[0x15680 - 0x15658 - sizeof(UnkCooldownXParts_l4d048c)];
    u32 field_0x15680;
    u8 pad_0x15684[0x15834 - 0x15684];
    bool field_0x15834;
};
static_assert(__builtin_offsetof(UnkCooldownXAI_l4d048c, field_0x15834) == 0x15834, "layout");

void fn_71004d0cf4(UnkCooldownXAI_l4d048c* self, bool a, bool b);

// Adds `part` under its first id, growing the arrays by the step when they are full.
static inline void Add(UnkCooldownXParts_l4d048c* parts, const UnkCooldownXPart_l4d048c& part) {
    if (parts->count == parts->capacity)
        fn_71004d559c(reinterpret_cast<base::global::CRntSmallDictionary<CStrId, UnkCooldownXRecord_l4d559c>*>(parts),
                      parts->count + parts->step);
    new (&parts->keys[parts->count]) CStrId(static_cast<const CStrId&>(static_cast<const base::global::UnkStrIdBase&>(part.names.ids[0])));
    new (&parts->values[parts->count]) UnkCooldownXPart_l4d048c(part);
    ++parts->count;
}

void fn_71004d048c(UnkCooldownXAI_l4d048c* self) {
    fn_71003ad278(reinterpret_cast<UnkObject_71003ad278*>(self));
    self->field_0x15650 = 0;
    self->field_0x15680 = 0;
    self->field_0x15834 = false;
    self->field_0x15654 = false;
    self->parts.Clear();
    for (u32 slot = 0; slot < 4; ++slot) {
        UnkStringData* name = g_unk_71726d8710.data;
        UnkActorDefFactory* factory = g_unk_71726a0220;
        ++name->reads;
        UnkCooldownXDefType_l4d048c type;
        type.id = g_unk_7172a0ba48;
        auto* spawned = reinterpret_cast<UnkCooldownXSpawned_l4d048c*>(
            fn_710119438c(factory, reinterpret_cast<const char*>(&name->String()), &g_unk_71726d8710,
                          reinterpret_cast<const UnkPowerUpLifeType_b52&>(type), nullptr));
        if (!spawned || !fn_710009d50c(spawned->vf_0x0(), fn_7100d70c20()))
            continue;
        UnkCooldownXPart_l4d048c part;
        for (u32 i = 0; i < 6; ++i)
            part.names.ids[i].Id() = g_unk_71726d86d8.slots[i][slot];
        AssignRef(&part.handle, fn_7100d78378(reinterpret_cast<UnkWeakTarget*>(spawned)));
        part.active = true;
        Add(&self->parts, part);
        UnkCooldownXWeakPoint_l4d048c* life = spawned->life;
        if (life && fn_710009d50c(life->vf_0x0(), fn_71004d65b4()))
            life->hash = g_unk_71726d86d8.slots[6][slot].data->Key();
    }
    fn_71004d0cf4(self, false, false);
}
