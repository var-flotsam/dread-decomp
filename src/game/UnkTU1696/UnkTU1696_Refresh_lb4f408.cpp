// Unit 1696 of tools/tu_map.py: refreshes a file menu entry. Shows the hard mode button as unlocked from the
// user data, sets the title label to the header text of the current mode, and switches the entry to
// "NewContent" or "NoContent" from the main menu's rewards and archives. Names and layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CBlackboard.h"
#include "game/GUI/CDisplayObjectContainer.h"
#include "game/GUI/CLabel.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2553.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU2768.h"
#include "game/UnkTU57.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;

struct UnkMenuEntry_lb4f408 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();  // builds `root`
    UnkGuiDisplayObject* root;

    UnkGuiDisplayObject* const& Root() const { return root; }
};

struct UnkMenuState_lb4f408 {
    u8 pad_0x0[0x268];
    UnkGuiDisplayObject* field_0x268;
    u8 pad_0x270[0xc];
    u32 field_0x27c;
};

// The game manager's current mode name.
struct UnkGameManagerMode_lb4f408 {
    u8 pad_0x0[0x7f0];
    CRntString field_0x7f0;
};

// A composition that counts new content.
struct UnkContentComposition_lb4f408 {
    virtual const base::reflection::CClass* GetClass();
    u8 pad_0x8[0x280];
    u32 field_0x288;
};

struct UnkMainMenu_lb4f408 {
    virtual const base::reflection::CClass* GetClass();
    u8 pad_0x8[0x518];
    UnkContentComposition_lb4f408* field_0x520;  // the chozo archives
    u8 pad_0x528[0x8];
    UnkContentComposition_lb4f408* field_0x530;  // the ending rewards
};

static inline UnkGuiDisplayObject* FindByPath(UnkGuiDisplayObject* self, const CRntString& path) {
    return fn_71011d9508(self, reinterpret_cast<const char* const*>(&path));
}

static inline bool IsA(const base::reflection::CClass* type, const base::reflection::CClass* base) {
    return fn_710009d50c(reinterpret_cast<const base::reflection::CType*>(type),
                         reinterpret_cast<const base::reflection::CType*>(base));
}

// The object when it is of the class `Type` returns (or derives from it), else null.
template <const base::reflection::CClass* (*Type)()>
static inline UnkGuiDisplayObject* As(UnkGuiDisplayObject* o) {
    return o && IsA(o->GetClass(), Type()) ? o : nullptr;
}

// The composition when it is of the class `Type` returns, else null.
template <const base::reflection::CClass* (*Type)()>
static inline UnkContentComposition_lb4f408* CastTo(UnkContentComposition_lb4f408* o) {
    return IsA(o->GetClass(), Type()) ? o : nullptr;
}

static inline const char* CStr(const CRntString& s) { return s.data ? s.data : g_unk_710162e2ed; }

static inline bool Equal(const CRntString& a, const CRntString& b) {
    return fn_710007f120(reinterpret_cast<const UnkCompared_0x30*>(&a), reinterpret_cast<const UnkCompared_0x30*>(&b));
}

static inline UnkUserDataOwner* GetOwner() {
    UnkUserDataOwner* owner = g_unk_7101d47308;
    if (!owner) {
        owner = static_cast<UnkUserDataOwner*>(fn_71000839a0(0x2b8, 0x3f));
        fn_710120320c(owner);
        g_unk_7101d47308 = owner;
    }
    return owner;
}

// Calls a GUI object's virtual function by its vtable offset (the slots are not named yet).
template <u32 Offset, class... Args>
static inline void CallSlot(UnkGuiDisplayObject* o, Args... args) {
    void** vtable = *reinterpret_cast<void***>(o);
    reinterpret_cast<void (*)(UnkGuiDisplayObject*, Args...)>(vtable[Offset / 8])(o, args...);
}

void SetFileMenuHardModeButtonTitleAndContentState(UnkMenuEntry_lb4f408* self, UnkMenuState_lb4f408* state) {
    if (!self->root)
        self->vf_0x10();
    state->field_0x268 = self->root;
    state->field_0x27c = 4;

    UnkGuiDisplayObject* content = As<fn_71011d7b84>(FindByPath(self->Root(), CRntString("Content")));
    if (!content)
        return;

    if (UnkGuiDisplayObject* button = As<fn_71011be400>(FindByPath(content, CRntString("ListComposition.Button3")))) {
        CBlackboard* data = fn_710120384c(GetOwner());
        bool unlocked = false;
        {
            CStrId section("USER_DATA", true);
            CStrId key("HardModeUnlocked", true);
            if (CBlackboard::CSection* s = fn_7101047270(data, section))
                fn_710006d11c(s, &key, &unlocked);
        }
        fn_71011cdce8(button, unlocked);
    }

    const CRntString& mode = reinterpret_cast<UnkGameManagerMode_lb4f408*>(g_unk_7101d47310)->field_0x7f0;
    u32 header;
    if (Equal(mode, fn_7100df703c(0)))
        header = 1;
    else if (Equal(mode, fn_7100df703c(1)))
        header = 2;
    else if (Equal(mode, fn_7100df703c(2)))
        header = 3;
    else
        header = 1;

    if (UnkGuiDisplayObject* title = As<fn_71011e8dfc>(FindByPath(content, CRntString("CLabel-Title")))) {
        CRntString text("#GUI_FILEMENU_HEADER_");
        fn_710007f634(&text, header);
        fn_71011ec424(title, CRntString(CStr(text)));
        fn_71011cd5a0(title);
    }

    UnkGuiDisplayObject* root = self->root;
    UnkGuiDisplayObject* new_content = As<fn_71011f6890>(FindByPath(root, CRntString("Content.NewContent")));
    UnkGuiDisplayObject* glow = As<fn_71011f6890>(FindByPath(root, CRntString("Content.NewContent_glow")));
    if (!new_content || !glow)
        return;
    CallSlot<0x78, s32, bool>(new_content, -2100, false);
    CallSlot<0x78, s32, bool>(glow, -2000, false);

    UnkMainMenu_lb4f408* menu = reinterpret_cast<UnkMainMenu_lb4f408*>(fn_7100e033a8(g_unk_7101d47310));
    if (!menu || !IsA(menu->GetClass(), fn_7100b34230()))
        return;
    u32 new_rewards = CastTo<fn_7100b1c740>(menu->field_0x530)->field_0x288;
    u32 new_archives = CastTo<fn_7100af6844>(menu->field_0x520)->field_0x288;
    if (new_rewards | new_archives)
        CallSlot<0x1e8, const char*, bool>(root, "NewContent", false);
    else
        CallSlot<0x1e8, const char*, bool>(root, "NoContent", false);
}
