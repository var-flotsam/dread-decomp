// Unit 2666 (lua_bind): advances the first actor's current attack index to the next attack in its list
// that has an enabled entry, wrapping to the start. Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CAttack.h"
#include "game/CCharClassAttack.h"
#include "game/UnkManager_71726af468.h"
#include "game/UnkTU2677.h"
#include "types.h"

struct UnkAttackTarget_A2_b68;
struct UnkAttackListSelf_b0143;
CAttack* fn_7101105148(UnkAttackListSelf_b0143* self, const char* name);
s32 fn_71003623a4(CAttack* attack);

struct UnkIdTextData_l086370 {
    u8 pad_0x0[0x10];
    const char* text;
    u8 pad_0x18[0x2c];
    union {
        s32 reads;
    };
};

struct UnkAttackEntry_l086370 {
    u8 pad_0x0[0x10];
    bool enabled;
};

struct UnkAttackAI_l086370 {
    u8 pad_0x0[0x1780];
    union {
        UnkIdTextData_l086370* id;
    };
    u32 index;
};
struct UnkAttackActor_l086370 {
    u8 pad_0x0[0x240];
    UnkAttackAI_l086370* ai;
};
struct UnkAttackHolder_l086370 {
    u8 pad_0x0[0x10];
    UnkAttackActor_l086370* actor;
};

static inline UnkAttackEntry_l086370* GetEntry_l086370(CAttack* attack, u32 i) {
    return static_cast<UnkAttackEntry_l086370*>(
        fn_71003646b0(reinterpret_cast<UnkAttackTarget_A2_b68*>(attack->field_0x20), i));
}

s32 fn_7101086370(lua_State*) {
    UnkManager_71726af468* manager = g_unk_71726af468;
    if (!manager) {
        manager = static_cast<UnkManager_71726af468*>(fn_71000839a0(0x2610, 0x3f));
        fn_7100da7e2c(manager);
        g_unk_71726af468 = manager;
    }
    if (UnkAttackHolder_l086370* holder = *reinterpret_cast<UnkAttackHolder_l086370**>(manager)) {
        if (UnkAttackActor_l086370* actor = holder->actor) {
            if (UnkAttackAI_l086370* ai = actor->ai) {
                u32 index = 0;
                if (reinterpret_cast<void*>(ai->id) != &g_unk_7101d47108->empty) {
                    ++ai->id->reads;
                    const char* text = ai->id->text;
                    CAttack* attack = fn_7101105148(reinterpret_cast<UnkAttackListSelf_b0143*>(ai),
                                                    text ? text : g_unk_710162e2ed);
                    if (attack) {
                        s32 i = ai->index + 1;
                        for (; i < fn_71003623a4(attack); ++i) {
                            UnkAttackEntry_l086370* entry = GetEntry_l086370(attack, i);
                            if (entry && entry->enabled)
                                break;
                        }
                        if (i < fn_71003623a4(attack)) {
                            index = i;
                        } else {
                            for (i = 0; i < fn_71003623a4(attack); ++i) {
                                UnkAttackEntry_l086370* entry = GetEntry_l086370(attack, i);
                                if (entry && entry->enabled) {
                                    index = i;
                                    break;
                                }
                            }
                        }
                    }
                }
                ai->index = index;
            }
        }
    }
    return 0;
}
