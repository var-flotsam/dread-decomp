// CBreakableHintComponent unit (tools/tu_map.py, the name from reflection). The component keeps a flat
// map at +0x48 from pickable items (the key is the item's address) to references registered in each
// item's deferred set at +0x250 (the reference holds the set, the component and the callback
// fn_710075312c, which erases the item's entry). Here: the map's reserve (fn_7100752fbc), push-back
// (fn_7100752eec) and erase at an iterator (fn_7100752a78, the last entry moves into the hole); the
// destructor (fn_7100751eb8); the refresh that rebuilds the map from the pickable items inside the
// component's shape (fn_7100752208); the update (fn_71007527ac: entering the shape marks the component,
// and while the player stands in it in state 3 a 0.6 s timer runs out into fn_7100e9d654 unless the
// minimap table holds a zero); and the load from the scenario's blackboard section (fn_7100751ffc: the
// BREAKABLE_HINTS property, a map of entity ids to flags). Names and layouts are placeholders.
#include <new>

#include "base/UnkDeferredSet.h"
#include "base/UnkVector.h"
#include "base/math/CVector.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CActorComponent.h"
#include "game/CBreakableHintComponent.h"
#include "game/CCharacterMovement.h"
#include "game/UnkComponent.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU2553.h"
#include "types.h"

// The out-of-line Remove and Add of the deferred set a hint reference registers in (copies of
// base::UnkDeferredSet's members; the item is the reference's address).
struct UnkHintRef_ovb0093;
void fn_7100752ba8(base::UnkDeferredSet<UnkHintRef_ovb0093>* set, UnkHintRef_ovb0093* item);
void fn_7100752d38(base::UnkDeferredSet<UnkHintRef_ovb0093>* set, UnkHintRef_ovb0093* item);

// A reference that keeps itself registered in a deferred set: moving it re-registers the new
// address and leaves the source empty.
struct UnkHintRef_ovb0093 {
    base::UnkDeferredSet<UnkHintRef_ovb0093>* set;
    void* field_0x8;
    void* field_0x10;

    void Reset() {
        if (set) {
            fn_7100752ba8(set, this);
            set = nullptr;
        }
        field_0x8 = nullptr;
        field_0x10 = nullptr;
    }
    UnkHintRef_ovb0093(UnkHintRef_ovb0093&& o) : set(o.set), field_0x8(o.field_0x8), field_0x10(o.field_0x10) {
        o.Reset();
        if (set)
            fn_7100752d38(set, this);
    }
    ~UnkHintRef_ovb0093() { Reset(); }
    UnkHintRef_ovb0093() : set(nullptr), field_0x8(nullptr), field_0x10(nullptr) {}
    void Assign(base::UnkDeferredSet<UnkHintRef_ovb0093>* s, void* a, void* b) {
        field_0x8 = a;
        field_0x10 = b;
        if (set) {
            fn_7100752ba8(set, this);
            set = nullptr;
        }
        set = s;
        fn_7100752d38(s, this);
    }
};

// A flat map: values and keys in parallel arrays sharing a count and capacity.
struct UnkHintMap_ovb0093 {
    UnkHintRef_ovb0093* values;
    u64* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;
};

__attribute__((noinline)) void fn_7100752fbc(UnkHintMap_ovb0093* self, u32 new_capacity) {
    if (self->capacity >= new_capacity)
        return;
    UnkHintRef_ovb0093* values = static_cast<UnkHintRef_ovb0093*>(
        self->allocator->vf_0x40((u64)new_capacity * sizeof(UnkHintRef_ovb0093), 8, self->flags));
    u64* keys = static_cast<u64*>(self->allocator->vf_0x40((u64)new_capacity * sizeof(u64), 8, self->flags));
    if (self->values) {
        UnkHintRef_ovb0093* src = self->values;
        UnkHintRef_ovb0093* dst = values;
        for (u32 i = 0; i < self->count; ++i) {
            new (dst) UnkHintRef_ovb0093(static_cast<UnkHintRef_ovb0093&&>(*src));
            src->~UnkHintRef_ovb0093();
            ++src;
            ++dst;
        }
        self->allocator->vf_0x48(self->values);
    }
    if (self->keys) {
        __builtin_memcpy(keys, self->keys, (u64)self->count * sizeof(u64));
        self->allocator->vf_0x48(self->keys);
    }
    self->values = values;
    self->keys = keys;
    self->capacity = new_capacity;
}

UnkHintRef_ovb0093* fn_7100752eec(UnkHintMap_ovb0093* self, const u64& key, UnkHintRef_ovb0093&& value) {
    if (self->count == self->capacity)
        fn_7100752fbc(self, self->count + self->step);
    __builtin_memcpy(&self->keys[self->count], &key, sizeof(u64));
    new (&self->values[self->count]) UnkHintRef_ovb0093(static_cast<UnkHintRef_ovb0093&&>(value));
    u32 n = self->count + 1;
    self->count = n;
    return n == 0 ? self->values : &self->values[n] - 1;
}

UnkHintRef_ovb0093* fn_7100752a78(UnkHintMap_ovb0093* self, UnkHintRef_ovb0093* it) {
    u32 index = it - self->values;
    self->values[index].~UnkHintRef_ovb0093();
    u32 last = self->count - 1;
    if (last != index) {
        new (&self->values[index]) UnkHintRef_ovb0093(static_cast<UnkHintRef_ovb0093&&>(self->values[last]));
        self->values[self->count - 1].~UnkHintRef_ovb0093();
        __builtin_memcpy(&self->keys[index], &self->keys[self->count - 1], sizeof(u64));
    }
    --self->count;
    return &self->values[index];
}

struct UnkLinkList;
void fn_71002aeb9c(UnkLinkList* list, UnkLinkList** slot);
extern void* g_unk_7101a6e2a0[];
extern void* g_unk_7101a81968[];

// As src/game/CBreakableHintComponent/CBreakableHintComponent_Remove_A3_b72.cpp declares them.
struct UnkBreakableHint_A3_b72;
bool fn_710075312c(UnkBreakableHint_A3_b72* self, u64 key);

struct UnkHintWords_ovb0093 {
    u32 x, y, z;
};

struct UnkHintPosition_ovb0093 {
    u8 pad_0x0[0x44];
    u32 x;  // 0x44
    u8 pad_0x48[0xc];
    u32 y;  // 0x54
    u8 pad_0x58[0xc];
    u32 z;  // 0x64
};

// A pickable item as the hint refresh sees it.
struct UnkHintItem_ovb0093 : base::reflection::UnkReflObject {
    u8 pad_0x8[0x10];
    UnkHintPosition_ovb0093* actor;  // 0x18
    u8 pad_0x20[0x10b];
    bool field_0x12b;
    u8 pad_0x12c[0x124];
    base::UnkDeferredSet<UnkHintRef_ovb0093> hints;  // 0x250
};

struct UnkHintItemSlot_ovb0093 {
    u8 pad_0x0[0x10];
    UnkHintItem_ovb0093* item;  // 0x10
};

struct UnkHintItemList_ovb0093 {
    UnkHintItemSlot_ovb0093** slots;
    u32 count;
};

UnkHintItemList_ovb0093* fn_71009cf74c();

// The component's shape (owned: the destructor deletes it through slot 2).
struct UnkHintArea_ovb0093 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void vf_0x48();
    virtual bool vf_0x50(const base::math::CVector2D* point);
};

struct UnkHintOwnerEntity_ovb0093 {
    u8 pad_0x0[0x30];
    u64 id;  // 0x30
};

struct UnkBreakableHint_ovb0093 {
    void** vtable;
    u8 pad_0x8[0x10];
    UnkHintOwnerEntity_ovb0093* entity;  // 0x18
    u8 pad_0x20[0x10];
    UnkHintArea_ovb0093* area;  // 0x30
    u8 pad_0x38[0x4];
    f32 timer;  // 0x3c
    bool entered;  // 0x40
    union {
        bool empty;  // 0x41
        u8 shown;
    };
    u8 pad_0x42[0x6];
    UnkHintMap_ovb0093 hints;  // 0x48
    UnkLinkList* link;  // 0x70
    u64 field_0x78;
    u64 field_0x80;
};

void fn_7100751eb8(UnkBreakableHint_ovb0093* self) {
    self->vtable = g_unk_7101a6e2a0 + 2;
    UnkLinkList** slot = &self->link;
    if (*slot) {
        fn_71002aeb9c(*slot, slot);
        *slot = nullptr;
    }
    self->field_0x78 = 0;
    self->field_0x80 = 0;
    UnkHintMap_ovb0093* hints = &self->hints;
    if (hints->values) {
        for (u64 i = 0; i < hints->count; ++i)
            hints->values[i].~UnkHintRef_ovb0093();
    }
    hints->count = 0;
    if (hints->keys)
        hints->allocator->vf_0x48(hints->keys);
    if (hints->values)
        hints->allocator->vf_0x48(hints->values);
    self->vtable = g_unk_7101a81968 + 2;
    UnkHintArea_ovb0093* area = self->area;
    self->area = nullptr;
    if (area)
        area->vf_0x10();
    fn_710024f6d4(reinterpret_cast<UnkBaseComponent*>(self));
}

// Whether the actor's translation (copied word by word), on the 2D plane, lies in the area.
static inline bool InArea(UnkHintArea_ovb0093* area, const UnkHintPosition_ovb0093* actor) {
    UnkHintWords_ovb0093 position;
    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    base::math::CVector2D point;
    fn_710007b430(&point, reinterpret_cast<const base::math::CVector3D*>(&position));
    return area->vf_0x50(&point);
}

void fn_7100752208(UnkBreakableHint_ovb0093* self) {
    UnkHintMap_ovb0093* hints = &self->hints;
    if (hints->values) {
        for (u64 i = 0; i < hints->count; ++i)
            hints->values[i].~UnkHintRef_ovb0093();
    }
    hints->count = 0;
    UnkHintItemList_ovb0093* list = fn_71009cf74c();
    UnkHintItemSlot_ovb0093** end = list->slots + list->count;
    for (UnkHintItemSlot_ovb0093** it = list->slots; it != end; ++it) {
        UnkHintItemSlot_ovb0093* slot = *it;
        if (!slot)
            continue;
        UnkHintItem_ovb0093* item = slot->item;
        if (!item || !fn_710009d50c(item->vf_0x0(), fn_71009ce53c()))
            continue;
        if (!InArea(self->area, item->actor) || item->field_0x12b)
            continue;
        u64 key = reinterpret_cast<u64>(item);
        UnkHintRef_ovb0093* ref = fn_7100752eec(hints, key, UnkHintRef_ovb0093());
        ref->Assign(&item->hints, self, reinterpret_cast<void*>(fn_710075312c));
    }
    if (hints->count == 0) {
        self->empty = true;
        fn_71007523e8(reinterpret_cast<UnkBreakableHint_A3_b72*>(self));
    }
}

struct UnkPlayer;
struct UnkMinimapManagerState;
void fn_7100e9d654(UnkMinimapManagerState* self);

struct UnkHintWorld_ovb0093 {
    u8 pad_0x0[0xd64420];
    const u32* table;  // 0xd64420
    u32 count;         // 0xd64428
};

struct UnkHintPlayerState_ovb0093 {
    u8 pad_0x0[0x10];
    s32 kind;  // 0x10
};

struct UnkHintPlayerSub_ovb0093 {
    u8 pad_0x0[0x2d08];
    UnkHintPlayerState_ovb0093* state;  // 0x2d08
};

struct UnkHintPlayer_ovb0093 : UnkHintPosition_ovb0093 {
    u8 pad_0x68[0x190];
    void* field_0x1f8;
    u8 pad_0x200[0x8];
    UnkHintPlayerSub_ovb0093* field_0x208;
};

// As InArea, for the player (the point is declared first, so it is above the position on the stack).
static inline bool InAreaPlayer(UnkHintArea_ovb0093* area, const UnkHintPosition_ovb0093* actor) {
    base::math::CVector2D point;
    UnkHintWords_ovb0093 position;
    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    fn_710007b430(&point, reinterpret_cast<const base::math::CVector3D*>(&position));
    return area->vf_0x50(&point);
}

void fn_71007527ac(UnkBreakableHint_ovb0093* self, f32 dt) {
    fn_710024fb24(self);
    UnkHintWorld_ovb0093* world = static_cast<UnkHintWorld_ovb0093*>(g_unk_7101d47310->field_0x2228);
    bool blocked = false;
    const u32* p = world->table;
    for (u64 n = world->count; n != 0; --n, ++p) {
        if (*p == 0) {
            blocked = true;
            break;
        }
    }
    const bool& entered = self->entered;
    if (blocked && entered)
        return;
    UnkHintPlayer_ovb0093* player = reinterpret_cast<UnkHintPlayer_ovb0093*>(fn_7100dec788(g_unk_7101d47310));
    if (InAreaPlayer(self->area, player)) {
        if (!entered) {
            self->entered = true;
            fn_71007523e8(reinterpret_cast<UnkBreakableHint_A3_b72*>(self));
        }
        if (fn_7100979ef0(player->field_0x1f8) && player->field_0x208) {
            if (UnkHintPlayerState_ovb0093* state = player->field_0x208->state) {
                if (state->kind == 3 && !blocked && self->timer < 0.0f)
                    self->timer = 0.6f;
            }
        }
    }
    if (self->timer > 0.0f) {
        self->timer -= dt;
        if (self->timer <= 0.0f)
            fn_7100e9d654(static_cast<UnkMinimapManagerState*>(g_unk_7101d47310->field_0x2228));
    }
}

namespace unk_static_init {
struct UnkEnumType;
}
using base::global::CStrId;
using unk_static_init::UnkEnumType;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
extern UnkPropertyEx<> g_unk_7172711390;  // BREAKABLE_HINTS
extern UnkEnumType g_unk_7172735f58;      // its value type

// The stored hints: whether each entity's hint is shown, keyed by the entity id.
struct UnkHintFlags_ovb0093 {
    u8* values;
    u64* keys;
    u32 count;

    u8* Find(const u64& key) {
        for (u64 i = 0; i < count; ++i)
            if (keys[i] == key)
                return &values[i];
        return &values[count];
    }
};

struct UnkHintSectionValues_ovb0093 {
    base::reflection::UnkReflValue* values;
    u64* ids;
    u32 count;

    base::reflection::UnkReflValue* Find(const u64& id) {
        for (u64 i = 0; i < count; ++i)
            if (ids[i] == id)
                return &values[i];
        return &values[count];
    }
};

static inline const UnkQualifiedId& HintsQualifiedId() {
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_7172711390);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_7172711390), g_unk_7101d470e0);
    return qid;
}

void fn_7100751ffc(UnkBreakableHint_ovb0093* self, CBlackboard* blackboard) {
    CStrId section_id = fn_71011a2fbc(g_unk_7101d47310->field_0x608);
    self->timer = -1.0f;
    self->entered = false;
    const UnkQualifiedId& qid = HintsQualifiedId();
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id)) {
        UnkHintSectionValues_ovb0093* values = reinterpret_cast<UnkHintSectionValues_ovb0093*>(&section->data.properties);
        base::reflection::UnkReflValue* v = values->Find(*reinterpret_cast<const u64*>(&qid.id));
        if (v != &values->values[values->count] && v->type) {
            void* p = v->field_0x14 == 1 || *reinterpret_cast<const u64*>(v->type + 0x18) > 8 ? v->field_0x0 : v;
            bool ok = fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(v->type),
                                    reinterpret_cast<const base::reflection::CType*>(&g_unk_7172735f58));
            if (UnkHintFlags_ovb0093* flags = ok ? static_cast<UnkHintFlags_ovb0093*>(p) : nullptr) {
                u8* it = flags->Find(self->entity->id);
                if (it != &flags->values[flags->count]) {
                    self->shown = *it;
                    self->entered = true;
                }
            }
        }
    }
}
