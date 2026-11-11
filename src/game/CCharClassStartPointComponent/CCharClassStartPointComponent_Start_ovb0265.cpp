// CCharClassStartPointComponent's unit (tools/tu_map.py): starting the component. The owner (+0x30) is
// handed to the component that fn_7100a81ed0 or, failing that, fn_71007cfd8c answers (slots 0x180 and
// 0x258). When the owner has the object at +0x1e0 and the name at +0x40 is not empty, the script function it
// names ("<scenario>.<name>" unless the name already holds a ".") is run with the owner and the argument.
// Then the id at +0x70 is applied (flag +0x78) and, when the argument is the game manager's player (after
// fn_7100e01fc8 if it was already), the argument is handed to the global at g_unk_71726bee10. Names and
// layouts are placeholders.
#include <new>

#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/UnkStringPoolBody.h"
#include "game/CAutsharpAIComponent.h"
#include "game/CElectricReactionComponent.h"
#include "game/CElevatorUsableComponent.h"
#include "game/UnkGameManager.h"
#include "game/UnkTU2547.h"
#include "game/UnkTU2677.h"
#include "types.h"

using base::global::CRntString;

struct lua_State;
extern lua_State* g_unk_7172a01d18;
s32 fn_71010c0bb0(lua_State* L, const char* name);
int fn_7100fbc4e0(lua_State* L, void* arg);
CRntString fn_710007ef90(const CRntString*, const char*);

union UnkStringSlot {
    CRntString value;
    UnkStringSlot() {}
    ~UnkStringSlot() {}
};

struct UnkStartPointComponentA {
    void** vtable;
    void vf_0x180(void* actor) { reinterpret_cast<void (*)(UnkStartPointComponentA*, void*)>(vtable[0x180 / 8])(this, actor); }
    void vf_0x258(void* actor) { reinterpret_cast<void (*)(UnkStartPointComponentA*, void*)>(vtable[0x258 / 8])(this, actor); }
};
struct UnkSmartObjectActor;
struct UnkSmartObjectUserA;
struct UnkSmartObjectUserB;

struct UnkStartPointActor {
    u8 pad_0x0[0x1e0];
    void* field_0x1e0;
};

struct UnkStartPointSelf {
    u8 pad_0x0[0x30];
    union {
        UnkStartPointActor* actor;  // 0x30
    };
    u8 pad_0x38[8];
    CRntString name;                // 0x40
    base::global::CStrId id;        // 0x70
    bool flag;                      // 0x78
};

void fn_7100df249c(UnkGameManager* self, const base::global::CStrId* id, bool flag);
struct UnkPlayer;
UnkPlayer* fn_7100dec788(UnkGameManager* self);
void fn_7100e01fc8(UnkGameManager* self);
struct UnkDoorRegistry_b61;
void fn_7100ddb9a4(UnkDoorRegistry_b61* self, UnkStartPointSelf* component);

// A string id's entry as this reads it: the text at +0x10 and a use count at +0x44, a union member because its
// store makes the original reload the id.
struct UnkScenarioEntry {
    u8 pad_0x0[0x10];
    CRntString text;  // 0x10
    u8 pad_0x40[0x4];
    union {
        u32 uses;  // 0x44
    };
};

struct UnkScenarioInfo {
    u8 pad_0x0[0x18];
    UnkScenarioEntry* name;  // 0x18
};

static inline const CRntString* ScenarioName(UnkScenarioEntry* const& id) {
    ++id->uses;
    return &id->text;
}

void fn_710116c644(UnkStartPointSelf* self, UnkPlayer* player) {
    if (auto* a = reinterpret_cast<UnkStartPointComponentA*>(fn_7100a81ed0(reinterpret_cast<UnkSmartObjectActor*>(self->actor))))
        a->vf_0x180(self->actor);
    else {
        // keeps the second lookup's load of the owner apart from the first branch's
        asm("" : "+r"(self));
        if (auto* b = reinterpret_cast<UnkStartPointComponentA*>(fn_71007cfd8c(reinterpret_cast<UnkSmartObjectActor*>(self->actor))))
            b->vf_0x258(self->actor);
    }
    if (self->actor->field_0x1e0 && self->name.data && self->name.data[0]) {
        s32 found;
        if (fn_710007fb9c(&self->name, ".")) {
            lua_State* L = g_unk_7172a01d18;
            found = fn_71010c0bb0(L, self->name.data ? self->name.data : g_unk_710162e2ed);
        } else {
            lua_State* L = g_unk_7172a01d18;
            UnkStringSlot full;  // declared first: it gets the lower slot
            UnkStringSlot prefix;
            new (&prefix.value) CRntString(fn_710007ef90(
                ScenarioName(reinterpret_cast<UnkScenarioInfo*>(g_unk_7101d47310->field_0x608)->name), "."));
            new (&full.value) CRntString(fn_710007eee0(&prefix.value, &self->name));
            found = fn_71010c0bb0(L, full.value.data ? full.value.data : g_unk_710162e2ed);
            full.value.~CRntString();
            prefix.value.~CRntString();
        }
        if (found) {
            fn_7100fbc4e0(g_unk_7172a01d18, self->actor);
            fn_7100fbc4e0(g_unk_7172a01d18, player);
            char out[0x100];
            fn_71010c0d80(g_unk_7172a01d18, 2, out, 0x100, nullptr, nullptr);
        }
    }
    if (self->id.data != &g_unk_7101d47108->empty)
        fn_7100df249c(g_unk_7101d47310, &self->id, self->flag);
    if (fn_7100dec788(g_unk_7101d47310) == player)
        fn_7100e01fc8(g_unk_7101d47310);
    if (fn_7100dec788(g_unk_7101d47310) == player) {
        if (!g_unk_71726bee10) {
            UnkDoorRegistry_b61* registry = static_cast<UnkDoorRegistry_b61*>(fn_71000839a0(4, 0x3f));
            fn_7100ddb6dc(registry);
            g_unk_71726bee10 = registry;
        }
        fn_7100ddb9a4(g_unk_71726bee10, self);
    }
}
