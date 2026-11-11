// CHydrogigaZiplineRailComponent unit (tools/tu_map.py): fn_710091d2fc reads "LUA:HYDROGIGA_DEAD" from
// the current scenario's blackboard section (through a string-id handle, fn_7100ae986c); when it is set,
// the rail is marked (+0x3d) and updated (fn_710091d4bc). Then every linked entity (+0x40, 64 bytes each,
// count +0x48; the object behind each link is +0x10 of what fn_71003b0c4c returns) has its component
// (fn_71005c3084) told the mark (fn_710091c994). Names and layouts are placeholders.
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/CBossAIComponent.h"
#include "game/CHydrogigaZiplineComponent.h"
#include "game/CHydrogigaZiplineRailComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU883.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

// As src/game/CInventoryComponent/CInventoryComponent_b40.cpp declares it.
struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
};
void fn_7100ae986c(UnkStrIdHandle* self, const CStrId& id);

// The handle, built in place by fn_7100ae986c and released on leaving the scope.
struct UnkZiplineKey_b49 {
    union {
        UnkStrIdHandle handle;
    };
    UnkZiplineKey_b49(const CStrId& id) { fn_7100ae986c(&handle, id); }
    ~UnkZiplineKey_b49() { handle.~UnkStrIdHandle(); }
};

struct UnkZiplineLinkTarget_b49 {
    u8 pad_0x0[0x10];
    void* object;
};
struct UnkZiplineLink_b49 {
    u8 pad_0x0[0x40];
};
struct UnkZiplineRailPart_b49;

struct UnkZiplineRail_b49 {
    u8 pad_0x0[0x3d];
    bool dead;
    u8 pad_0x3e[0x2];
    UnkZiplineLink_b49* links;
    u32 count;
};

void fn_710091d2fc(UnkZiplineRail_b49* self) {
    UnkGameManager* manager = g_unk_7101d47310;
    CBlackboard* blackboard = &manager->field_0x968;
    UnkGameManagerSub_0x608* scenario = manager->field_0x608;
    bool dead = false;
    if (scenario) {
        UnkZiplineKey_b49 key(CStrId("LUA:HYDROGIGA_DEAD", true));
        CStrId section_id = fn_71011a2fbc(scenario);
        if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
            fn_710006d11c(section, &key.handle.id, &dead);
    }
    if (dead) {
        self->dead = true;
        fn_710091d4bc(self);
    }
    if (self->count) {
        bool value = self->dead;
        UnkZiplineLink_b49* it = self->links;
        do {
            UnkZiplineLinkTarget_b49* target = *fn_71003b0c4c(it);
            fn_710091c994(fn_71005c3084(target ? target->object : nullptr), value);
            ++it;
        } while (it != self->links + self->count);
    }
}
