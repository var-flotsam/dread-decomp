// CHeatRoomComponent unit (tools/tu_map.py unit 1538): refreshes the list of heat rooms the
// blackboard keeps ("HEAT_ROOMS", in the current scenario's section). The list is read; an enabled
// room that is not listed is added unless it is cooling down, a disabled one that is listed is
// removed, and the list is stored again. Names and layouts are placeholders.
#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CCollectionType.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CHeatRoomComponent.h"
#include "game/CScenario.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;

extern UnkPropertyEx<> g_unk_71727461c8;  // "HEAT_ROOMS"

struct UnkActivatable_b48;

// The game manager's flag at +0x2441 (which of the two cooling times applies). Stands for a field
// UnkGameManager (include/game/UnkGameManager.h) lacks: bool field_0x2441 inside pad_0x2278.
struct UnkGameManagerView_la26828 {
    u8 pad_0x0[0x2441];
    bool field_0x2441;
};

struct UnkHeatRoomEntity_la26828 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x1b4 - 0x38];
    bool field_0x1b4;
};

struct UnkHeatRoom_la26828 {
    u8 pad_0x0[0x18];
    UnkHeatRoomEntity_la26828* entity;  // 0x18
    bool enabled;                       // 0x20
    u8 pad_0x21[0xbc0 - 0x21];
    f32 field_0xbc0;
    u8 pad_0xbc4[0xbe4 - 0xbc4];
    f32 field_0xbe4;
    u8 pad_0xbe8[0xc04 - 0xbe8];
    f32 field_0xc04;
};
static_assert(__builtin_offsetof(UnkHeatRoom_la26828, field_0xc04) == 0xc04, "layout");

// The list's qualified id, built on first use.
static inline const UnkQualifiedId& HeatRoomsId() {
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_71727461c8);
    if ((u64)qid.id.data == (u64)g_unk_7101d47108 + 0x10)
        fn_7100ae9a10(&qid, ItemSource(g_unk_71727461c8), g_unk_7101d470e0);
    return qid;
}

// The list as the blackboard's reader converts it (fn_710006d11c's shape, on the list's type).
struct UnkHeatRoomsTag_la26828;
template <>
struct base::reflection::UnkFieldTraits<UnkHeatRoomsTag_la26828> {
    static const CType* type() { return &g_unk_7101d5c998; }
};

// Adds a copy of `name` at the end, growing the array as CRntVector::PushBackCopy does; the names
// are moved over leaving empty ids behind.
static inline void PushName(CRntVector<CStrId>& rooms, const CStrId& name) {
    u32 n = rooms.count;
    u32 old_capacity = rooms.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && rooms.step != 0)
        new_capacity = (n / rooms.step + 1) * rooms.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (rooms.capacity < new_capacity) {
        CStrId* p = static_cast<CStrId*>(rooms.allocator->vf_0x40((u64)new_capacity * sizeof(CStrId), 8, rooms.flags));
        if (rooms.data) {
            CStrId* src = rooms.data;
            CStrId* dst = p;
            for (u32 i = 0; i < rooms.count; ++i, ++dst, ++src) {
                CStrId::MoveConstructReloading(dst, *src);
                src->~CStrId();
            }
            rooms.allocator->vf_0x48(rooms.data);
        }
        rooms.data = p;
        rooms.capacity = new_capacity;
    }
    new (&rooms.data[rooms.count]) CStrId(name);
    ++rooms.count;
}

// Releases the name at `index` and moves the later ones down.
static inline void EraseName(CRntVector<CStrId>& rooms, u32 index) {
    if (index < rooms.count) {
        rooms.data[index].~CStrId();
        for (u32 i = index; i < rooms.count - 1; ++i) {
            CStrId::MoveConstructReloading(rooms.data + i, *(rooms.data + i + 1));
            (rooms.data + i + 1)->~CStrId();
        }
        rooms.count = rooms.count - 1;
    }
}

// Whether the room is still cooling down: the time at +0xbc0 against the cooling time in force.
static inline bool IsCooling(UnkHeatRoom_la26828* self) {
    bool alternate = reinterpret_cast<UnkGameManagerView_la26828*>(g_unk_7101d47310)->field_0x2441;
    f32 duration = alternate ? self->field_0xc04 : self->field_0xbe4;
    if (!(duration > 0.0f))
        return false;
    f32 t = self->field_0xbc0 / duration;
    f32 heat = 1.0f - (!(t >= 1.0f) ? t : 1.0f);
    return t > 0.0f && heat < 1.0f;
}

// The listed name equal to `name`, or the end of the list.
static inline CStrId* FindName(CRntVector<CStrId>& rooms, const CStrId& name) {
    u32 n = rooms.count;
    CStrId* data = rooms.data;
    for (u64 i = 0; i < n; ++i)
        if (data[i] == name)
            return &data[i];
    return &data[n];
}

// Adds an enabled room that is not listed (unless it is cooling down) or removes a disabled one
// that is; whether the list changed.
static inline bool UpdateRooms(UnkHeatRoom_la26828* self, CRntVector<CStrId>& rooms) {
    UnkHeatRoomEntity_la26828* entity = self->entity;
    CStrId* it = FindName(rooms, entity->name);
    if (self->enabled && entity->field_0x1b4) {
        if (it != rooms.data + rooms.count)
            return false;
        if (fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self)) && IsCooling(self))
            return false;
        PushName(rooms, self->entity->name);
        return true;
    }
    if (it == rooms.data + rooms.count || !entity->field_0x1b4)
        return false;
    u32 i = 0;
    for (; i < rooms.count; ++i)
        if (rooms.data[i] == entity->name)
            break;
    EraseName(rooms, i);
    return true;
}

void fn_7100a26828(UnkHeatRoom_b49* room) {
    auto* self = reinterpret_cast<UnkHeatRoom_la26828*>(room);
    CRntVector<CStrId> rooms(0xf);
    UnkGameManager* manager = g_unk_7101d47310;
    if (manager->field_0x968.field_0xe8)
        return;
    CBlackboard* blackboard = &manager->field_0x968;
    {
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        {
            const UnkQualifiedId& key = HeatRoomsId();
            if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
                base::refl_GetKeyedValue<UnkHeatRoomsTag_la26828>(
                    reinterpret_cast<char*>(section), reinterpret_cast<const base::UnkStrIdMember*>(&key.id), &rooms);
        }
        if (UpdateRooms(self, rooms)) {
            const UnkQualifiedId& key = HeatRoomsId();
            if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_id)) {
                if (!blackboard->field_0xe8) {
                    UnkTypedValue v;
                    v.type = &g_unk_7101d5c998;
                    v.value = &rooms;
                    fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(const_cast<UnkQualifiedId*>(&key)), &v);
                }
            }
        }
    }
    rooms.Free();
}
