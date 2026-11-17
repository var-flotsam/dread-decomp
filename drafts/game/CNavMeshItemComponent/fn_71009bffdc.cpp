// CNavMeshItemComponent unit (tools/tu_map.py unit 1482): stores the item's navigation state in the current
// scenario's blackboard section (its position when it moved, and per actor the nodes of its sources).
// Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkSubId_7100fc4b34.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct UnkEnumType;
struct UnkNavElemMap_ovb0100;

extern UnkPropertyEx<> g_unk_717273e210;       // the property the nodes map is stored under
extern UnkPropertyEx<0x160> g_unk_717273e258;  // the property the positions are stored under
extern UnkEnumType g_unk_717273e7f0;           // the nodes map's type
extern const f32 g_unk_7101647774;
extern const base::math::CVector2D g_unk_7101647780;

bool fn_7100df255c(UnkGameManager* self);
CRntVector<u64>* fn_71009c09a8(UnkNavElemMap_ovb0100* self, CRntVector<u64>* it);
CRntVector<u64>* fn_71009c478c(UnkNavElemMap_ovb0100* self, const CStrId* key, const CRntVector<u64>* value);

// A position as the sources hand it back (by hidden pointer: it is not trivially copyable).
struct UnkNavPoint_l9bffdc {
    f32 x;
    f32 y;
    UnkNavPoint_l9bffdc(const UnkNavPoint_l9bffdc& other) : x(other.x), y(other.y) {}
};

struct UnkNavNode_l9bffdc {
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
    virtual void vf_0x50();
    virtual u32 vf_0x58();  // its kind flags

    u8 pad_0x8[0x10];
    u64 field_0x18;
};

struct UnkNavSource_l9bffdc {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual UnkNavNode_l9bffdc* vf_0x20(u32 index);
    virtual UnkNavPoint_l9bffdc vf_0x28();

    u8 pad_0x8[0x44];
    u32 count;  // 0x4c
};

struct UnkNavActor_l9bffdc {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x1b4 - 0x38];
    bool field_0x1b4;
};

struct UnkNavOwner_l9bffdc {
    u8 pad_0x0[0x1b8];
    base::reflection::UnkReflObject* life;  // 0x1b8
};

struct UnkNavMeshItem_l9bffdc {
    u8 pad_0x0[0x18];
    UnkNavActor_l9bffdc* actor;  // 0x18
    bool field_0x20;
    u8 pad_0x21[0x30 - 0x21];
    UnkNavOwner_l9bffdc* owner;  // 0x30
    u8 pad_0x38[0x178 - 0x38];
    UnkNavSource_l9bffdc* selected;  // 0x178
    UnkNavSource_l9bffdc* current;   // 0x180
    u8 pad_0x188[0x1b8 - 0x188];
    bool field_0x1b8;
};

// The node lists of the actors, by actor name (the map fn_71009c478c and fn_71009c09a8 work on).
struct UnkNavNodeMap_l9bffdc {
    CRntVector<u64>* values;
    CStrId* keys;
    u32 count;
    u32 capacity;
    u32 step;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkNavNodeMap_l9bffdc(u32 step, u32 flags)
        : values(nullptr), keys(nullptr), count(0), capacity(0), step(step), flags(flags),
          allocator(fn_7100083990()) {}

    CRntVector<u64>* End() { return &values[count]; }

    CRntVector<u64>* Find(const CStrId& key) {
        for (u64 i = 0; i < count; ++i) {
            if (keys[i].data == key.data)
                return &values[i];
        }
        return End();
    }

    UnkNavElemMap_ovb0100* Raw() { return reinterpret_cast<UnkNavElemMap_ovb0100*>(this); }
};

// Releases the keys, frees the lists and both arrays.
static void FreeNodeMap(UnkNavNodeMap_l9bffdc& map) {
    if (map.keys) {
        for (u64 i = 0; i < map.count; ++i)
            map.keys[i].~CStrId();
    }
    if (map.values) {
        for (u64 i = 0; i < map.count; ++i)
            map.values[i].Free();
    }
    map.count = 0;
    if (map.keys)
        map.allocator->vf_0x48(map.keys);
    if (map.values)
        map.allocator->vf_0x48(map.values);
}

// The section's properties, looked up by name.
struct UnkNavSectionValues_l9bffdc {
    UnkReflValue* values;
    CStrId* ids;
    u32 count;

    UnkReflValue* Find(const CStrId& id) {
        for (u64 i = 0; i < count; ++i) {
            if (ids[i].data == id.data)
                return &values[i];
        }
        return &values[count];
    }
};

static inline const CType* NodeMapType() {
    return reinterpret_cast<const CType*>(&g_unk_717273e7f0);
}

// Whether the owner's life component is of the class `getter` returns.
template <const base::reflection::CClass* (*getter)()>
static inline bool OwnerLifeIs(UnkNavMeshItem_l9bffdc* self) {
    base::reflection::UnkReflObject* life = self->owner->life;
    return life && fn_710009d50c(life->vf_0x0(), getter());
}

static inline const UnkQualifiedId& NodesQualifiedId() {
    UnkQualifiedId& qid = reinterpret_cast<UnkQualifiedId&>(g_unk_717273e210.field_0x10);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, reinterpret_cast<UnkIdSource*>(&g_unk_717273e210), g_unk_7101d470e0);
    return qid;
}

static inline UnkSubId_7100fc4b34 PositionKey(const CStrId& name) {
    UnkQualifiedName* qualified = reinterpret_cast<UnkQualifiedName*>(g_unk_717273e258.field_0x10);
    if (!qualified->str || !qualified->str[0])
        fn_7100ae9d7c(qualified, reinterpret_cast<UnkIdSource*>(&g_unk_717273e258), g_unk_7101d470e0);
    return fn_71003327a4(qualified, &name);
}

// Reads the nodes map stored in `section` into `map`.
static inline void ReadNodeMap(CBlackboard::CSection* section, const CStrId& id, UnkNavNodeMap_l9bffdc* map) {
    auto* properties = reinterpret_cast<UnkNavSectionValues_l9bffdc*>(&section->data.properties);
    UnkReflValue* v = properties->Find(id);
    if (v == &properties->values[properties->count])
        return;
    const char* type = v->type;
    if (!type)
        return;
    void* p = v->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? v->field_0x0 : v;
    if (!p)
        return;
    const CType* t = NodeMapType();
    UnkTypedRef dst{NodeMapType(), map};
    UnkTypedRef src;
    fn_71000a16e0(&src, type, p);
    fn_710009d7a8(t, &dst, &src);
}

// Stores the nodes map in the current scenario's section, unless the blackboard is locked.
static inline void WriteNodeMap(UnkGameManager* manager, const CStrId& scenario, UnkNavNodeMap_l9bffdc* map) {
    const UnkQualifiedId& qid = NodesQualifiedId();
    CBlackboard::CSection* section = fn_71010472e0(&manager->field_0x968, scenario);
    if (section && !manager->field_0x968.field_0xe8) {
        UnkTypedValue v;
        v.type = NodeMapType();
        v.value = map;
        fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<CStrId*>(&qid.id)), &v);
    }
}

static inline bool NearlyEqual(f32 a, f32 b) {
    f32 lo = a - g_unk_7101647774;
    f32 hi = a + g_unk_7101647774;
    return hi >= b && lo <= b;
}

// Appends the field_0x18 of each of the source's nodes whose kind is one of the navigable ones.
static inline void CollectNodes(UnkNavSource_l9bffdc* const& source, CRntVector<u64>& nodes) {
    for (u32 i = 0; i < source->count; ++i) {
        UnkNavNode_l9bffdc* node = source->vf_0x20(i);
        if (node && (node->vf_0x58() & 0x25b))
            nodes.PushBack(node->field_0x18);
    }
}

void fn_71009bffdc(UnkNavMeshItem_l9bffdc* self, bool force) {
    if (OwnerLifeIs<fn_71008cb078>(self) || OwnerLifeIs<fn_71008ddcf0>(self))
        return;
    if (!force && !fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self)) && !fn_7100df255c(g_unk_7101d47310))
        return;

    UnkNavNodeMap_l9bffdc map(10, 0x15);
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x968.field_0xe8) {
        CStrId scenario = fn_71011a2fbc(manager->field_0x608);
        CBlackboard* blackboard = &manager->field_0x968;
        const UnkQualifiedId& qid = NodesQualifiedId();
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, scenario))
            ReadNodeMap(section, qid.id, &map);

        UnkNavActor_l9bffdc* actor = self->actor;
        CRntVector<u64>* entry = map.Find(actor->name);
        if (self->field_0x20 && actor->field_0x1b4 && self->current && self->field_0x1b8) {
            CRntVector<u64> nodes(0xf);
            UnkNavPoint_l9bffdc pos = self->current->vf_0x28();
            if (!NearlyEqual(pos.x, g_unk_7101647780.x) || !NearlyEqual(pos.y, g_unk_7101647780.y)) {
                UnkTypedValue v;
                UnkSubId_7100fc4b34 key = PositionKey(self->actor->name);
                CBlackboard::CSection* section = fn_71010472e0(blackboard, scenario);
                if (section && !manager->field_0x968.field_0xe8) {
                    v.type = fn_7100024504();
                    v.value = &pos;
                    fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&key.id), &v);
                }
            }
            CollectNodes(self->current, nodes);
            if (self->selected)
                CollectNodes(self->selected, nodes);
            if (entry == map.End())
                fn_71009c478c(map.Raw(), &self->actor->name, &nodes);
            else
                *entry = nodes;
            WriteNodeMap(manager, scenario, &map);
            nodes.Free();
        } else if (entry != map.End()) {
            fn_71009c09a8(map.Raw(), entry);
            WriteNodeMap(manager, scenario, &map);
        }
    }
    FreeNodeMap(map);
}
