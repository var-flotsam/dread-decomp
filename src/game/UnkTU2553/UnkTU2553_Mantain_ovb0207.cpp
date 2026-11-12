// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): copies a string into the game
// manager's +0xe88, calls the object at +0xd40, marks two blackboard sections (named by the id table) and the
// current scenario's section as locked, then runs fn_7100dfc0c8 with the "[MANTAIN]" tag. Names and layouts
// are placeholders.
#include "base/UnkNamedRegistry.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/CBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2553.h"
#include "types.h"

using base::global::CStrId;

struct UnkGroup_ovb0207 {
    u8 pad_0x0[0x60];
    bool field_0x60;
};

struct UnkTunable_ovb0207;
struct UnkTunableVtbl_ovb0207 {
    void* slots[3];
    void (*vf_0x18)(UnkTunable_ovb0207* self);
};
struct UnkTunable_ovb0207 {
    UnkTunableVtbl_ovb0207* vtbl;
};

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);
bool fn_7100dfc0c8(UnkGameManager* self, const base::UnkNamedEntry* a, const base::UnkNamedEntry* b, bool c, bool d,
                   bool e, bool f, bool g, s32 mode);

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};

inline const CStrId& SectionId_ovb0207(u32 offset) {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(s_table.pad_0x0 + offset);
}

static inline void Lock_ovb0207(CBlackboard* blackboard, const CStrId& id) {
    CBlackboard::CSection* s = fn_71010472e0(blackboard, id);
    s->data.field_0x38 = true;
    if (auto* group = static_cast<UnkGroup_ovb0207*>(s->field_0x10))
        group->field_0x60 = true;
}

bool fn_7100dfbe78(UnkGameManager* self, void* text) {
    u8* raw = reinterpret_cast<u8*>(self);
    fn_710007eaa0(reinterpret_cast<base::global::CRntString*>(raw + 0xe88),
                  static_cast<const base::global::CRntString*>(text));
    UnkTunable_ovb0207* tunable = reinterpret_cast<UnkTunable_ovb0207*>(raw + 0xd40);
    UnkTunableVtbl_ovb0207* vtbl = tunable->vtbl;
    CBlackboard* blackboard = &self->field_0x968;
    vtbl->vf_0x18(tunable);
    Lock_ovb0207(blackboard, SectionId_ovb0207(0x1148));
    Lock_ovb0207(blackboard, SectionId_ovb0207(0x1bd0));
    {
        CStrId id = self->field_0x608 ? fn_71011a2fbc(self->field_0x608) : g_unk_7101d470e0;
        Lock_ovb0207(blackboard, id);
    }
    base::UnkNamedEntry a, b;
    fn_710007e704(&a, "[MANTAIN]");
    fn_710007e704(&b, "[MANTAIN]");
    bool r = fn_7100dfc0c8(self, &a, &b, false, false, true, false, false, 3);
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&b));
    fn_710007ecd4(reinterpret_cast<base::global::CRntString*>(&a));
    return r;
}
