// CChozoCommanderXLifeComponent's start (after its base step fn_710115fe1c): when the owner's flag (+0x1b4)
// is set, tells the game manager's object at +0x2228 (fn_7100e8f9a0), resolves the lazily resolved handle at
// +0x2e8 (as src/game/CAccessPointCommanderComponent/CAccessPointCommanderComponent_Start_A5_b76.cpp
// does) and, when its object has the component fn_71004067a4 looks for, hooks fn_71008c0000 up and notifies
// it, else runs fn_71008bdd84. Then stores true under "SG_ChozoCommander_X" in the current scenario's
// blackboard section (as src/game/CAbilityShinespark/CAbilityShinespark_Glow_ovb0045.cpp). Names and layouts
// are placeholders.
#include "base/UnkRefHandle.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/CBlackboard.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;

CStrId fn_71011a2fbc(UnkGameManagerSub_0x608*);

void fn_710115fe1c(char*);
void fn_7100e8f9a0(char*, bool, base::global::CStrId*);
void* fn_71004067a4(void* owner);
void fn_71007a8280(void* object);
void fn_71008bdd84(void* self, bool flag);
void fn_71008c0000();
extern __attribute__((visibility("hidden"))) char g_unk_71727302a0[];
extern void* g_unk_7101d47330;

struct UnkCallback_kr {
    void* object;
    void (*fn)();
};
void fn_71011995c0(void* owner, const UnkCallback_kr& callback);

struct UnkCountedA5_b76 : base::UnkRefCounted {
    union {
        void* field_0x10;
        u64 raw_0x10;
    };
};

struct UnkRef {
    UnkCountedA5_b76* ptr;
    ~UnkRef() {
        if (ptr && --ptr->ref_count == 0)
            delete ptr;
    }
};
UnkRef fn_7100353be8(void* from);

struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);
const UnkSubId_7100fc4b34& fn_71008c0004(UnkQualifiedName* self, const char* name);
extern UnkPropertyEx<0x160> g_unk_7172759848;

struct UnkCommanderOwner_b0266 {
    u8 pad_0x0[0x1b4];
    bool field_0x1b4;
};

struct UnkCommanderLife_b0266 {
    u8 pad_0x0[0x30];
    UnkCommanderOwner_b0266* owner;
    u8 pad_0x38[0x280];
    u8 items[0x28];  // 0x2b8
    u32 state;       // 0x2e0
    u32 pad_0x2e4;
    base::UnkRefHandle handle;  // 0x2e8
};
static_assert(__builtin_offsetof(UnkCommanderLife_b0266, handle) == 0x2e8, "layout");

static inline UnkSubId_7100fc4b34 SubId(const char* name) {
    UnkQualifiedName* qualified = reinterpret_cast<UnkQualifiedName*>(reinterpret_cast<char*>(&g_unk_7172759848) + 0x10);
    if (!qualified->str || !qualified->str[0])
        fn_7100ae9d7c(qualified, reinterpret_cast<UnkIdSource*>(&g_unk_7172759848), g_unk_7101d470e0);
    return fn_71008c0004(qualified, name);
}

void fn_71008bdb50(UnkCommanderLife_b0266* self) {
    fn_710115fe1c(reinterpret_cast<char*>(self));
    if (self->owner->field_0x1b4) {
        UnkGameManager* manager = g_unk_7101d47310;
        fn_7100e8f9a0(static_cast<char*>(manager->field_0x2228), false, reinterpret_cast<CStrId*>(g_unk_71727302a0));
        if (self->state == 1) {
            {
                UnkRef fresh = fn_7100353be8(self->items);
                fn_710031d208(&self->handle, reinterpret_cast<const base::UnkRefHandle*>(&fresh));
            }
            self->state = 2;
        }
        auto* c = static_cast<UnkCountedA5_b76*>(self->handle.ptr);
        if (c && c->raw_0x10) {
            // reads the target again after the test, as the original does
            asm("" ::: "memory");
            if (void* object = fn_71004067a4(c->field_0x10)) {
                fn_71011995c0(g_unk_7101d47330, UnkCallback_kr{self, fn_71008c0000});
                fn_71007a8280(object);
            }
        } else {
            fn_71008bdd84(self, false);
        }
        manager = g_unk_7101d47310;
        UnkTypedValue v;
        CBlackboard* blackboard = &manager->field_0x968;
        CStrId section_id = fn_71011a2fbc(manager->field_0x608);
        UnkSubId_7100fc4b34 key = SubId("SG_ChozoCommander_X");
        bool value = true;
        if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section_id)) {
            if (!manager->field_0x968.field_0xe8) {
                v.type = fn_710000d228();
                v.value = &value;
                fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&key.id), &v);
            }
        }
    }
}
