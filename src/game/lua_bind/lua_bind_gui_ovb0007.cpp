// Unit 2667 (tools/tu_map.py, "lua_bind"): functions the GUI scripts call. Each reads its arguments
// with the unit's argument reader (fn_71010c0270, a format string of "s" strings, "i" ints, "b" bools,
// "t" property bags and "_" for an argument it skips) or the display object at an argument
// (fn_710109b870: a userdata's weak handle target, or a path looked up in the GUI root), then
// forwards them to the GUI root (g_unk_717271e8f8), the game manager (g_unk_7101d47310) or the
// display object itself, and returns no results. Names and layouts are placeholders.
#include "../../../lib/lua/lua.h"
#include "../../game_s7/UnkPropBagS7.h"
#include "base/UnkCloneRecords.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflAttributes.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/CChozoCommanderAIComponent.h"
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
#include "game/UnkTU1686.h"
#include "game/UnkTU2676.h"
#include "game/UnkTU2677.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

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

int fn_710109a7a0(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    fn_71010c0270(L, "ss", &a, &b);
    return 0;
}

int fn_710109a7e0(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    fn_71010c0270(L, "ss", &a, &b);
    return 0;
}

int fn_710109a750(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    const char* c = g_unk_710162e2ed;
    fn_71010c0270(L, "sss", &a, &b, &c);
    return 0;
}

int fn_7101099b20(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name))
        fn_71011ba65c(g_unk_717271e8f8, CRntString(name));
    return 0;
}

int fn_7101099b90(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name))
        fn_71011b8be0(g_unk_717271e8f8, CRntString(name));
    return 0;
}

int fn_7101099f10(lua_State* L) {
    const char* name;
    if (fn_71010c0270(L, "s", &name))
        fn_71011ba2d4(g_unk_717271e8f8, CRntString(name));
    return 0;
}

const base::global::CRntString** fn_710109c444(void* vec, u32 index);
void fn_71011b892c(UnkGuiRoot* root, s32 index, const CRntString& name);

struct UnkGuiRootLists_b0007 {
    u8 pad_0x0[0x188];
    u8 field_0x188[0xc8];
    UnkLabelVec2 field_0x250;
};

int fn_710109a370(lua_State* L) {
    s32 index = 0;
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    const char* c = g_unk_710162e2ed;
    if (fn_71010c0270(L, "isss", &index, &a, &b, &c))
        fn_710109c444(reinterpret_cast<UnkGuiRootLists_b0007*>(g_unk_717271e8f8)->field_0x188, index);
    return 0;
}

int fn_710109a2f0(lua_State* L) {
    s32 index = 0;
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "is", &index, &name))
        fn_71011b892c(g_unk_717271e8f8, index, CRntString(name));
    return 0;
}

UnkLabelVec2 fn_71011ba240(UnkGuiRoot* root, UnkGuiDisplayObject* object, const UnkLabelVec2* size);
void fn_71011b7c28(UnkGuiRoot* root, const UnkLabelVec2* size, UnkGuiDisplayObject* object);

int fn_7101099a60(lua_State* L) {
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

void fn_7100dfaf88(UnkGameManager* self, void* name, const CRntString& tag, bool flag);

int fn_710109a820(lua_State* L) {
    const char* a = g_unk_710162e2ed;
    const char* b = g_unk_710162e2ed;
    fn_71010c0270(L, "ss", &a, &b);
    UnkGameManager* manager = g_unk_7101d47310;
    CRntString name("AfterNewAbilityAcquired");
    fn_7100dfaf88(manager, &name, CRntString(g_unk_710162e2ed), true);
    return 0;
}

struct UnkWideStr_0x20;
struct UnkNumberText_b41a3 {
    u8 unk_0x0[0x20];
};

int fn_71010998b0(lua_State* L) {
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

int fn_7101099950(lua_State* L) {
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

void* fn_7100deab70(UnkGameManager* self, const CRntString& name);
void fn_710118aa54(void* menu, const CRntString& a, const CRntString& b);

int fn_7101099d60(lua_State* L) {
    const char* a;
    const char* b;
    if (fn_71010c0270(L, "ss", &a, &b)) {
        if (void* menu = fn_7100deab70(g_unk_7101d47310, CRntString("INGAME")))
            fn_710118aa54(menu, CRntString(a), CRntString(b));
    }
    return 0;
}

struct UnkStrIdHandle {
    CStrId id;
    void* field_0x8;
    UnkStrIdHandle(const CStrId& id);  // fn_7100ae986c
};
void fn_7100b30454(UnkIngameMenu* menu, const UnkStrIdHandle& handle);

int fn_710109aee0(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        CStrId id(name, true);
        fn_7100b30454(fn_7100deac84(g_unk_7101d47310), reinterpret_cast<const UnkStrIdHandle&>(id));
    }
    return 0;
}

struct UnkUnit2754Item;
UnkUnit2754Item* fn_71011ba1d4(UnkGuiRoot* root, const CStrId& name);
void fn_71011f5f54(UnkUnit2754Item* self, bool enabled);

int fn_7101099ca0(lua_State* L) {
    const char* name;
    bool enabled;
    if (fn_71010c0270(L, "sb", &name, &enabled)) {
        UnkUnit2754Item* item = fn_71011ba1d4(g_unk_717271e8f8, CStrId(name, true));
        if (item)
            fn_71011f5f54(item, enabled);
    }
    return 0;
}

struct UnkGuiScriptObject_b0007 {
    void** vtable;
    void vf_0x1e8(const char* name, bool arg) {
        reinterpret_cast<void (*)(UnkGuiScriptObject_b0007*, const char*, bool)>(vtable[0x1e8 / 8])(this, name, arg);
    }
};

int fn_710109ade0(lua_State* L) {
    UnkIngameMenu* menu = fn_7100deac84(g_unk_7101d47310);
    UnkGuiDisplayObject* object =
        fn_71011d94d4(reinterpret_cast<UnkGuiDisplayObject*>(menu), CStrId("escapecomposition", true));
    if (object)
        reinterpret_cast<UnkGuiScriptObject_b0007*>(object)->vf_0x1e8("OnHide", false);
    return 0;
}

struct UnkIngameSub_b0007 {
    u8 pad_0x0[0x238];
    CStrId field_0x238;
};
UnkIngameSub_b0007* fn_7100b30d00(UnkIngameMenu* menu);

int fn_710109b790(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        if (UnkIngameSub_b0007* sub = fn_7100b30d00(fn_7100deac84(g_unk_7101d47310)))
            { CStrId id(name, true); sub->field_0x238.ReleaseAndTake(id); }
    }
    return 0;
}

int fn_7101075ad0(lua_State* L, UnkGuiDisplayObject* object);

int fn_71010987c0(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        UnkGuiDisplayObject* object = fn_71011b69d8(g_unk_717271e8f8, CRntString(name)).Get();
        if (object)
            return fn_7101075ad0(L, object);
    }
    return 0;
}

s32 fn_7101202300(const base::global::CRntString* name);  // an event's kind from its name

int fn_7101099f80(lua_State* L) {
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

int fn_710109a560(lua_State* L) {
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

void fn_71011b81b4(UnkGuiRoot* root, const CStrId& id, const CFilePathStrId& path, const CRntString& name, u32 mode,
                   bool flag);

int fn_710109a3e0(lua_State* L) {
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

UnkGuiHandle fn_71011b685c(UnkGuiRoot* root, const CRntString& name, const CRntString& path, UnkPropBag* bag);

int fn_7101098660(lua_State* L) {
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

UnkGuiDisplayObject* fn_71011b5aa4(UnkGuiRoot* root, const CStrId& name, const CRntString& path, UnkPropBag* bag);

int fn_7101098070(lua_State* L) {
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

int fn_71010990f0(lua_State* L) {
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

struct UnkCameraCtrl;
bool fn_7100023d00(const UnkCameraCtrl* object, const base::reflection::CClass* cls);

int fn_7101097ee0(lua_State* L) {
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

// "NumEmmisDefeated" (src/game_s7/fn_7100ae39e0.cpp).
extern UnkPropertyEx<> g_unk_71727593f8;

bool fn_7100395a30(CBlackboard::CSection* section, const void* name, s32* out);

// A typed value as the section setter takes it (as in src/game/CDoorLifeComponent.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

struct UnkGameManagerData_b0007 {
    u8 pad_0x0[0x968];
    CBlackboard blackboard;
};

// The property's qualified id, built on first use.
static inline UnkQualifiedId& Qid(UnkPropertyEx<>& item) {
    UnkQualifiedId& qid = ItemQualifiedId(item);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(item), g_unk_7101d470e0);
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

int fn_710109af90(lua_State* L) {
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
                    fn_71000803a4(pool, data);
            }
        }
    }
    const CStrId& Id() const { return reinterpret_cast<const CStrId&>(*this); }
};

// A request the game manager's list at +0x2128 takes: a word, an id and a list of ids.
struct UnkIdRequest_b0007 {
    u32 field_0x0;
    UnkListedId_b0007 id;
    base::UnkVector<UnkListedId_b0007> ids{0xc};

    UnkIdRequest_b0007(UnkIdArg_b0007 id) : field_0x0(0), id(id.Id()) {}
    __attribute__((always_inline)) ~UnkIdRequest_b0007() { ids.Free(); }
};

struct UnkGameManagerRequests_b0007 {
    u8 pad_0x0[0x2128];
    u8 field_0x2128[1];
};
struct UnkMlogMessage_b42a1;

int fn_710109b1a0(lua_State* L) {
    const char* name = g_unk_710162e2ed;
    if (fn_71010c0270(L, "s", &name)) {
        CStrId id(name, true);
        UnkIdRequest_b0007 request(id);
        fn_7100456b7c(reinterpret_cast<UnkGameManagerRequests_b0007*>(g_unk_7101d47310)->field_0x2128, reinterpret_cast<const UnkMlogMessage_b42a1*>(&request));
    }
    return 0;
}

void* fn_710109e770(lua_State* L, int index);
s32 fn_7101202300(const base::global::CRntString* name);  // an event's kind from its name

extern char g_unk_7101caf688[];

// A counted reference to a display object's weak handle (0x10 bytes, vtable g_unk_7101caf688).
struct UnkWeakRef_b0007 {
    char* vtable;
    UnkWeakHandle* handle;
    UnkWeakRef_b0007() : vtable(g_unk_7101caf688 + 0x10), handle(nullptr) {}
};

int fn_710109a0c0(lua_State* L) {
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
