// UnkTU2726 (gui/scripts/msemenu_ingame.lua): closes the in-game menu's model (fn_7101189cdc): the team
// manager's current actor stops showing its marker, the model at +0x2b0 goes back to the model manager
// and the state at +0x260 is cleared. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "game/CTeamManager/CTunableTeamManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkTU2523.h"
#include "game/UnkTU2739.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

extern char* g_unk_71726b2398;

struct UnkMarker_l189cdc {
    u8 pad_0x0[0x6c];
    bool visible;  // 0x6c
};

struct UnkMarkedActor_l189cdc {
    u8 pad_0x0[0x1b8];
    UnkMarker_l189cdc* marker;  // 0x1b8
};

struct UnkTeamPart_l189cdc {
    u8 pad_0x0[0x10];
    UnkWeakTarget* current;  // 0x10
};

struct UnkTeamManager_l189cdc {
    u8 pad_0x0[0x100];
    UnkTeamPart_l189cdc* part;  // 0x100
};

struct UnkMenu_l189cdc {
    u8 pad_0x0[0x260];
    u32 field_0x260;
    u8 pad_0x264[0x4c];
    void* model;  // 0x2b0
};

inline UnkTeamManager_l189cdc* TeamManager_l189cdc() {
    char* instance = g_unk_71726b2398;
    if (!instance) {
        instance = static_cast<char*>(fn_71000839a0(0x108, 0x3f));
        fn_71011a664c(instance);
        g_unk_71726b2398 = instance;
    }
    return reinterpret_cast<UnkTeamManager_l189cdc*>(instance);
}

inline bool HasCurrentTarget_l189cdc() {
    UnkTeamPart_l189cdc* part = TeamManager_l189cdc()->part;
    return part && part->current;
}

inline UnkWeakTarget* CurrentTarget_l189cdc() {
    UnkTeamPart_l189cdc* part = TeamManager_l189cdc()->part;
    return part ? part->current : nullptr;
}

static inline UnkModelManager* ModelManager() {
    if (!g_unk_71726b0cd8) {
        UnkModelManager* m = static_cast<UnkModelManager*>(fn_71000839a0(0x1e8, 0x3f));
        fn_71011a870c(m);
        g_unk_71726b0cd8 = m;
    }
    return g_unk_71726b0cd8;
}

// A counted reference to a weak handle.
struct UnkHandleRef_l189cdc {
    UnkWeakHandle* handle;

    explicit UnkHandleRef_l189cdc(UnkWeakHandle* p) : handle(p) {
        if (handle)
            ++handle->ref_count;
    }
    ~UnkHandleRef_l189cdc() {
        if (handle) {
            if (--handle->ref_count == 0)
                handle->vf_0x10();
        }
    }
    UnkMarkedActor_l189cdc* Get() const {
        return handle ? reinterpret_cast<UnkMarkedActor_l189cdc*>(handle->target) : nullptr;
    }
};

void fn_7101189cdc(UnkMenu_l189cdc* self) {
    UnkHandleRef_l189cdc current(HasCurrentTarget_l189cdc() ? fn_7100d78378(CurrentTarget_l189cdc()) : nullptr);
    if (UnkMarkedActor_l189cdc* actor = current.Get()) {
        if (actor->marker)
            actor->marker->visible = false;
    }
    if (self->model) {
        fn_71011a91c4(ModelManager(), self->model, 0);
        self->model = nullptr;
    }
    self->field_0x260 = 0;
}
