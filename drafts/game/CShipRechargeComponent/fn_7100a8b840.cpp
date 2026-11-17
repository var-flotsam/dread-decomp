// CShipRechargeComponent's unit (tools/tu_map.py unit 1598): after the base step, saves the game at the
// ship (the owner's name, "ST_SamusShip", with this component and its callback), then starts the save
// station's loop sound unless it is already playing. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2553.h"
#include "game/sound/UnkSoundDesc.h"
#include "game/sound/UnkSoundHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;

struct UnkSaveGamePair_b54 {
    u64 first;
    u64 second;
};

// Called back once the game is saved.
void fn_7100a8c670();

// The handle's view of the reference block.
struct UnkSoundRefView_la8b840 {
    void* sound;
    s64 serial;
};

static inline bool Playing(const UnkSoundHandle& handle) {
    const UnkSoundRefView_la8b840* ref = reinterpret_cast<const UnkSoundRefView_la8b840*>(handle.ref);
    return ref && ref->serial == handle.serial && handle.serial != -1 && ref->sound;
}

struct UnkShipRechargeOwner_la8b840 {
    u8 pad_0x0[0x30];
    CStrId name;  // 0x30
};

struct UnkShipRecharge_la8b840;
struct UnkShipRechargeVtable_la8b840 {
    u8 pad_0x0[0x2b0];
    void (*vf_0x2b0)(UnkShipRecharge_la8b840* self);
};

struct UnkShipRecharge_la8b840 {
    UnkShipRechargeVtable_la8b840* vtable;
    u8 pad_0x8[0x28];
    UnkShipRechargeOwner_la8b840* owner;  // 0x30
    u8 pad_0x38[0x540 - 0x38];
    UnkSoundHandle loop_sound;  // 0x540
};

// The id's text as a string, counting the read.
static inline CRntString* NameString(const CStrId& id) {
    ++id.data->reads;
    return const_cast<CRntString*>(&id.data->String());
}

void fn_7100a8b840(UnkShipRecharge_la8b840* self) {
    self->vtable->vf_0x2b0(self);
    {
        UnkGameManager* manager = g_unk_7101d47310;
        UnkShipRechargeOwner_la8b840* owner = self->owner;
        CRntString* name = NameString(owner->name);
        CRntString tag("ST_SamusShip");
        fn_7100dfd174(manager, name, &tag, true, 1,
                      UnkSaveGamePair_b54{reinterpret_cast<u64>(self), reinterpret_cast<u64>(&fn_7100a8c670)});
    }
    if (Playing(self->loop_sound))
        return;
    static CFilePathStrId s_path("props/savestation/savestation_loop.wav", true);
    static UnkSoundDesc s_desc;
    s_desc.kind.field_0x0 = 0;
    s_desc.path = s_path;
    s_desc.field_0x38 = 0.6f;
    s_desc.field_0x3c = 500.0f;
    s_desc.field_0x40 = 3000.0f;
    s_desc.field_0x44 = 1.0f;
    s_desc.field_0x6c = true;
    s_desc.field_0x6d = true;
    s_desc.field_0x4c = 0.25f;
    self->loop_sound = fn_7100d7c374(reinterpret_cast<UnkEntityWithSound*>(self->owner), &s_desc, &g_unk_7101d470e0,
                                     false, true, true);
}
