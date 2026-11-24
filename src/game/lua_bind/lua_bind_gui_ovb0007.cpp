// Unit 2667 (tools/tu_map.py, "lua_bind"): functions the GUI scripts call. Each reads its arguments
// with the unit's argument reader (fn_71010c0270, a format string of "s" strings, "i" ints, "b" bools,
// "t" property bags and "_" for an argument it skips) or the display object at an argument
// (fn_710109b870: a userdata's weak handle target, or a path looked up in the GUI root), then
// forwards them to the GUI root (g_unk_717271e8f8), the game manager (g_unk_7101d47310) or the
// display object itself, and returns no results. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkCloneRecords.h"
#include "base/global/CRntVector.h"
#include "base/core/CBaseObject.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/ReflectedAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAbilityShinespark.h"
#include "game/CBlackboard.h"
#include "game/CChozoCommanderAIComponent.h"
#include "game/GUI/CIngameMenu.h"
#include "game/GUI/CText.h"
#include "game/UnkGameBlackboard.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "game/UnkGuiFrames.h"
#include "game/UnkGuiHandle.h"
#include "game/UnkGuiLabel.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkNumberText_b41a3.h"
#include "game/UnkStrIdHandle.h"
#include "game/UnkTU1645.h"
#include "game/UnkTU1686.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2676.h"
#include "game/UnkTU2677.h"
#include "game/UnkTU2761.h"
#include "game/UnkTUProperty.h"
#include "game/lua_bind.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"
#include "game/UnkTypedValue.h"

using base::global::CFilePathStrId;
using base::global::CRntString;
using base::global::CStrId;
using unk_prop_bag::Key;
using unk_prop_bag::UnkPropBag;

// The display object at argument `index` (not decompiled yet).
UnkGuiDisplayObject* fn_710109b870(lua_State* L, int index);
void fn_71011ba65c(UnkGuiRoot* root, const CRntString& name);
void fn_71011b8be0(UnkGuiRoot* root, const CRntString& name);
void fn_71011ba2d4(UnkGuiRoot* root, const CRntString& name);

namespace lua_bind {
int LaunchMessageItemAdquired(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    fn_71010c0270(L, "ss", &a, &b);
    return 0;
}

int LaunchMessagePowerUpAdquired(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    fn_71010c0270(L, "ss", &a, &b);
    return 0;
}

int LaunchMessage(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    const char* c = g_unk_710162e2ed;
    fn_71010c0270(L, "sss", &a, &b, &c);
    return 0;
}

int LoadShapes(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name))
        fn_71011ba65c(g_unk_717271e8f8, CRntString(name));
    return 0;
}

int LoadSkins(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name))
        fn_71011b8be0(g_unk_717271e8f8, CRntString(name));
    return 0;
}

int PrefetchPersistentSound(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name))
        fn_71011ba2d4(g_unk_717271e8f8, CRntString(name));
    return 0;
}
}  // namespace lua_bind

void fn_71011b892c(UnkGuiRoot* root, s32 index, const CRntString& name);

struct UnkGuiRootLists_b0007 {
    u8 pad_0x0[0x188];
    u8 field_0x188[0xc8];
    UnkLabelVec2 field_0x250;
};

namespace lua_bind {
int AddSymbolGlyphTable(lua_State* L) {
    s32 index = 0;
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    const char* c = g_unk_710162e2ed;
    if (fn_71010c0270(L, "isss", &index, &a, &b, &c))
        fn_710109c444(reinterpret_cast<UnkGuiRootLists_b0007*>(g_unk_717271e8f8)->field_0x188, index);
    return 0;
}

int AddSymbolTable(lua_State* L) {
    s32 index = 0;
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "is", &index, &name))
        fn_71011b892c(g_unk_717271e8f8, index, CRntString(name));
    return 0;
}
}  // namespace lua_bind

UnkLabelVec2 fn_71011ba240(UnkGuiRoot* root, UnkGuiDisplayObject* object, const UnkLabelVec2* size);
void fn_71011b7c28(UnkGuiRoot* root, const UnkLabelVec2* size, UnkGuiDisplayObject* object);

namespace lua_bind {
int StartDragNDrop(lua_State* L) {
    if (UnkGuiDisplayObject* object = fn_710109b870(L, 1)) {
        object->vf_0x60();
        UnkGuiRoot* root = g_unk_717271e8f8;
        UnkLabelVec2 size = reinterpret_cast<UnkGuiRootLists_b0007*>(root)->field_0x250;
        UnkLabelVec2 position = fn_71011ba240(root, object, &size);
        fn_71011cde64(object, reinterpret_cast<const base::math::CVector2D*>(&position), 0, 0);
        fn_71011b7c28(g_unk_717271e8f8, &size, object);
    }
    return 0;
}

int LaunchMessageAbilityAdquired(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    fn_71010c0270(L, "ss", &a, &b);
    UnkGameManager* manager = g_unk_7101d47310;
    CRntString name("AfterNewAbilityAcquired");
    fn_7100dfaf88(manager, &name, CRntString(g_unk_710162e2ed), true);
    return 0;
}
}  // namespace lua_bind

struct UnkWideStr_0x20;

namespace lua_bind {
int SetTextText(lua_State* L) {
    UnkGuiDisplayObject* object = fn_710109b870(L, 1);
    if (object && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(object->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(fn_7100b8559c()))) {
        const char* str;
        if (fn_71010c0270(L, "_s", &str)) {
            UnkNumberText_b41a3 text;
            fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(&text), str);
            fn_7100b868b0(reinterpret_cast<char*>(object), &text);
            fn_7100066e44(&text);
        }
    }
    return 0;
}

int SetLabelText(lua_State* L) {
    UnkGuiDisplayObject* object = fn_710109b870(L, 1);
    if (object && fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(object->GetClass()),
                                reinterpret_cast<const base::reflection::CType*>(fn_71011e8dfc()))) {
        const char* str;
        if (fn_71010c0270(L, "_s", &str)) {
            UnkGuiText text;
            fn_7100066b38(reinterpret_cast<UnkWideStr_0x20*>(&text), str);
            fn_71011ec468(object, text, false);
            fn_7100066e44(&text);
        }
    }
    return 0;
}
}  // namespace lua_bind

void fn_710118aa54(void* menu, const CRntString& a, const CRntString& b);

namespace lua_bind {
int ResetIngameMenuGUI(lua_State* L) {
    const char* a;
    const char* b;
    if (fn_71010c0270(L, "ss", &a, &b)) {
        if (void* menu = fn_7100deab70(g_unk_7101d47310, CRntString("INGAME")))
            fn_710118aa54(menu, CRntString(a), CRntString(b));
    }
    return 0;
}

int ShowPowerUpInSamusMenu(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        CStrId id(name, true);
        fn_7100b30454(fn_7100deac84(g_unk_7101d47310), reinterpret_cast<const UnkStrIdHandle&>(id));
    }
    return 0;
}
}  // namespace lua_bind

struct UnkUnit2754Item;
UnkUnit2754Item* fn_71011ba1d4(UnkGuiRoot* root, const CStrId& name);
void SetStageItemEnabled(UnkUnit2754Item* self, bool enabled);

namespace lua_bind {
int SetStageEnabled(lua_State* L) {
    const char* name;
    bool enabled;
    if (fn_71010c0270(L, "sb", &name, &enabled)) {
        UnkUnit2754Item* item = fn_71011ba1d4(g_unk_717271e8f8, CStrId(name, true));
        if (item)
            SetStageItemEnabled(item, enabled);
    }
    return 0;
}
}  // namespace lua_bind

struct UnkGuiScriptObject_b0007 {
    void** vtable;
    void vf_0x1e8(const char* name, bool arg) {
        reinterpret_cast<void (*)(UnkGuiScriptObject_b0007*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, arg);
    }
};

namespace lua_bind {
int HideEscapeCounter(lua_State* L) {
    UnkIngameMenu* menu = fn_7100deac84(g_unk_7101d47310);
    UnkGuiDisplayObject* object =
        fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(menu), CStrId("escapecomposition", true));
    if (object)
        reinterpret_cast<UnkGuiScriptObject_b0007*>(object)->vf_0x1e8("OnHide", false);
    return 0;
}
}  // namespace lua_bind

struct UnkIngameSub_b0007 {
    u8 pad_0x0[0x238];
    CStrId field_0x238;
};
UnkIngameSub_b0007* fn_7100b30d00(UnkIngameMenu* menu);

namespace lua_bind {
int SetSamusMenuCompositionLuaCallbackOnClosed(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        if (UnkIngameSub_b0007* sub = fn_7100b30d00(fn_7100deac84(g_unk_7101d47310)))
            { CStrId id(name, true); sub->field_0x238.ReleaseAndTake(id); }
    }
    return 0;
}
}  // namespace lua_bind

int fn_7101075ad0(lua_State* L, UnkGuiDisplayObject* object);

namespace lua_bind {
int CreateMSCPInstanceEx(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGuiDisplayObject* object = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
        if (object)
            return fn_7101075ad0(L, object);
    }
    return 0;
}

int DispatchEvent(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    s32 kind;
    UnkPropBag* bag = new (fn_71000839a0(sizeof(UnkPropBag), 0xc)) UnkPropBag;
    if (UnkGuiDisplayObject* object = fn_710109b870(L, 1)) {
        if (fn_71010c0270(L, "_st", &name, bag)) {
            s32 result;
            {
                CRntString key(name);
                result = fn_7101202300(&key);
            }
            kind = result;
            if (kind != 0x2c) {
                UnkGuiEvent event;
                fn_710120235c(&event, object, &kind, bag);
                fn_71011b6e0c(g_unk_717271e8f8, &event);
                fn_7101202488(&event);
            }
        }
    }
    delete bag;
    return 0;
}
}  // namespace lua_bind

// An empty string id: the pool is made first if needed.
struct UnkEmptyStrId_b0007 {
    base::global::UnkStringData* data;
    UnkEmptyStrId_b0007() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
    ~UnkEmptyStrId_b0007() { Id()->~CStrId(); }
    CStrId* Id() { return reinterpret_cast<CStrId*>(this); }
};

namespace lua_bind {
int SetIgnoreInput(lua_State* L) {
    bool add;
    UnkEmptyStrId_b0007 key;
    if (fn_71010c0270(L, "bs", &add, &key)) {
        UnkGuiRoot* root = g_unk_717271e8f8;
        if (add)
            fn_710002fd10(&root->field_0xca8, key.Id());
        else
            fn_710002ffb4(&root->field_0xca8, key.Id());
    }
    return 0;
}
}  // namespace lua_bind

void fn_71011b81b4(UnkGuiRoot* root, const CStrId& id, const CFilePathStrId& path, const CRntString& name, u32 mode,
                   bool flag);

namespace lua_bind {
int AddFont(lua_State* L) {
    const char* id = g_unk_710162e2ed;
    const char* path = g_unk_710162e2ed;
    const char* name = g_unk_710162e2ed;
    u32 mode = 0;
    bool flag = false;
    if (fn_71010c0270(L, "sssib", &id, &path, &name, &mode, &flag)) {
        u32 m = mode < 5 ? mode : 0;
        fn_71011b81b4(g_unk_717271e8f8, CStrId(id, true), CFilePathStrId(path, true), CRntString(name), m, flag);
    }
    return 0;
}
}  // namespace lua_bind

UnkGuiHandle fn_71011b685c(UnkGuiRoot* root, const CRntString& name, const CRntString& path, UnkPropBag* bag);

namespace lua_bind {
int CreateCompInstanceEx(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    const char* path = g_unk_710162e2ed;
    UnkPropBag bag;
    if (fn_71010c0270(L, "sst", &name, &path, &bag)) {
        UnkGuiDisplayObject* object =
            fn_71011b685c(g_unk_717271e8f8, CRntString(name), CRntString(path), &bag).Get();
        if (object)
            return fn_7101075ad0(L, object);
    }
    return 0;
}
}  // namespace lua_bind

UnkGuiDisplayObject* fn_71011b5aa4(UnkGuiRoot* root, const CStrId& name, const CRntString& path, UnkPropBag* bag);

namespace lua_bind {
int CreateDisplayObjectEx(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    const char* path = g_unk_710162e2ed;
    UnkPropBag bag;
    if (fn_71010c0270(L, "sst", &name, &path, &bag)) {
        UnkGuiDisplayObject* object = fn_71011b5aa4(g_unk_717271e8f8, CStrId(name, true), CRntString(path), &bag);
        if (object)
            return fn_7101075ad0(L, object);
    }
    return 0;
}

int CreateHUDRoot(lua_State* L) {
    UnkPropBag bag;
    unk_prop_bag::fn_710050f4ac(&bag, CStrId("Enabled", true), false);
    unk_prop_bag::fn_710050f4ac(&bag, CStrId("Focusable", true), true);
    unk_prop_bag::fn_7100aa3b9c(&bag, CStrId("FocusIdx", true), -1);
    UnkGuiDisplayObject* menu = fn_71011b685c(g_unk_717271e8f8, CRntString("IngameMenuComposition"),
                                              CRntString("IngameMenuComposition"), &bag)
                                    .Get();
    fn_71011cde44(menu, 1);
    return fn_7101075ad0(L, menu);
}
}  // namespace lua_bind

struct UnkCameraCtrl;

namespace lua_bind {
int CreateDisplayObject(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    const char* path = g_unk_710162e2ed;
    UnkPropBag bag;
    if (UnkGuiDisplayObject* parent = fn_710109b870(L, 1)) {
        if (fn_71010c0270(L, "_sst", &name, &path, &bag)) {
            UnkGuiDisplayObject* child =
                fn_71011b5aa4(g_unk_717271e8f8, CStrId(name, true), CRntString(path), &bag);
            if (child) {
                if (fn_7100023d00(reinterpret_cast<const UnkCameraCtrl*>(parent), fn_71011d7b84()))
                    fn_71011d968c(parent, child);
                return fn_7101075ad0(L, child);
            }
        }
    }
    return 0;
}
}  // namespace lua_bind

// "NumEmmisDefeated" (src/game_s7/fn_7100ae39e0.cpp).

struct UnkGameManagerData_b0007 {
    u8 pad_0x0[0x968];
    CBlackboard blackboard;
};

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), base::global::CStrId::Empty());
    return qid;
}

// Stores the int at `value` under `name` in the section `section` names (unless the blackboard is
// locked).
static inline void SetSectionInt(CBlackboard* blackboard, const CStrId& section, UnkQualifiedId& name, s32* value) {
    if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
        if (!blackboard->field_0xe8) {
            UnkTypedValue v;
            v.type = fn_7100014768();
            v.value = value;
            fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&name), &v);
        }
    }
}

namespace lua_bind {
int WriteEmmyDeathToBlackboard(lua_State* L) {
    s32 count = 0;
    CBlackboard* data = &reinterpret_cast<UnkGameManagerData_b0007*>(g_unk_7101d47310)->blackboard;
    {
        const CStrId& section = Key(0x1150);
        UnkQualifiedId& name = Qid(g_unk_71727593f8);
        if (CBlackboard::CSection* s = fn_7101047270(data, section))
            fn_7100395a30(s, &name, &count);
    }
    ++count;
    {
        const CStrId& section = Key(0x1150);
        UnkQualifiedId& name = Qid(g_unk_71727593f8);
        SetSectionInt(data, section, name, &count);
    }
    return 0;
}
}  // namespace lua_bind

// A string id released in two steps (the count is reloaded after the decrement), as CStrId::Reset.
struct UnkListedId_b0007 {
    base::global::UnkStringData* data;
    UnkListedId_b0007(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkListedId_b0007() { reinterpret_cast<CStrId*>(this)->Reset(); }
};

// The request's id argument: a counted copy, released in one step.
struct UnkIdArg_b0007 {
    base::global::UnkStringData* data;
    UnkIdArg_b0007(const CStrId& other) : data(other.data) {
        if (&g_unk_7101d47108->empty != data)
            ++data->ref_count;
    }
    ~UnkIdArg_b0007() {
        if (base::global::UnkStringPool* pool = g_unk_7101d47108) {
            if (data && &pool->empty != data) {
                if (--data->ref_count == 0)
                    StringPool_Release(pool, data);
            }
        }
    }
    const CStrId& Id() const { return reinterpret_cast<const CStrId&>(*this); }
};

// A request the game manager's list at +0x2128 takes: a word, an id and a list of ids.
struct UnkIdRequest_b0007 {
    u32 field_0x0;
    UnkListedId_b0007 id;
    base::global::CRntVector<UnkListedId_b0007> ids{0xc};

    UnkIdRequest_b0007(UnkIdArg_b0007 id) : field_0x0(0), id(id.Id()) {}
    __attribute__((always_inline)) ~UnkIdRequest_b0007() { ids.Free(); }
};

struct UnkGameManagerRequests_b0007 {
    u8 pad_0x0[0x2128];
    u8 field_0x2128[1];
};
struct UnkMlogMessage_b42a1;

namespace lua_bind {
int AddEmmyMissionLogEntry(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        CStrId id(name, true);
        UnkIdRequest_b0007 request(id);
        fn_7100456b7c(reinterpret_cast<UnkGameManagerRequests_b0007*>(g_unk_7101d47310)->field_0x2128, reinterpret_cast<const UnkMlogMessage_b42a1*>(&request));
    }
    return 0;
}
}  // namespace lua_bind

void* fn_710109e770(lua_State* L, int index);

extern char g_unk_7101caf688[];

// A counted reference to a display object's weak handle (0x10 bytes, vtable g_unk_7101caf688).
struct UnkWeakRef_b0007 {
    char* vtable;
    UnkWeakHandle* handle;
    UnkWeakRef_b0007() : vtable(g_unk_7101caf688 + 0x10), handle(nullptr) {}
};

namespace lua_bind {
int AddLuaEventHandler(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    void* handler = nullptr;
    UnkGuiDisplayObject* object = fn_710109b870(L, 1);
    if (!object || !fn_71010c0ad0(L, 1, 1, "s", &name))
        return 0;
    handler = fn_710109e770(L, 3);
    if (!handler)
        return 0;
    UnkWeakRef_b0007* ref = nullptr;
    if (UnkGuiDisplayObject* target = fn_710109b870(L, 4)) {
        ref = new (fn_71000839a0(sizeof(UnkWeakRef_b0007), 0xc)) UnkWeakRef_b0007;
        AssignRef(&ref->handle, GetWeakHandle(reinterpret_cast<UnkWeakTarget*>(target)));
    }
    u32 mode = 6;
    if (lua_type(L, 5) == LUA_TBOOLEAN)
        mode = fn_71010a4900(L, 5) ? 1 : 6;
    s32 kind;
    CRntString event(name);
    kind = fn_7101202300(&event);
    void* holder = fn_71000839a0(0x10, 0xc);
    fn_7100b33f90(holder, &handler);
    fn_71011cf394(object, &kind, holder, ref, mode);
    return 0;
}
}  // namespace lua_bind
