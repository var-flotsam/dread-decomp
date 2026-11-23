// CMinimapData's unit (2563): for every door, records the heat and freeze rooms that hold a point just in
// front of and just behind it (150 units to either side of the door's two frames).
// Names and layouts are placeholders.
#include "../../../src/game/CMinimapData/UnkStrIdIterBeginEnd_A6_b75.h"
#include "base/UnkCloneRecords.h"
#include "base/global/CRntSmallDictionary.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU2563.h"
#include "game/UnkTU365.h"
#include "types.h"

using base::global::CStrId;
using base::math::CVector2D;
using base::reflection::CClass;
using base::reflection::UnkReflObject;

// A door's record: the two frames' bounds and the rooms the door leads into.
struct UnkDoorData_le4466c {
    u8 unk_0x0[0x8];
    CVector2D frontMin;  // 0x8
    CVector2D frontMax;  // 0x10
    CVector2D backMin;   // 0x18
    CVector2D backMax;   // 0x20
    u8 unk_0x28[0x10];
    base::UnkStrIdVec rooms;  // 0x38

    ~UnkDoorData_le4466c() { fn_7100e4a1d0(reinterpret_cast<UnkRecord60*>(this)); }
};
static_assert(sizeof(UnkDoorData_le4466c) == 0x60, "layout");

struct UnkMinimapDoors_le4466c {
    u8 unk_0x0[0x6140];
    base::global::CRntSmallDictionary<CStrId, UnkDoorData_le4466c> mapDoors;  // 0x6140
};

// A room's area: vf_0x50 tells whether a point lies inside it.
struct UnkRoomShape_le4466c {
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
    virtual bool vf_0x50(const CVector2D* point);
};

struct UnkRoomEntity_le4466c {
    u8 unk_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkRoomTracker_le4466c {
    u8 unk_0x0[0x18];
    UnkRoomEntity_le4466c* entity;  // 0x18
    u8 unk_0x20[0x10];
    UnkRoomShape_le4466c* shape;  // 0x30
};

struct UnkRoomComponent_le4466c : UnkReflObject {
    u8 unk_0x8[0x10];
    void* owner;  // 0x18
};

struct UnkRoomHandle_le4466c {
    u8 unk_0x0[0x10];
    UnkRoomComponent_le4466c* component;  // 0x10
};

struct UnkRoomHandleList_le4466c {
    UnkRoomHandle_le4466c** data;
    u32 count;
};

// The live heat and freeze room components.
UnkRoomHandleList_le4466c* fn_7100a26740();
UnkRoomHandleList_le4466c* fn_7100a23208();

using UnkRoomTrackers_le4466c = base::global::CRntVector<UnkRoomTracker_le4466c*>;
using UnkDoorIter_le4466c = UnkStrIdIterValue_A6_b75<UnkDoorData_le4466c>;

// Appends the tracker of every component in the list that is of the class GetClass returns.
template <const CClass* (*GetClass)()>
inline void CollectRoomTrackers_le4466c(UnkRoomTrackers_le4466c* trackers, UnkRoomHandleList_le4466c* list) {
    for (UnkRoomHandle_le4466c **it = list->data, **end = it + list->count; it != end; ++it) {
        UnkRoomHandle_le4466c* handle = *it;
        if (!handle)
            continue;
        UnkRoomComponent_le4466c* component = handle->component;
        if (!component || !fn_710009d50c(component->vf_0x0(), GetClass()))
            continue;
        UnkRoomTracker_le4466c* tracker =
            reinterpret_cast<UnkRoomTracker_le4466c*>(fn_710028aca8(component->owner));
        if (tracker)
            trackers->PushBack(tracker);
    }
}

inline UnkDoorData_le4466c* DoorOf_le4466c(UnkDoorIter_le4466c& it) {
    return it.state == 2 ? it.ref.element : &it.value.element;
}

// Adds room to the door's rooms unless it is there already.
inline void AddRoom_le4466c(base::UnkStrIdVec* rooms, const CStrId& room) {
    u32 count = rooms->count;
    for (u32 i = 0; i < count; ++i)
        if (rooms->data[i] == room)
            return;
    rooms->PushBack(room);
}

void fn_7100e4466c(UnkMinimapDoors_le4466c* self) {
    UnkRoomTrackers_le4466c trackers(0xf);
    trackers.Reserve(10);
    CollectRoomTrackers_le4466c<fn_7100a259f4>(&trackers, fn_7100a26740());
    CollectRoomTrackers_le4466c<fn_7100a226c4>(&trackers, fn_7100a23208());

    base::global::CRntSmallDictionary<CStrId, UnkDoorData_le4466c>& doors = self->mapDoors;
    const CVector2D reach = {150.0f, 0.0f};
    for (UnkDoorIter_le4466c it(doors.keys, doors.values); it.ref.key != doors.keys + doors.count;
         ++it.ref.key, ++it.ref.element) {
        UnkDoorData_le4466c* door = DoorOf_le4466c(it);
        CVector2D front;
        front.x = (door->frontMin.x + door->frontMax.x) * 0.5f - reach.x;
        front.y = (door->frontMin.y + door->frontMax.y) * 0.5f - reach.y;
        CVector2D back;
        back.x = (door->backMin.x + door->backMax.x) * 0.5f + reach.x;
        back.y = (door->backMin.y + door->backMax.y) * 0.5f + reach.y;

        CStrId frontRoom(base::global::UnkStrIdInline(), nullptr, false);
        CStrId backRoom(base::global::UnkStrIdInline(), nullptr, false);
        for (UnkRoomTracker_le4466c **t = trackers.data, **end = t + trackers.count; t != end; ++t) {
            UnkRoomTracker_le4466c* tracker = *t;
            UnkRoomShape_le4466c* shape = tracker->shape;
            if (!shape)
                continue;
            if (shape->vf_0x50(&front)) {
                frontRoom = tracker->entity->name;
                AddRoom_le4466c(&DoorOf_le4466c(it)->rooms, frontRoom);
            }
            if (shape->vf_0x50(&back)) {
                backRoom = tracker->entity->name;
                AddRoom_le4466c(&DoorOf_le4466c(it)->rooms, backRoom);
            }
        }
    }

    if (trackers.data)
        trackers.allocator->vf_0x48(trackers.data);
}
