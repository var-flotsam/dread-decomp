// CBreakableTileGroupComponent's unit (tools/tu_map.py): reads the "MINIMAP_TILES" map (actor name ->
// the minimap states of its tiles) from the current scenario's blackboard section, replaces the owner's
// entry with the group's tiles as they are now (or removes it when the group shows on no minimap), and
// stores the map back unless the blackboard is locked.
// Names and layouts are placeholders.
#include <string.h>

#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CBreakableTileGroup.h"
#include "game/CScenario.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTUProperty.h"
#include "game/UnkTypedValue.h"
#include "types.h"

#include "../../../src/game_5821e4/UnkStaticInit.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

extern UnkPropertyEx<> g_unk_7172711588;
extern unk_static_init::UnkEnumType g_unk_71727122c0;

struct UnkEntryInfo;
UnkEntryInfo* fn_7100ca1ea8(void* entry, u8 sub);
f32 fn_7100c9e290(UnkQueryObj* obj);

struct UnkTileElem_ovb0028;
using UnkTileElemVector_ovb0028 = CRntVector<UnkTileElem_ovb0028>;
struct UnkTileStatesMap_ovb0028;
UnkTileElemVector_ovb0028* fn_71007616e8(UnkTileStatesMap_ovb0028* self, UnkTileElemVector_ovb0028* it);
UnkTileElemVector_ovb0028* fn_7100760da8(UnkTileStatesMap_ovb0028* self, const CStrId* key,
                                         const UnkTileElemVector_ovb0028* value);

// CBreakableTileGroupComponent::SMinimapTileState.
struct UnkMinimapTileState_l759440 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 state;
};
using UnkMinimapTiles_l759440 = CRntVector<UnkMinimapTileState_l759440>;

// CBreakableTileGroupComponent::TActorTileStatesMap: the states of each actor's tiles by actor name.
struct UnkActorTileStates_l759440 {
    UnkMinimapTiles_l759440* values = nullptr;
    CStrId* keys = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;
    u32 flags = 0xe;
    base::IAllocator* allocator = GetDefaultAllocator();

    inline ~UnkActorTileStates_l759440() {
        if (keys) {
            for (u64 i = 0; i < count; ++i)
                keys[i].Reset();
        }
        if (values) {
            for (u64 i = 0; i < count; ++i)
                values[i].Free();
        }
        count = 0;
        if (keys)
            allocator->vf_0x48(keys);
        if (values)
            allocator->vf_0x48(values);
    }

    UnkMinimapTiles_l759440* End() { return &values[count]; }
    UnkMinimapTiles_l759440* Find(const CStrId& key) {
        u64 i = 0;
        for (; i < count; ++i) {
            if (keys[i].data == key.data)
                break;
        }
        return &values[i];
    }
    void Remove(const CStrId& key) {
        UnkMinimapTiles_l759440* it = Find(key);
        if (it != End())
            fn_71007616e8(reinterpret_cast<UnkTileStatesMap_ovb0028*>(this),
                          reinterpret_cast<UnkTileElemVector_ovb0028*>(it));
    }
};

struct UnkTile_l759440 {
    u8 pad_0x0[0x28];
    u32 field_0x28;
    u32 field_0x2c;
    u8 pad_0x30[8];
    u32 state;  // 0x38
};

struct UnkTileGroup_l759440 {
    u8 pad_0x0[0x40];
    u32 tile_count;
};

struct UnkTileActor_l759440 {
    u8 pad_0x0[0x30];
    CStrId name;
    u8 pad_0x38[0x1b4 - 0x38];
    bool on_minimap;  // 0x1b4
};

struct UnkTileGroupComponent_l759440 {
    u8 pad_0x0[0x18];
    UnkTileActor_l759440* actor;  // 0x18
    bool enabled;                 // 0x20
    u8 pad_0x21[0xa0 - 0x21];
    UnkTileGroup_l759440* group;  // 0xa0
};

// The property's id, built on first use.
inline const CStrId& PropertyId(UnkPropertyEx<>& property) {
    UnkQualifiedId& id = ItemQualifiedId(property);
    if (id.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&id, ItemSource(property), g_unk_7101d470e0);
    return id.id;
}

// Reads the value under `name` in `section` into the map.
inline void ReadStates(CBlackboard::CSection* section, const CStrId& name, UnkActorTileStates_l759440* out) {
    UnkPropertyArrays& properties = section->data.properties;
    UnkReflValue* values = static_cast<UnkReflValue*>(properties.values);
    u64 i = 0;
    for (; i < properties.count; ++i) {
        if (properties.ids[i].data == name.data)
            break;
    }
    UnkReflValue* v = &values[i];
    if (v == &values[properties.count])
        return;
    const char* type = v->type;
    if (!type)
        return;
    void* p = v->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? v->field_0x0 : v;
    if (!p)
        return;
    const CType* map_type = reinterpret_cast<const CType*>(&g_unk_71727122c0);
    UnkTypedRef dst{map_type, out};
    UnkTypedRef src;
    fn_71000a16e0(&src, type, p);
    fn_710009d7a8(map_type, &dst, &src);
}

// Stores the map under the property in the section `section_id` names, unless the blackboard is locked.
inline void StoreStates(CBlackboard* blackboard, const CStrId& section_id, UnkActorTileStates_l759440* states) {
    const CStrId& name = PropertyId(g_unk_7172711588);
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue value;
            value.type = reinterpret_cast<const CType*>(&g_unk_71727122c0);
            value.value = states;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<CStrId*>(&name)), &value);
        }
    }
}

// Copies `src` over `dst`.
inline void CopyTiles(UnkMinimapTiles_l759440* dst, const UnkMinimapTiles_l759440* src) {
    if (dst == src)
        return;
    dst->Clear();
    dst->Reserve(src->capacity);
    memcpy(dst->data, src->data, (u64)src->count * sizeof(UnkMinimapTileState_l759440));
    dst->count = src->count;
}

// Appends a zeroed state and returns it.
inline UnkMinimapTileState_l759440& AddState(UnkMinimapTiles_l759440& tiles) {
    u32 n = tiles.count + 1;
    tiles.Reserve(n);
    memset(&tiles.data[tiles.count], 0, (u64)(n - tiles.count) * sizeof(UnkMinimapTileState_l759440));
    tiles.count = n;
    UnkMinimapTileState_l759440* end = &tiles.data[n];
    return n ? end[-1] : *end;
}

void fn_7100759440(UnkTileGroupComponent_l759440* self) {
    UnkActorTileStates_l759440 states;
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    CStrId section_id = fn_71011a2fbc(manager->field_0x608);
    const CStrId& name = PropertyId(g_unk_7172711588);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        ReadStates(section, name, &states);

    bool shown = self->enabled && self->actor->on_minimap && self->group;
    UnkMinimapTiles_l759440* it = states.Find(self->actor->name);
    if (shown) {
        UnkMinimapTiles_l759440 tiles(0xf);
        for (u32 i = 0; i < self->group->tile_count; ++i) {
            UnkTile_l759440* tile = reinterpret_cast<UnkTile_l759440*>(fn_7100ca1ea8(self->group, i));
            UnkMinimapTileState_l759440& state = AddState(tiles);
            state.field_0x8 = fn_7100c9e960(reinterpret_cast<UnkQueryObj*>(tile));
            if (tile->state < 2)
                state.state = tile->state;
            else if (tile->state == 2)
                state.state = fn_7100c9e290(reinterpret_cast<UnkQueryObj*>(tile)) <= 0.0f ? 2 : 1;
            state.field_0x0 = tile->field_0x28;
            state.field_0x4 = tile->field_0x2c;
        }
        if (it == states.End())
            fn_7100760da8(reinterpret_cast<UnkTileStatesMap_ovb0028*>(&states), &self->actor->name,
                          reinterpret_cast<UnkTileElemVector_ovb0028*>(&tiles));
        else
            CopyTiles(it, &tiles);
        StoreStates(blackboard, section_id, &states);
        tiles.Free();
    } else {
        if (it != states.End())
            states.Remove(self->actor->name);
        StoreStates(blackboard, section_id, &states);
    }
}
