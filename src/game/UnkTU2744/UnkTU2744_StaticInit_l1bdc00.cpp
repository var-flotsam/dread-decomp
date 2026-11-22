// Unit 2744 (gui/scripts/guimanager.lua): the unit's static initialiser (helpers in game/UnkTUInit.h).
// The header ("sources" first, the word last) with its function records, GUI::CGUIManager and its
// three containers' classes, and the property ids the manager's events use. Names and layouts are
// placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/reflection/CClass.h"
#include "game/UnkTU2744.h"
#include "game/UnkTUInit.h"
#include "types.h"

// The shared helpers see a reflected type through the placeholder layout (UnkReflType).
static inline UnkReflType* AsUnk(base::reflection::CType* t) { return reinterpret_cast<UnkReflType*>(t); }

// The unit's data: the header built the other way round, the event property ids and the function
// records.
struct UnkTU2744Data {
    u32 field_0x0;
    CStrId sources;   // 0x8
    CStrId binaries;  // 0x10
    CStrId selected_backwards;
    CStrId user_index;
    CStrId key;
    CStrId virtual_key;
    CStrId digital_hefty;
    CStrId parent_centered_x;
    CStrId parent_centered_y;
    u8 pad_0x50[0x20];
    UnkFnRecord fns[3];  // 0x70
};
static_assert(__builtin_offsetof(UnkTU2744Data, selected_backwards) == 0x18, "");
static_assert(__builtin_offsetof(UnkTU2744Data, fns) == 0x70, "");
extern UnkTU2744Data g_unk_7172a0d0b8 __attribute__((visibility("hidden")));

// GUI::CGUIManager
extern UnkReflType g_unk_7172a0c778;
void fn_71011b0210(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(),
                   void* b);  // GUI::CGUIManager::refl_InitClass
void fn_71011b020c();
extern char g_unk_7172a0c8b0;

// GUI::CGUIManager::SpriteSheetContainer
extern base::reflection::CType g_unk_7172a0c8b8;
void fn_71011b0640(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71011b0358();
extern char g_unk_7172a0c9f0;

// GUI::CGUIManager::SkinContainer
extern base::reflection::CType g_unk_7172a0c9f8;
void fn_71011b0b60(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71011b0918();
extern char g_unk_7172a0cb30;

// GUI::CGUIManager::ShapeContainer
extern base::reflection::CType g_unk_7172a0cb38;
void fn_71011b1354(UnkReflType* self, const CStrId& name, UnkReflType* base, void* a, void (*init)(), void* b);
void fn_71011b0ce4();
extern char g_unk_7172a0cc70;

// The containers' std::unique_ptr types.
extern char g_unk_7172a0cc71;
extern char g_unk_7172a0cc72;
extern char g_unk_7172a0cc73;

static inline void InitId(CStrId& id, const char* name) {
    new (&id) CStrId(name, true);
    fn_7100000250(CStrId_Destroy, &id, &g_unk_7101a19000);
}

void fn_71011bdc00() {
    UnkTU2744Data& tu = g_unk_7172a0d0b8;
    InitId(tu.sources, "sources");
    InitId(tu.binaries, "binaries");
    tu.field_0x0 = 6;
    InitRecord(tu.fns[0], fn_710000cf00);
    InitRecord(tu.fns[1], fn_710000d470);
    InitRecord(tu.fns[2], fn_710000d4f4);

    InitClass(&g_unk_7172a0c778, "GUI::CGUIManager", fn_71011b0210, fn_71011b020c);
    fn_710009ec00(&g_unk_7172a0c8b0, &g_unk_7172a0c778);
    InitClass(AsUnk(&g_unk_7172a0c8b8), "GUI::CGUIManager::SpriteSheetContainer", fn_71011b0640, fn_71011b0358);
    fn_710009ec00(&g_unk_7172a0c9f0, &g_unk_7172a0c8b8);
    InitClass(AsUnk(&g_unk_7172a0c9f8), "GUI::CGUIManager::SkinContainer", fn_71011b0b60, fn_71011b0918);
    fn_710009ec00(&g_unk_7172a0cb30, &g_unk_7172a0c9f8);
    InitClass(AsUnk(&g_unk_7172a0cb38), "GUI::CGUIManager::ShapeContainer", fn_71011b1354, fn_71011b0ce4);
    fn_710009ec00(&g_unk_7172a0cc70, &g_unk_7172a0cb38);
    fn_710009ec00(&g_unk_7172a0cc71, refl_TypeOf<fn_71011b1924>());
    fn_710009ec00(&g_unk_7172a0cc72, refl_TypeOf<fn_71011b1b28>());
    fn_710009ec00(&g_unk_7172a0cc73, refl_TypeOf<fn_71011b1d2c>());

    InitId(tu.selected_backwards, "SelectedBackwards");
    InitId(tu.user_index, "uUserIndex");
    InitId(tu.key, "sKey");
    InitId(tu.virtual_key, "iVirtualKey");
    InitId(tu.digital_hefty, "digital_hefty");
    InitId(tu.parent_centered_x, "v2ParentCenteredPositionX");
    InitId(tu.parent_centered_y, "v2ParentCenteredPositionY");
}
