// CGameManager::CTunableGameManager's unit (tools/tu_map.py unit 2553): tears the current game down: tells the
// script environment behind +0x608 it is leaving, releases it and the players, and resets the managers that
// `flags` selects. Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "game/CAbilityEnergyWave.h"
#include "game/CBombGun.h"
#include "game/CHydrogigaAIComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkModelManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkSubAreaGuiCache.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2562.h"
#include "game/UnkTU2669.h"
#include "game/UnkTU2739.h"
#include "game/UnkTU315.h"
#include "game/UnkTU341.h"
#include "game/UnkTU344.h"
#include "game/UnkTU365.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;

struct UnkScriptManager_ovb0085;
struct UnkManager_7100863a8c;
struct UnkActorDefFactory;
struct UnkSweepList_b49;
struct UnkActor_71002329e8;

char* fn_7100219210(char* p0);
namespace callee { char* fn_71011a2d7c(u64); }
void fn_71011a19a4(void* self);
void fn_7100def678(UnkGameManager* self, const CRntString& name, const char* fmt, ...);
char* fn_7100d7820c(char* p0, u32 p1, u32 p2, u32 p3, u32 p4);
void fn_7100de70a8(UnkGameManager* self);
void fn_7101047ea8(void* self);
void fn_7100e2c28c(void* self);
char* fn_7100ed779c(char* p0);
void fn_7100ed6bc4(void* self);
void fn_7100e8efac(void* world);
void fn_710119d584(UnkSweepList_b49* self, bool flag);
void fn_710119336c(UnkActorDefFactory* factory);
void fn_7100d46804(UnkManager_7100863a8c* self);
void fn_7100d46c5c(UnkManager_7100863a8c* self);
void fn_7100defd08(UnkGameManager* self);

// The script environment behind +0x608 (its +0x18 holds the environment's name and a use count).
struct UnkExitScriptEnv {
    u8 pad_0x0[0x10];
    CRntString name;
    u8 pad_0x40[0x4];
    union {
        s32 field_0x44;
    };
};
struct UnkExitScriptHost {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    u8 pad_0x8[0x10];
    UnkExitScriptEnv* env;
};

// An actor the handles below refer to (its +0x1b2 is a flag).
struct UnkExitActor {
    u8 pad_0x0[0x1b2];
    u8 field_0x1b2;
};
struct UnkExitHandle {
    u8 pad_0x0[0x10];
    UnkExitActor* obj;
};

// The object fn_7100decab0 gives: +0x248 is a polymorphic object reset on exit.
struct UnkExitTargetObj {
    virtual void vf_0x0();
    u8 pad_0x8[0xc20];
    u8 field_0xc28;
    u8 pad_0xc29[3];
    u32 field_0xc2c;
    void Reset() {
        field_0xc28 = 0;
        field_0xc2c = 0;
        reinterpret_cast<void (***)(UnkExitTargetObj*)>(this)[0][0x168 / 8](this);
    }
};
struct UnkExitTarget {
    u8 pad_0x0[0x248];
    UnkExitTargetObj* field_0x248;
};

// The members of the game manager this function reads that the shared layout leaves as padding.
struct UnkGameManagerExitView {
    u8 pad_0x0[0x578];
    u8 field_0x578;
    u8 pad_0x579[0x93f];
    u8 field_0xeb8[0x1ae0];
    UnkExitHandle* field_0x2998;
    UnkExitHandle* field_0x29a0;
};
static_assert(__builtin_offsetof(UnkGameManagerExitView, field_0xeb8) == 0xeb8, "layout");
static_assert(__builtin_offsetof(UnkGameManagerExitView, field_0x2998) == 0x2998, "layout");

template <class T>
static inline T* Instance(T*& slot, u64 size, void (*construct)(T*)) {
    T* object = slot;
    if (!object) {
        object = static_cast<T*>(fn_71000839a0(size, 0x3f));
        construct(object);
        slot = object;
    }
    return object;
}

static inline bool IsEditor(UnkGameManager* self) {
    char* object = static_cast<char*>(self->field_0x498);
    return object && fn_710007f140(reinterpret_cast<const CRntString*>(object + 8), "EDITOR");
}

static inline UnkExitActor* Target(UnkExitHandle* handle) {
    return handle ? handle->obj : nullptr;
}

void fn_7100def930(UnkGameManager* self, u32 flags) {
    UnkGameManagerExitView* view = reinterpret_cast<UnkGameManagerExitView*>(self);
    fn_71011aa820(Instance(g_unk_71726b0cd8, 0x1e8, fn_71011a870c));
    fn_7100219210(reinterpret_cast<char*>(g_unk_7101d47300));
    if (self->field_0x608) {
        callee::fn_71011a2d7c(reinterpret_cast<u64>(self->field_0x608));
        if (!IsEditor(self))
            fn_7100def678(self, CRntString("OnExit"), g_unk_710162e2ed);
        fn_7100def888(reinterpret_cast<char*>(self));
        UnkExitActor* first = Target(view->field_0x2998);
        UnkExitActor* second = Target(view->field_0x29a0);
        if (first)
            fn_7100d7820c(reinterpret_cast<char*>(first), 1, 1, 1, 1);
        if (second)
            fn_7100d7820c(reinterpret_cast<char*>(second), 1, 1, 1, 1);
        if (first && first->field_0x1b2)
            fn_71002329e8(reinterpret_cast<UnkActor_71002329e8*>(first));
        if (second && second->field_0x1b2)
            fn_71002329e8(reinterpret_cast<UnkActor_71002329e8*>(second));
        fn_71011a19a4(self->field_0x608);
        fn_7100de6ee8(self);
    }
    if (UnkExitTarget* target = reinterpret_cast<UnkExitTarget*>(fn_7100decab0(self)))
        if (UnkExitTargetObj* object = target->field_0x248)
            object->Reset();
    if (flags & 2)
        fn_7101047ea8(view->field_0xeb8);
    fn_7100e2c28c(Instance(g_unk_71726eb3e8, 0xa0f8, fn_7100e211cc));
    if (!(flags & 8))
        fn_7100219804(g_unk_7101d47300, 0);
    fn_7100ed779c(static_cast<char*>(g_unk_71726af0b0));
    UnkExitScriptHost* host = reinterpret_cast<UnkExitScriptHost*>(self->field_0x608);
    if ((flags & 4) && host && !view->field_0x578) {
        fn_7100000790("Scenario.ResetCurrentScenarioInfo", g_unk_710162e2ed);
        UnkScriptManager_ovb0085* scripts = Instance(g_unk_71727bed90, 0x88, fn_710109d810);
        {
            UnkExitScriptHost* current = reinterpret_cast<UnkExitScriptHost*>(self->field_0x608);
            ++current->env->field_0x44;
            CRntString name = fn_710007ef90(&current->env->name, "_auto");
            fn_710109d890(scripts, &name);
        }
        scripts = Instance(g_unk_71727bed90, 0x88, fn_710109d810);
        UnkExitScriptHost* current = reinterpret_cast<UnkExitScriptHost*>(self->field_0x608);
        ++current->env->field_0x44;
        fn_710109d890(scripts, &current->env->name);
        host = reinterpret_cast<UnkExitScriptHost*>(self->field_0x608);
    }
    if (host) {
        host->vf_0x10();
        self->field_0x608 = nullptr;
    }
    fn_7100de6ee8(self);
    if (flags & 1) {
        fn_7100de70a8(self);
        fn_710119d584(reinterpret_cast<UnkSweepList_b49*>(Instance(g_unk_71727254f8, 0x48, fn_710119d484)), false);
        fn_710119336c(g_unk_71726a0220);
    }
    fn_7100ed6bc4(g_unk_71726af0b0);
    fn_7100e8efac(self->field_0x2228);
    fn_7100d46c5c(Instance(g_unk_71726a4788, 0x6b0, fn_7100d46804));
    fn_7100defd08(self);
}
