// CAudioComponent unit: plays a sound request (with the other rumble kind, fn_7100d7c6c8) through a slot of the component's slot array. A slot
// already holding the request's file while it plays is returned as it is; otherwise, when the owner is
// the player (or the game lets others play), the request is resolved (group and bus defaults, the bus
// volume, the group level), played (fn_710071a7dc) with a rumble when asked, and stored in a free slot
// or a new one. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAudioComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU101.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2656.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

struct UnkSoundObject_b50;
struct UnkSoundRef {
    UnkSoundObject_b50* object;
    s64 serial;
};

struct UnkHandleTarget;
struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

struct UnkEntityWithSound;
UnkHandle fn_7100d7c6c8(UnkEntityWithSound* self, void* a, bool b, bool c, bool d);

struct UnkAudio_b50;
struct UnkSoundRequest_b50;
UnkSoundHandle fn_710071a7dc(UnkAudio_b50* self, const UnkSoundRequest_b50* request);

struct UnkStruct_710071faf4;
void fn_710071faf4(UnkStruct_710071faf4* self, const UnkStruct_710071faf4* other);
struct UnkAudioSound_ovb0015;
void fn_710071f878(UnkAudioSound_ovb0015* self);
void fn_710010602c(UnkSoundObject_b50* object, u32 value);

// The object a handle names while it is still the one the handle was made for.
inline UnkSoundObject_b50* GetSoundObject_b550(const UnkSoundHandle* handle) {
    if (handle->ref && handle->serial != -1 && handle->ref->serial == handle->serial)
        return handle->ref->object;
    return nullptr;
}

// A position, copied word by word.
struct UnkSoundPos_b550 {
    f32 x, y, z;
    UnkSoundPos_b550(const UnkSoundPos_b550& o) : x(o.x), y(o.y), z(o.z) {}
};

// A sound request (0x70 bytes). A group or bus of kNone takes the request's default.
struct UnkSoundRequest_b550 {
    static constexpr s32 kNone = 0x7fffffff;

    s32 group;
    s32 bus;
    s32 default_group;
    s32 default_bus;
    s32 level;
    CStrId name;                // 0x18
    CFilePathStrId path;        // 0x20
    UnkSoundPos_b550 position;  // 0x28
    u32 field_0x34;
    f32 volume;  // 0x38
    u8 field_0x3c[0x30];
    bool field_0x6c;
    u8 field_0x6d;
    bool rumble;  // 0x6e
};
static_assert(sizeof(UnkSoundRequest_b550) == 0x70, "UnkSoundRequest_b550 layout");

void fn_710071b39c(UnkSoundRequest_b550* self, const UnkSoundRequest_b550* other);

// A sound slot (0xd0 bytes).
struct UnkAudioSlot_b550 {
    u8 pad_0x0[0x8];
    UnkSoundRequest_b550 request;  // 0x8
    UnkSoundHandle handle;         // 0x78
    UnkHandle rumble;              // 0x88
    u8 pad_0x98[0x28];
    bool stopped;  // 0xc0
    u32 flags;     // 0xc4
    f32 volume;    // 0xc8
    u8 pad_0xcc[0x4];

    // The sound the slot's handle names while it is still the one the handle was made for.
    UnkSoundObject_b50* Sound() const {
        if (handle.ref && handle.ref->serial == handle.serial && handle.serial != -1)
            return handle.ref->object;
        return nullptr;
    }
};
static_assert(sizeof(UnkAudioSlot_b550) == 0xd0, "UnkAudioSlot_b550 layout");

void fn_710071fcb0(UnkAudioSlot_b550* self);

struct UnkAudioSlots_b550 {
    UnkAudioSlot_b550* data;
    u32 count;
    u32 capacity;
    u32 step;
    bool field_0x14;
    u32 flags;
    base::UnkAllocator* allocator;

    UnkAudioSlot_b550* Find(const CFilePathStrId& path) {
        for (u32 i = 0; i < count; ++i)
            if (data[i].request.path == path)
                return &data[i];
        return nullptr;
    }
};

struct UnkAudio_b550 {
    virtual void vf_0();
    virtual void vf_1();
    virtual void vf_2();
    virtual void vf_3();
    virtual void vf_4();
    virtual void vf_5();
    virtual void vf_6();
    virtual void vf_7();
    virtual void vf_8();
    virtual void vf_9();
    virtual void vf_10();
    virtual void vf_11();
    virtual void vf_12();
    virtual void vf_13();
    virtual void vf_14();
    virtual void vf_15();
    virtual void vf_16();
    virtual void vf_17();
    virtual void vf_18();
    virtual void vf_19();
    virtual void vf_20();
    virtual void vf_21();
    virtual void vf_22();
    virtual void vf_23();
    virtual void vf_24();
    virtual void vf_25();
    virtual void vf_26();
    virtual void vf_27();
    virtual void vf_28();
    virtual void vf_29();
    virtual void vf_30();
    virtual void vf_31();
    virtual void vf_32();
    virtual void vf_33();
    virtual void vf_34();
    virtual void vf_35();
    virtual void vf_36();
    virtual void vf_37();
    virtual void vf_38();
    virtual void vf_39();
    virtual void vf_40();
    virtual void vf_41();
    virtual void vf_42();
    virtual void vf_43();
    virtual void vf_44();
    virtual void vf_45();
    virtual void vf_46();
    virtual bool vf_47();  // whether the component can play

    u8 pad_0x8[0x28];
    UnkEntityWithSound* owner;  // 0x30
    u8 pad_0x38[0x10];
    UnkAudioSlots_b550 slots;  // 0x48
    u8 pad_0x70[0x7c];
    f32 bus_volumes[11];  // 0xec
    s32 group_levels[8];  // 0x118
};
static_assert(__builtin_offsetof(UnkAudio_b550, bus_volumes) == 0xec, "UnkAudio_b550 layout");
static_assert(__builtin_offsetof(UnkAudio_b550, group_levels) == 0x118, "UnkAudio_b550 layout");

// The game manager's reference to the other playable entity (game/UnkGameManager.h leaves it as padding).
struct UnkGameManagerObjRef_b550 {
    u8 pad_0x0[0x10];
    void* obj;
};
struct UnkGameManagerRefs_b550 {
    u8 pad_0x0[0x29a8];
    UnkGameManagerObjRef_b550* field_0x29a8;
};

UnkSoundHandle fn_710071b550(UnkEntitySoundComponent* component, const u32* params, bool a, bool b, bool c) {
    UnkAudio_b550* self = reinterpret_cast<UnkAudio_b550*>(component);
    const UnkSoundRequest_b550* request = reinterpret_cast<const UnkSoundRequest_b550*>(params);
    if (!self->vf_47())
        return g_unk_7108b6c460;

    UnkAudioSlot_b550* slot = self->slots.Find(request->path);
    if (slot && slot->Sound() && (slot->flags & 1) && !slot->stopped)
        return slot->handle;

    UnkGameManager* gm = g_unk_7101d47310;
    UnkPlayer* player = fn_7100dec788(gm);
    UnkGameManagerObjRef_b550* ref = reinterpret_cast<UnkGameManagerRefs_b550*>(g_unk_7101d47310)->field_0x29a8;
    void* other = ref ? ref->obj : nullptr;
    if (player != reinterpret_cast<UnkPlayer*>(self->owner)) {
        if (fn_7101053a0c(gm->field_0x1040) && !request->field_0x6c)
            return g_unk_7108b6c460;
        if (fn_7100df5c94(g_unk_7101d47310) && other != self->owner)
            return g_unk_7108b6c460;
    }

    UnkSoundRequest_b550 p(*request);
    s32 group = p.group == UnkSoundRequest_b550::kNone ? p.default_group : p.group;
    s32 bus = p.bus == UnkSoundRequest_b550::kNone ? p.default_bus : p.bus;
    p.group = group;
    p.bus = bus;
    p.default_group = group;
    p.default_bus = bus;
    f32 volume = p.volume;
    p.volume = volume * self->bus_volumes[bus];
    p.level = self->group_levels[group];

    UnkSoundHandle handle = fn_710071a7dc(reinterpret_cast<UnkAudio_b50*>(self),
                                          reinterpret_cast<const UnkSoundRequest_b50*>(&p));
    UnkHandle rumble = {nullptr, (u64)-1};
    if (GetSoundObject_b550(&handle)) {
        if (p.rumble)
            rumble = fn_7100d7c6c8(self->owner, &p, a, b, c);
        if (!b) {
            fn_7100106024(GetSoundObject_b550(&handle), nullptr);
            fn_710010602c(GetSoundObject_b550(&handle), 0);
        }
    } else if (!p.field_0x6c) {
        return handle;
    }

    UnkAudioSlot_b550* entry;
    if (slot && !slot->Sound() && (slot->flags & 1))
        entry = slot;
    else {
        // Append a new slot, moving the slots to a larger array when full.
        UnkAudioSlots_b550& slots = self->slots;
        u32 n = slots.count + 1;
        if (slots.capacity < n) {
            UnkAudioSlot_b550* data = static_cast<UnkAudioSlot_b550*>(
                slots.allocator->vf_0x40((u64)n * sizeof(UnkAudioSlot_b550), 8, slots.flags));
            if (UnkAudioSlot_b550* src = slots.data) {
                UnkAudioSlot_b550* dst = data;
                for (u32 i = 0; i < slots.count; ++i, ++dst, ++src) {
                    fn_710071faf4(reinterpret_cast<UnkStruct_710071faf4*>(dst),
                                  reinterpret_cast<UnkStruct_710071faf4*>(src));
                    fn_710071f878(reinterpret_cast<UnkAudioSound_ovb0015*>(src));
                }
                slots.allocator->vf_0x48(slots.data);
            }
            slots.data = data;
            slots.capacity = n;
        }
        for (u32 i = slots.count; i < n; ++i)
            fn_710071fcb0(&slots.data[i]);
        slots.count = n;
        UnkAudioSlot_b550* end = &slots.data[n];
        entry = n ? &end[-1] : end;
    }
    fn_710071b39c(&entry->request, &p);
    entry->handle = handle;
    entry->rumble = rumble;
    entry->stopped = false;
    entry->flags = p.field_0x6c | (a ? 4 : 0) | (b ? 8 : 0) | (c ? 0x10 : 0);
    entry->volume = volume;
    return handle;
}
