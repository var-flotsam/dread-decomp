// CFreezeRoomComponent (tools/tu_map.py unit 1537): fn_7100a232d4 keeps the blackboard's list of the
// rooms that freeze ("FREEZE_ROOMS", in the current scenario's section) up to date: the owner's name
// is added while the room is active and not cooling down, and removed once it is not. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkCloneHelpers.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/CCollectionType.h"
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityShinespark.h"
#include "game/CActorComponent.h"
#include "game/CBlackboard.h"
#include "game/CScenario.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTypedValue.h"
#include "types.h"

using base::global::CRntVector;
using base::global::CStrId;
using base::reflection::UnkReflValue;
using base::reflection::UnkTypedRef;

struct CFreezeRoomComponent;
extern UnkPropertyEx<> g_unk_7172745bc0;  // FREEZE_ROOMS

struct UnkFreezeOwner_la232d4 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
    u8 pad_0x38[0x1b4 - 0x38];
    bool field_0x1b4;
};

struct UnkFreezeRoom_la232d4 {
    u8 pad_0x0[0x18];
    UnkFreezeOwner_la232d4* owner;  // 0x18
    bool enabled;                   // 0x20
    u8 pad_0x21[0xbc0 - 0x21];
    f32 time;  // 0xbc0
    u8 pad_0xbc4[0xbe4 - 0xbc4];
    f32 duration;  // 0xbe4
    u8 pad_0xbe8[0xc00 - 0xbe8];
    f32 alt_duration;  // 0xc00
};

// The property's qualified id, built on first use.
static inline UnkQualifiedId& RoomsId() {
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_7172745bc0);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_7172745bc0), g_unk_7101d470e0);
    return qid;
}

// A section's value slot past the last one.
static inline UnkReflValue* End(UnkPropertyArrays& properties) {
    return &static_cast<UnkReflValue*>(properties.values)[properties.count];
}

// The value stored under `key`, or End().
static inline UnkReflValue* Find(UnkPropertyArrays& properties, const CStrId& key) {
    for (u64 i = 0; i < properties.count; ++i)
        if (properties.ids[i].data == key.data)
            return &static_cast<UnkReflValue*>(properties.values)[i];
    return End(properties);
}

// Copies the list stored under `key` in the section `section_key` names into `out`, if there is one.
static inline void ReadRooms(CBlackboard* blackboard, const CStrId& section_key, const UnkQualifiedId& key,
                             CRntVector<CStrId>* out) {
    CBlackboard::CSection* section = fn_7101047270(blackboard, section_key);
    if (!section)
        return;
    UnkPropertyArrays& properties = section->data.properties;
    UnkReflValue* value = Find(properties, key.id);
    if (value == End(properties))
        return;
    const char* type = value->type;
    if (!type)
        return;
    void* p = value->field_0x14 == 1 || *reinterpret_cast<const u64*>(type + 0x18) > 8 ? value->field_0x0 : value;
    if (!p)
        return;
    const base::reflection::CType* t = &g_unk_7101d5c998;
    UnkTypedRef dst{&g_unk_7101d5c998, out};
    UnkTypedRef src;
    fn_71000a16e0(&src, type, p);
    fn_710009d7a8(t, &dst, &src);
}

// Stores the list under `key` in the section `section_key` names (unless the blackboard is locked).
static inline void WriteRooms(CBlackboard* blackboard, const CStrId& section_key, UnkQualifiedId& key,
                              CRntVector<CStrId>* rooms) {
    if (CBlackboard::CSection* section = fn_71010472e0(blackboard, section_key)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = &g_unk_7101d5c998;
            v.value = rooms;
            fn_7100332c80(section, reinterpret_cast<UnkPropertyName*>(&key), &v);
        }
    }
}

// Appends a copy of `name`, growing the array to the next multiple of its step (the ids are moved over).
static inline void AddRoom(CRntVector<CStrId>& rooms, const CStrId& name) {
    u32 n = rooms.count;
    u32 old_capacity = rooms.capacity;
    u32 new_capacity = old_capacity;
    if (old_capacity <= n && rooms.step != 0)
        new_capacity = (n / rooms.step + 1) * rooms.step;
    if ((s32)n >= (s32)new_capacity)
        for (;;) {
        }
    if (old_capacity < new_capacity) {
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

// Removes the id at `index`, moving the later ones down.
static inline void EraseRoom(CRntVector<CStrId>& rooms, u32 index) {
    if (index < rooms.count) {
        rooms.data[index].~CStrId();
        for (u32 i = index; i < rooms.count - 1; ++i) {
            CStrId::MoveConstructReloading(&rooms.data[i], rooms.data[i + 1]);
            rooms.data[i + 1].~CStrId();
        }
        rooms.count = rooms.count - 1;
    }
}

static inline void RemoveRoom(CRntVector<CStrId>& rooms, const CStrId& name) {
    for (u64 i = 0; i < rooms.count; ++i) {
        if (rooms.data[i] == name) {
            EraseRoom(rooms, i);
            return;
        }
    }
}

// The game manager's flag that picks the alternative parameters.
// The id slot holding `name`, or the slot past the last one.
static inline CStrId* FindRoom(CRntVector<CStrId>& rooms, const CStrId& name) {
    for (u64 i = 0; i < rooms.count; ++i)
        if (rooms.data[i] == name)
            return &rooms.data[i];
    return &rooms.data[rooms.count];
}

static inline bool UsesAltParams() { return reinterpret_cast<const u8*>(g_unk_7101d47310)[0x2441]; }

// Whether the room is still cooling down: part of its duration has passed and not all of it.
static inline bool IsCooling(UnkFreezeRoom_la232d4* self) {
    f32 duration = UsesAltParams() ? self->alt_duration : self->duration;
    if (!(duration <= 0.0f)) {
        f32 t = self->time / duration;
        f32 remaining = 1.0f - (!(t >= 1.0f) ? t : 1.0f);
        return !(t <= 0.0f) && !(remaining >= 1.0f);
    }
    return false;
}

void fn_7100a232d4(CFreezeRoomComponent* component) {
    UnkFreezeRoom_la232d4* self = reinterpret_cast<UnkFreezeRoom_la232d4*>(component);
    CRntVector<CStrId> rooms(0xf);
    UnkGameManager* manager = g_unk_7101d47310;
    if (!manager->field_0x968.field_0xe8) {
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId scenario = fn_71011a2fbc(manager->field_0x608);
        ReadRooms(blackboard, scenario, RoomsId(), &rooms);
        UnkFreezeOwner_la232d4* owner = self->owner;
        CStrId* it = FindRoom(rooms, owner->name);
        if (self->enabled && owner->field_0x1b4) {
            if (it == &rooms.data[rooms.count] &&
                !(fn_710024fb2c(reinterpret_cast<UnkActivatable_b48*>(self)) && IsCooling(self))) {
                AddRoom(rooms, self->owner->name);
                WriteRooms(blackboard, scenario, RoomsId(), &rooms);
            }
        } else if (it != &rooms.data[rooms.count]) {
            RemoveRoom(rooms, owner->name);
            WriteRooms(blackboard, scenario, RoomsId(), &rooms);
        }
    }
    rooms.Free();
}
