// CFreezeRoomComponent (tools/tu_map.py unit 1537): fn_7100a23b80, the step while an actor is inside.
// When the game manager accepts the actor and the room is idle, plays "ThermalRoomEnter" once (unless
// it still plays), records whether the environment manager knows the room's environment, then runs
// the base step. Names and layouts are placeholders.
#include "base/UnkAtExit.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection.h"
#include "base/global/CRntVector.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU0.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;
using base::global::UnkStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

struct UnkEntityWithSound;
UnkSoundHandle fn_7100d7c3b0(UnkEntityWithSound* self, const u32* params, bool a, bool b, bool c);
struct UnkDamageTrigger_b0119;
void fn_7100a45cf0(UnkDamageTrigger_b0119* self, void* arg, void* arg2);
void* fn_7100d8efc0(void* manager, const CStrId* name);

// A string id copied from another (retained) that takes a new one by move.
struct UnkSoundName_la23b80 {
    UnkStringData* data;

    explicit UnkSoundName_la23b80(const CStrId& id) : data(id.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    UnkSoundName_la23b80& operator=(UnkStrId&& other) {
        UnkStringPool* pool = g_unk_7101d47108;
        if (pool) {
            if (data && data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0) {
                    StringPool_Release(pool, data);
                    pool = g_unk_7101d47108;
                }
            }
        }
        data = other.data;
        other.data = &g_unk_7101d47108->empty;
        return *this;
    }
};

// The sound description the room plays, with its defaults.
struct UnkSoundDesc_la23b80 {
    u32 kind;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;  // unset in the original
    UnkSoundName_la23b80 name;  // 0x18
    CFilePathStrId path;  // 0x20
    f32 position[3];      // 0x28
    f32 field_0x34;
    f32 field_0x38;
    f32 field_0x3c;
    f32 field_0x40;
    f32 field_0x44;
    f32 field_0x48;
    f32 field_0x4c;
    f32 field_0x50;
    f32 field_0x54;
    f32 field_0x58;
    f32 field_0x5c;
    f32 field_0x60;
    f32 field_0x64;
    f32 field_0x68;
    bool field_0x6c;
    bool field_0x6d;
    bool field_0x6e;

    UnkSoundDesc_la23b80()
        : kind(0), field_0x4(0), field_0x8(0), field_0xc(0), field_0x10(0), name(g_unk_7101d470e0),
          path(g_unk_7101d470e8), position{0.0f, 0.0f, 0.0f}, field_0x34(-1.0f), field_0x38(1.0f),
          field_0x3c(500.0f), field_0x40(2000.0f), field_0x44(1.0f), field_0x48(0.0f), field_0x4c(0.0f),
          field_0x50(0.0f), field_0x54(-1.0f), field_0x58(-1.0f), field_0x5c(-1.0f), field_0x60(0.5f),
          field_0x64(5.0f), field_0x68(10.0f), field_0x6c(false), field_0x6d(false), field_0x6e(false) {}
    ~UnkSoundDesc_la23b80();  // fn_710034e0b8
};
static_assert(__builtin_offsetof(UnkSoundDesc_la23b80, field_0x6c) == 0x6c, "layout");

// The handle's view of the reference block.
struct UnkSoundRefView_la23b80 {
    void* sound;
    s64 serial;
};

static inline bool IsPlaying(const UnkSoundHandle& handle) {
    const UnkSoundRefView_la23b80* ref = reinterpret_cast<const UnkSoundRefView_la23b80*>(handle.ref);
    return ref && ref->serial == handle.serial && handle.serial != -1 && ref->sound;
}

// The environment manager, created on first use.
static inline void* EnvironmentManager() {
    void* manager = g_unk_7101d47368;
    if (!manager) {
        manager = fn_71000839a0(0x4c8, 0x3f);
        fn_7100d8ced8(manager);
        g_unk_7101d47368 = manager;
    }
    return manager;
}

struct UnkFreezeRoom_la23b80 {
    u8 pad_0x0[0x30];
    UnkEntityWithSound* entity;  // 0x30
    u8 pad_0x38[0x9b8 - 0x38];
    s32 field_0x9b8;
    u8 pad_0x9bc[0xc20 - 0x9bc];
    CFilePathStrId enter_path;  // 0xc20
    UnkSoundHandle enter_sound;  // 0xc28
    CStrId environment;          // 0xc38
    u8 pad_0xc40[0x8];
    bool has_environment;  // 0xc48
};

void fn_7100a23b80(UnkFreezeRoom_la23b80* self, void* actor, void* arg) {
    if (fn_7100df5c38(g_unk_7101d47310, actor) && self->field_0x9b8 == 0) {
        if (!IsPlaying(self->enter_sound)) {
            static UnkSoundDesc_la23b80 desc;
            desc.field_0x8 = 0;
            desc.name = UnkStrId("ThermalRoomEnter", true);
            desc.path = self->enter_path;
            desc.field_0x38 = 0.4f;
            self->enter_sound =
                fn_7100d7c3b0(self->entity, reinterpret_cast<const u32*>(&desc), false, true, true);
        }
        self->has_environment = self->environment.data != &g_unk_7101d47108->empty &&
                                fn_7100d8efc0(EnvironmentManager(), &self->environment) != nullptr;
    }
    fn_7100a45cf0(reinterpret_cast<UnkDamageTrigger_b0119*>(self), actor, arg);
}
