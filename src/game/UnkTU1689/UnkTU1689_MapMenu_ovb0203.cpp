// The map menu's unit (tools/tu_map.py unit 1689, hint GUI::CMapMenuController): the controller's open and
// close messages and flags, its id list, its state steps, the page list wiring and a few lookups. Names and
// layouts are placeholders.
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/CBlackboard.h"
#include "game/GUI/CIngameMenu.h"
#include "game/UnkGameManager.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiHandle.h"
#include "base/reflection/UnkReflAttributes.h"
#include "game/UnkGuiRoot.h"
#include "game/UnkItemProperties.h"
#include "game/UnkTU1600.h"
#include "game/UnkTU1689.h"
#include "game/UnkTU2566.h"
#include "game/UnkTU2567.h"
#include "game/UnkTUProperty.h"
#include "types.h"

using base::global::CStrId;
using base::global::UnkStrIdBase;

// A list of handles kept packed: the count and then the handles, from an offset that is only 4-aligned.
struct __attribute__((packed)) UnkHandleList_ovb0203 {
    u32 count;
    UnkGuiHandleObj* handles[1];
};

struct UnkMapMenuController_ovb0203;

// An object the controller notifies: it has a vtable with at least eight slots.
struct UnkMenuListener_ovb0203 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
};

struct UnkMapEntry_ovb0203 {
    u8 pad_0x0[0x10];
    UnkMapMenuController_ovb0203* controller;  // 0x10
};

struct UnkMapMenuEntry_ovb0203 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10(s32 previous);
    virtual void vf_0x18(s32 next);
    u8 pad_0x8[0x8];
    s32 kind;  // 0x10
};

// The map data (fn_7100e8d8b0): the zoom, the layer values and the map menu it belongs to.
struct UnkMapData {
    u8 pad_0x0[0xbc];
    f32 field_0xbc;
    u8 pad_0xc0[0x4];
    f32 field_0xc4;
    u8 pad_0xc8[0x48];
    CStrId field_0x110;
    u8 field_0x118[1];
    u8 pad_0x119[0xd63d30 - 0x119];
    f32 bounds[4];  // the map's box: min x, min y, max x, max y
    u8 pad_0xd63d40[0xd63f40 - 0xd63d40];
    UnkMapEntry_ovb0203* field_0xd63f40;
};

struct UnkMapMenuController_ovb0203 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    u8 pad_0x8[0x4];
    f32 field_0xc;  // 0xc
    u8 transition_0x10[0x50];  // 0x10
    u8 pad_0x60[0x54];
    bool field_0xb4;  // 0xb4
    u8 pad_0xb5[0x2b];
    f32 field_0xe0;  // 0xe0
    u8 pad_0xe4[0x1];
    bool field_0xe5;  // 0xe5
    u8 pad_0xe6[0xa];
    s32 mode;  // 0xf0
    u8 pad_0xf4[0xc];
    f32 timer;  // 0x100
    s32 state;  // 0x104
    u8 pad_0x108[0x128];
    bool field_0x230;  // 0x230
    bool field_0x231;  // 0x231
    u8 pad_0x232[0x2];
    u32 field_0x234;  // 0x234
    UnkGuiHandleObj* handle_0x238;  // 0x238
    u8 pad_0x240[0x10];
    UnkGuiHandleObj* handle_0x250;  // 0x250
    UnkGuiHandleObj* handle_0x258;  // 0x258
    u8 pad_0x260[0x10];
    UnkHandleList_ovb0203 handles;  // 0x270
    u8 pad_0x27c[0x84];
    UnkGuiHandleObj* handle_0x300;  // 0x300
    UnkGuiHandleObj* handle_0x308;  // 0x308
    UnkGuiHandleObj* handle_0x310;  // 0x310
    UnkGuiHandleObj* handle_0x318;  // 0x318
    UnkGuiHandleObj* handle_0x320;  // 0x320
    UnkGuiHandleObj* handle_0x328;  // 0x328
    u8 pad_0x330[0x8];
    UnkGuiHandleObj* handle_0x338;  // 0x338
    UnkGuiHandleObj* handle_0x340;  // 0x340
    u8 pad_0x348[0x48];
    UnkGuiHandleObj* field_0x390;  // 0x390
    void** items;  // 0x398
    CStrId* items_ids;  // 0x3a0
    u32 items_count;  // 0x3a8
    u8 pad_0x3ac[0x14];
    u8 transition_0x3c0[8];  // 0x3c0
    u8 pad_0x3c8[0xd0];
    CStrId field_0x498;  // 0x498
    CStrId field_0x4a0;  // 0x4a0
    CStrId id;  // 0x4a8
    u32 index;  // 0x4b0
    bool field_0x4b4;  // 0x4b4
    bool field_0x4b5;  // 0x4b5
    u8 pad_0x4b6[0x1a];
    base::global::CRntString field_0x4d0;  // 0x4d0
    u8 pad_0x500[0x118];
    u8 transition_0x618[0xd0];  // 0x618
    f32 field_0x6e8;  // 0x6e8
    u8 pad_0x6ec[0x4];
    s32 mode_0x6f0;  // 0x6f0
    u8 pad_0x6f4[0x1c4];
    CStrId* ids;  // 0x8b8
    u32 id_count;  // 0x8c0
    u8 pad_0x8c4[0x24];
    bool field_0x8e8;  // 0x8e8
    bool field_0x8e9;  // 0x8e9
    u8 pad_0x8ea[0x6];
    base::global::UnkStringData** known_ids;  // 0x8f0
    u32 known_count;  // 0x8f8
    u8 pad_0x8fc[0x2c];
    UnkMapMenuEntry_ovb0203* field_0x928;  // 0x928
    UnkMapMenuEntry_ovb0203* previous_entry;  // 0x930
    u32 entry_count;  // 0x938
    u8 pad_0x93c[0x80];
    s32 field_0x9bc;  // 0x9bc
    bool field_0x9c0;  // 0x9c0
    u8 pad_0x9c1[0x7e7];
    UnkMenuListener_ovb0203* field_0x11a8;  // 0x11a8
};

// As src/game/GUI/CMapMenuController_Timers_b43.cpp and src/game/GUI/CMapMenuController_Close.cpp declare them.
struct UnkStateSink;
extern UnkStateSink* g_unk_717275c380;
struct UnkStateKey;
extern UnkStateKey g_unk_71729f0178;
void fn_7100fc9d78(UnkStateSink* sink, UnkStateKey* key, void* state);
struct UnkMapMenuControllerClose;

// ---- fn_7100b37b80
extern UnkStrIdBase g_unk_71727c7fe8;
extern UnkStrIdBase g_unk_71727c7ff0;
extern UnkStrIdBase g_unk_71727c7ff8;
extern UnkStrIdBase g_unk_71727c8000;
extern UnkStrIdBase g_unk_71727c8008;
extern UnkStrIdBase g_unk_71727c8010;
extern UnkStrIdBase g_unk_71727c8018;
extern UnkStrIdBase g_unk_71727c8020;
extern UnkStrIdBase g_unk_71727c8028;
extern UnkTunable g_unk_71727ca410;
extern const base::math::CVector3D g_unk_71016477a8;

static inline const base::math::CVector3D* Colors_ovb0203() {
    return reinterpret_cast<const base::math::CVector3D*>(&g_unk_71727ca410);
}

// The colour of a minimap marker's kind: one of nine class pointers picks its entry of the table.
const base::math::CVector3D* fn_7100b37b80(const void* kind) {
    const void* type = *static_cast<const void* const*>(kind);
    if (type == g_unk_71727c7fe8.data)
        return &Colors_ovb0203()[46];
    if (type == g_unk_71727c7ff0.data)
        return &Colors_ovb0203()[47];
    if (type == g_unk_71727c7ff8.data)
        return &Colors_ovb0203()[48];
    if (type == g_unk_71727c8000.data)
        return &Colors_ovb0203()[49];
    if (type == g_unk_71727c8008.data)
        return &Colors_ovb0203()[50];
    if (type == g_unk_71727c8010.data)
        return &Colors_ovb0203()[51];
    if (type == g_unk_71727c8018.data)
        return &Colors_ovb0203()[52];
    if (type == g_unk_71727c8020.data)
        return &Colors_ovb0203()[53];
    return type == g_unk_71727c8028.data ? &Colors_ovb0203()[54] : &g_unk_71016477a8;
}

// ---- fn_7100b44ae4
extern CStrId g_unk_71727cc990;
extern UnkPropertyEx<> g_unk_71727639d0;

// Sends the open or close message to the object behind the handle at +0x390 (vtable slot 0x1e8).
#define SEND_MENU_MESSAGE(self, open, close)                                                      \
    do {                                                                                          \
        void** target = reinterpret_cast<void**>((self)->field_0x390->target);                    \
        const char* name = (self)->field_0x8e8 ? (open) : (close);                                \
        void** vtable = *reinterpret_cast<void***>(target);                                       \
        reinterpret_cast<void (*)(void*, const char*, u32)>(vtable[0x1e8 / 8])(target, name, 0);  \
    } while (0)

void fn_7100b44ae4(UnkMapMenuController_ovb0203* self) {
    self->field_0x8e8 = true;
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_71727639d0);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_71727639d0), g_unk_7101d470e0);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, g_unk_71727cc990))
        fn_710006d11c(section, &qid.id, &self->field_0x8e8);
    UnkMapMenuEntry_ovb0203* entry = self->field_0x928;
    if (!entry)
        return;
    switch (entry->kind) {
    case 1:
        SEND_MENU_MESSAGE(self, "OpenMenuArea", "CloseMenuArea");
        break;
    case 2:
        SEND_MENU_MESSAGE(self, "OpenMenuGlobal", "CloseMenuGlobal");
        break;
    }
}

// ---- fn_7100b44e88
extern const base::math::CVector2D g_unk_7101647780;
void fn_7100b3dbe0(UnkMapMenuController_ovb0203* self, const base::math::CVector2D* __attribute__((noescape)) pos);

void fn_7100b44e88(UnkMapMenuController_ovb0203* self, bool keep) {
    fn_71011cdce8(reinterpret_cast<UnkGuiDisplayObject*>(self), true);
    {
        base::math::CVector2D pos;
        pos.x = g_unk_7101647780.x;
        pos.y = g_unk_7101647780.y;
        fn_7100b3dbe0(self, &pos);
    }
    if (!keep)
        fn_7100fc9d78(g_unk_717275c380, &g_unk_71729f0178, self);
}

// ---- fn_7100b44f08
void fn_7100b44f08(UnkMapMenuController_ovb0203* self) {
    for (u64 i = 0; i < self->id_count; ++i)
        self->ids[i].Reset();
    self->id_count = 0;
    fn_710007eaa0(&self->field_0x4d0, &g_unk_7101d58d10);
}

// ---- fn_7100b459e4
void fn_7100ea6234(UnkMapData* self, const CStrId* id);
struct UnkIdArrayOwner;
void fn_7100e9b5d0(UnkIdArrayOwner* self);
extern CStrId g_unk_71727c82d8;
extern CStrId g_unk_71727c82e0;
extern CStrId g_unk_71727c82e8;
extern CStrId g_unk_71727c82f0;
extern CStrId g_unk_71727c82f8;
extern CStrId g_unk_71727c8300;
extern CStrId g_unk_71727c8308;
extern CStrId g_unk_71727c8310;
void fn_7100b3cb50(UnkMapMenuController_ovb0203* self);

void fn_7100b459e4(UnkMapMenuController_ovb0203* self, bool flag) {
    self->field_0x8e9 = flag;
    fn_7100b3cb50(self);
    if (flag) {
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c82d8);
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c82e0);
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c82e8);
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c82f0);
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c82f8);
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c8300);
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c8308);
        fn_7100ea6234(fn_7100e8d8b0(), &g_unk_71727c8310);
    } else {
        fn_7100e9b5d0(reinterpret_cast<UnkIdArrayOwner*>(fn_7100e8d8b0()));
    }
    self->field_0x234 = flag;
    UnkMapMenuEntry_ovb0203* entry = self->field_0x928;
    fn_7100b3e5ac(reinterpret_cast<UnkMapMenuControllerClose*>(self), entry ? entry->kind : 0x7fffffff, 0);
}

// ---- fn_7100b464e0
void fn_7100ea64b4(UnkMapData* self);
void fn_7100e9d7fc(UnkMapData* self);
void fn_7100e91a28(UnkMapData* self, bool flag);
struct UnkEnumType;
extern UnkEnumType g_unk_71729f0238;

void fn_7100b464e0(UnkMapMenuController_ovb0203* self) {
    switch (self->mode) {
    case 1: {
        self->mode = 2;
        fn_7100ea64b4(fn_7100e8d8b0());
        UnkMapEntry_ovb0203* entry = fn_7100e8d8b0()->field_0xd63f40;
        void* state = entry ? entry->controller : nullptr;
        fn_7100fc9d78(g_unk_717275c380, reinterpret_cast<UnkStateKey*>(&g_unk_71729f0238), state);
        self->field_0xc = 0.1f;
        break;
    }
    case 2:
        self->mode = 3;
        fn_7100e9d7fc(fn_7100e8d8b0());
        fn_7100e91a28(fn_7100e8d8b0(), true);
        self->field_0xc = 1.5f;
        break;
    case 3:
        self->vf_0x20();
        break;
    }
}

// ---- fn_7100b46998
void fn_7100b45cf8(UnkMapMenuController_ovb0203* self, f32 a, f32 b, f32 c);
extern u8 g_unk_710164b918[] __attribute__((visibility("hidden")));
void fn_7100e910fc(UnkMapData* map, const void* name);
// A rectangle given as float bits (the compiler keeps these four stores apart that way).
struct UnkRect_ovb0203 {
    u32 x0, y0, x1, y1;
};
void fn_7100e79654(void* target, const UnkRect_ovb0203* rect, u32 mode, u32 flags, void* extra);

void fn_7100b46998(UnkMapMenuController_ovb0203* self) {
    if (self->mode != 1)
        return;
    UnkMapData* map = fn_7100e8d8b0();
    fn_7100e910fc(map, g_unk_710164b918);
    fn_7100b45cf8(self, map->field_0xc4 * 0.3f, 0.0f, 1.0f);
    UnkRect_ovb0203 rect;
    rect.x0 = 0xc52f0000;  // -2800.0f
    rect.y0 = 0x459ab000;  // 4950.0f
    rect.x1 = 0x44480000;  // 800.0f
    rect.y1 = 0x45c99000;  // 6450.0f
    fn_7100e79654(map->field_0x118, &rect, 4, 0, nullptr);
    self->field_0xc = 2.5f;
    self->mode = 2;
}

// ---- fn_7100b46f48
void fn_7100b48c44(UnkMapMenuController_ovb0203* self);
void fn_7100b48c48(UnkMapMenuController_ovb0203* self);

struct UnkButtonCallback_ovb0203 {
    UnkMapMenuController_ovb0203* object;
    void (*function)(UnkMapMenuController_ovb0203*);
};

struct UnkGuiComposition_ovb0090;
struct UnkMapCallback_ovb0090;
void fn_7100b5fde0(UnkGuiComposition_ovb0090* gui, const CStrId* text, const CStrId* accept, const CStrId* back,
                   const UnkMapCallback_ovb0090* on_accept, const UnkMapCallback_ovb0090* on_back, u32 mode,
                   f32 time);
extern CStrId g_unk_7172763e08[] __attribute__((visibility("hidden")));

struct UnkGuiRef_ovb0203 {
    u8 pad_0x0[0x10];
    UnkGuiComposition_ovb0090* composition;  // 0x10
};

void fn_7100b46f48(UnkMapMenuController_ovb0203* self, f32 dt) {
    if (self->state != 2)
        return;
    self->timer = self->timer - dt;
    if (self->timer <= 0.0f) {
        UnkGuiRef_ovb0203* ref = static_cast<UnkGuiRef_ovb0203*>(g_unk_7101d47310->field_0x2258);
        if (ref && ref->composition) {
            UnkButtonCallback_ovb0203 accept_callback = {self, fn_7100b48c44};
            UnkButtonCallback_ovb0203 back_callback = {self, fn_7100b48c48};
            fn_7100b5fde0(ref->composition, &g_unk_7172763e08[2], &g_unk_7172763e08[1], &g_unk_7172763e08[0],
                          reinterpret_cast<const UnkMapCallback_ovb0090*>(&accept_callback),
                          reinterpret_cast<const UnkMapCallback_ovb0090*>(&back_callback), 2, -1.0f);
        }
        self->state = 3;
    }
}

// ---- fn_7100b43204
static inline UnkGuiDisplayObject* TargetOf_ovb0203(UnkGuiHandleObj* handle) {
    return handle ? handle->target : nullptr;
}

void fn_7100b43204(UnkMapMenuController_ovb0203* self, bool flag) {
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x318), flag);
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x320), flag);
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x328), flag);
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x300), flag);
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x308), flag);
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x310), flag);
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x338), flag);
    fn_71011cdce8(TargetOf_ovb0203(self->handle_0x340), flag);
}

// ---- fn_7100b46e10
void fn_7100e9cbac(void* world, const CStrId* current, CStrId* id, bool flag);
void fn_7100e9336c(UnkMapData* self);
struct UnkMapMenuController;
// The base handler takes a flag here, where src/game/GUI/CMapMenuController_3.cpp passes an argument pointer: the
// same symbol under a flag signature.
void fn_7100b43b30_flag(UnkMapMenuController* self, const CStrId* id, u32 flag) asm("_Z13fn_7100b43b30P20UnkMapMenuControllerPKN4base6global6CStrIdEPv");
namespace unk_gui {
void fn_7100eb7358(void* object, u32 a, u32 b);
}

// A slot of the map data that holds an id and a value.
struct UnkMapIdSlot_ovb0203 {
    CStrId id;
    u32 value;
};

struct UnkMapData2_ovb0203 {
    u8 pad_0x0[0xd641d0];
    UnkMapIdSlot_ovb0203 field_0xd641d0;
};

struct UnkMapMenuTarget2_ovb0203 {
    u8 pad_0x0[0x4b4];
    bool field_0x4b4;
    u8 pad_0x4b5[0x163];
    u8 field_0x618[1];
};

struct UnkMapMenuStage_ovb0203 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    u8 pad_0x8[0xe8];
    UnkGuiHandleObj* handle;  // 0xf0
    CStrId id;                // 0xf8
    f32 timer;                // 0x100
    s32 state;                // 0x104
};

void fn_7100b46e10(UnkMapMenuStage_ovb0203* self, bool enter) {
    if (!enter)
        return;
    UnkMapData* map = fn_7100e8d8b0();
    UnkGuiDisplayObject* target = self->handle ? self->handle->target : nullptr;
    fn_7100e9cbac(map, reinterpret_cast<const CStrId*>(reinterpret_cast<char*>(target) + 0x548), &self->id, true);
    fn_7100e9336c(fn_7100e8d8b0());
    UnkMapIdSlot_ovb0203* slot = &reinterpret_cast<UnkMapData2_ovb0203*>(fn_7100e8d8b0())->field_0xd641d0;
    slot->id = self->id;
    slot->value = 0;
    UnkMapEntry_ovb0203* entry = fn_7100e8d8b0()->field_0xd63f40;
    UnkMapMenuTarget2_ovb0203* menu = reinterpret_cast<UnkMapMenuTarget2_ovb0203*>(entry ? entry->controller : nullptr);
    fn_7100b43b30_flag(reinterpret_cast<UnkMapMenuController*>(menu), &self->id, 0);
    menu->field_0x4b4 = true;
    if (self->id.data == &g_unk_7101d47108->empty)
        unk_gui::fn_7100eb7358(menu->field_0x618, 0, 1);
    self->timer = 2.0f;
    self->state = 2;
}

// ---- fn_7100b3c9f4
void fn_7100b3cf40(UnkMapMenuController_ovb0203* self, u32 flag);

void fn_7100b3c9f4(UnkMapMenuController_ovb0203* self) {
    if (self->items_count) {
        void** end = self->items + self->items_count;
        for (void** item = self->items; item != end; ++item) {
            void* object = *item;
            fn_71011b57c4(g_unk_717271e8f8, object);
        }
    }
    if (self->items_ids) {
        for (u64 i = 0; i < self->items_count; ++i)
            self->items_ids[i].Reset();
    }
    self->items_count = 0;
    self->id.Reset();
    self->index = 0;
    if (self->field_0x8e9) {
        fn_7100b3cb50(self);
        fn_7100b3cf40(self, 0);
    }
    if (self->field_0x11a8)
        self->field_0x11a8->vf_0x38();
}

// ---- fn_7100b42148
extern UnkPropertyEx<> g_unk_7172759478;
void fn_7100b422a8();
void fn_7100b42be0(UnkMenuListener_ovb0203* listener, void* arg);
struct UnkStateOwner;

struct UnkGameManagerIdTable {
    u8 pad_0x0[0x2578];
    UnkGameManagerIdTable() asm("fn_7100031894");
    ~UnkGameManagerIdTable() asm("fn_71000407f0");
};

inline const CStrId& SectionId_ovb0203(u32 offset) {
    static UnkGameManagerIdTable s_table asm("g_unk_7101d552d0");
    return *reinterpret_cast<const CStrId*>(s_table.pad_0x0 + offset);
}

struct UnkIngameMenuState_ovb0203 {
    u8 pad_0x0[0x2e0];
    s32 state;  // 0x2e0
};

void fn_7100b42148(UnkMapMenuController_ovb0203* self, void* arg) {
    UnkGameManager* gm = g_unk_7101d47310;
    if (reinterpret_cast<UnkIngameMenuState_ovb0203*>(fn_7100deac84(gm))->state != 7)
        fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(fn_7100deac84(g_unk_7101d47310)), 7, true, true);
    bool flag = false;
    CBlackboard* blackboard = &g_unk_7101d47310->field_0x968;
    const CStrId& section_id = SectionId_ovb0203(0x1150);
    UnkQualifiedId& qid = ItemQualifiedId(g_unk_7172759478);
    if (qid.id.data == &g_unk_7101d47108->empty)
        fn_7100ae9a10(&qid, ItemSource(g_unk_7172759478), g_unk_7101d470e0);
    if (CBlackboard::CSection* section = fn_7101047270(blackboard, section_id))
        fn_710006d11c(section, &qid.id, &flag);
    UnkMenuListener_ovb0203* listener;
    if (flag) {
        fn_7100b422a8();
        listener = reinterpret_cast<UnkMenuListener_ovb0203*>(reinterpret_cast<char*>(self) + 0xcb0);
    } else {
        listener = reinterpret_cast<UnkMenuListener_ovb0203*>(reinterpret_cast<char*>(self) + 0xba8);
    }
    fn_7100b42be0(listener, arg);
    self->field_0x11a8 = listener;
    listener->vf_0x10();
}

// ---- fn_7100b46fec
// Two ids of the map menu's target (its +0x548 and +0x550).
struct UnkIdPair_7100a8e07c;
struct UnkIdPair_b49 {
    union {
        CStrId first;
    };
    union {
        CStrId second;
    };
    UnkIdPair_b49() { fn_7100a8e07c(reinterpret_cast<UnkIdPair_7100a8e07c*>(this)); }
    ~UnkIdPair_b49() {
        second.Reset();
        first.~CStrId();
    }
};
struct UnkMapTarget_ovb0203 {
    u8 pad_0x0[0x548];
    CStrId first;   // 0x548
    CStrId second;  // 0x550
};

void fn_7100b46fec(UnkMapMenuStage_ovb0203* self) {
    UnkIdPair_b49 ids;
    UnkMapTarget_ovb0203* target = reinterpret_cast<UnkMapTarget_ovb0203*>(self->handle->target);
    ids.first = target->first;
    ids.second = reinterpret_cast<UnkMapTarget_ovb0203*>(self->handle->target)->second;
    fn_7100a8f34c(self->handle ? self->handle->target : nullptr, 1, &ids);
    self->vf_0x20();
}

// ---- fn_7100b38894
struct UnkCallbackEntries_ovb0203;
void fn_7100b485ac(void* self);
void fn_7100b485b0(void* self);
void fn_7100b485b4(void* self);
void fn_7100b485f4(void* self);
void fn_7100b485f8(void* self);
void fn_7100b485fc(void* self);
void fn_7100b48600(void* self);
void fn_7100b48620(void* self);
void fn_7100b48624(void* self);
void fn_7100b48644(void* self);
void fn_7100b48648(void* self);
void fn_7100b4864c(void* self);

struct UnkCallback_ovb0203 {
    void* object;
    void (*function)(void*);
};

struct UnkEntryList_ovb0203;
// An entry of the controller's callback table (0x70 bytes): the list it belongs to, a key and three callbacks.
struct UnkCallbackEntry_ovb0203 {
    u8 pad_0x0[0x8];
    UnkEntryList_ovb0203* list;  // 0x8
    u32 key;                     // 0x10
    u8 pad_0x14[0x1c];
    UnkCallback_ovb0203 on_first;   // 0x30
    UnkCallback_ovb0203 on_second;  // 0x40
    UnkCallback_ovb0203 on_third;   // 0x50
    u8 pad_0x60[0x10];
};
static_assert(sizeof(UnkCallbackEntry_ovb0203) == 0x70, "UnkCallbackEntry_ovb0203 layout");

// The entries registered with the controller: its owner, a count and the entries (the array starts 4-aligned).
struct __attribute__((packed)) UnkEntryList_ovb0203 {
    u8 pad_0x0[0x8];
    void* owner;  // 0x8
    u8 pad_0x10[0x10];
    u32 count;  // 0x20
    u8 entries_raw[8];
};

struct UnkMapMenuCallbacks_ovb0203 {
    u8 pad_0x0[0x918];
    UnkEntryList_ovb0203 list;  // 0x918
    u8 pad_0x944[0x9c8 - 0x944];
    UnkCallbackEntry_ovb0203 entry[4];  // 0x9c8
};

// Adds the entry to the list unless one with its key is there.
static inline void AddEntry_ovb0203(UnkEntryList_ovb0203* list, UnkCallbackEntry_ovb0203* entry) {
    UnkCallbackEntry_ovb0203** entries = reinterpret_cast<UnkCallbackEntry_ovb0203**>(list->entries_raw);
    u32 count = list->count;
    UnkCallbackEntry_ovb0203** end = entries + count;
    for (UnkCallbackEntry_ovb0203** it = entries; it != end; ++it) {
        if ((*it)->key == entry->key)
            return;
    }
    entry->list = list;
    *end = entry;
    list->count = count + 1;
}

void fn_7100b38894(UnkMapMenuCallbacks_ovb0203* self) {
    self->list.owner = self;
    AddEntry_ovb0203(&self->list, &self->entry[0]);
    self->entry[0].on_first = {self, fn_7100b485ac};
    self->entry[0].on_third = {self, fn_7100b485b0};
    self->entry[0].on_second = {self, fn_7100b485b4};
    AddEntry_ovb0203(&self->list, &self->entry[1]);
    self->entry[1].on_first = {self, fn_7100b485f4};
    self->entry[1].on_third = {self, fn_7100b485f8};
    self->entry[1].on_second = {self, fn_7100b485fc};
    AddEntry_ovb0203(&self->list, &self->entry[2]);
    self->entry[2].on_first = {self, fn_7100b48600};
    self->entry[2].on_third = {self, fn_7100b48620};
    self->entry[2].on_second = {self, fn_7100b48624};
    AddEntry_ovb0203(&self->list, &self->entry[3]);
    self->entry[3].on_first = {self, fn_7100b48644};
    self->entry[3].on_third = {self, fn_7100b48648};
    self->entry[3].on_second = {self, fn_7100b4864c};
}

// ---- fn_7100b475d4
bool fn_7100b475d4(UnkMapMenuStage_ovb0203* self) {
    UnkMapData* map = fn_7100e8d8b0();
    UnkMapEntry_ovb0203* entry = map->field_0xd63f40;
    UnkMapMenuController_ovb0203* menu = entry ? entry->controller : nullptr;
    // keeps the compiler from assuming the menu exists
    asm("" : "+r"(menu));
    u64 count = menu->known_count;
    base::global::UnkStringData** id = menu->known_ids;
    for (; count != 0; --count, ++id) {
        if (*id == menu->id.data) {
            UnkIdPair_b49 ids;
            ids.first = map->field_0x110;
            ids.second = menu->id;
            fn_7100a8f34c(self->handle ? self->handle->target : nullptr, 1, &ids);
            self->vf_0x20();
            break;
        }
    }
    return true;
}

// ---- fn_7100b45b5c
extern CStrId g_unk_7172763d98 __attribute__((visibility("hidden")));
struct UnkMapTimersView_b43a3;
struct UnkStateOwner;

struct UnkMapCallback_ovb0203 {
    void* object;
    void (*function)(void* object);
};

struct UnkMapMenuOwner_ovb0203 {
    u8 pad_0x0[0x8];
    bool field_0x8;
    u8 pad_0x9[0xdf];
    f32 field_0xe8;
};

struct UnkMapMenuTarget_ovb0203 {
    u8 pad_0x0[0x234];
    u32 field_0x234;
    u8 pad_0x238[0x928 - 0x238];
    UnkMapMenuEntry_ovb0203* field_0x928;
    u8 pad_0x930[0xb98 - 0x930];
    UnkMapCallback_ovb0203 callback;  // 0xb98
    u8 pad_0xba8[0x11a8 - 0xba8];
    u64 field_0x11a8;
};

void fn_7100b45b5c(UnkMapMenuOwner_ovb0203* self) {
    fn_710002ffb4(&g_unk_717271e8f8->field_0xca8, &g_unk_7172763d98);
    UnkMapEntry_ovb0203* entry = fn_7100e8d8b0()->field_0xd63f40;
    UnkMapMenuTarget_ovb0203* menu = reinterpret_cast<UnkMapMenuTarget_ovb0203*>(entry ? entry->controller : nullptr);
    // keeps the compiler from assuming the menu exists
    asm("" : "+r"(menu));
    menu->field_0x234 = 0;
    UnkMapMenuEntry_ovb0203* current = menu->field_0x928;
    fn_7100b3e5ac(reinterpret_cast<UnkMapMenuControllerClose*>(menu), current ? current->kind : 0x7fffffff, 0);
    if (self->field_0x8)
        fn_7100e94054(reinterpret_cast<UnkMapTimersView_b43a3*>(g_unk_7101d47310->field_0x2228), self->field_0xe8);
    entry = fn_7100e8d8b0()->field_0xd63f40;
    menu = reinterpret_cast<UnkMapMenuTarget_ovb0203*>(entry ? entry->controller : nullptr);
    asm("" : "+r"(menu));
    bool restore = self->field_0x8;
    menu->field_0x11a8 = 0;
    if (restore)
        fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(fn_7100deac84(g_unk_7101d47310)), 1, true, true);
    if (menu->callback.function) {
        UnkMapCallback_ovb0203* cb = &menu->callback;
        cb->function(cb->object);
        __builtin_memset(cb, 0, sizeof(*cb));
    }
}

// ---- fn_7100b46744
// As src/game/UnkSubareaSetupState.cpp and src/game_s7/fn_7100fc4b34.cpp declare it.
struct UnkSubId_7100fc4b34 {
    CStrId id;
    u64 field_0x8;
};
using UnkQualifiedName_7100fc4b34 = UnkQualifiedName;
const UnkSubId_7100fc4b34& fn_71003327a4(UnkQualifiedName_7100fc4b34* self, const CStrId* name);

struct UnkMapNamedDef_ovb0203 : UnkIdSource {
    u64 field_0x8;
    UnkQualifiedName name;
};
extern UnkMapNamedDef_ovb0203 g_unk_717273e258;
extern CStrId g_unk_7172763d88 __attribute__((visibility("hidden")));

static inline UnkSubId_7100fc4b34 SubId_ovb0203(UnkMapNamedDef_ovb0203& def, const CStrId& name) {
    if (!def.name.str || !def.name.str[0])
        fn_7100ae9d7c(&def.name, &def, g_unk_7101d470e0);
    return fn_71003327a4(&def.name, &name);
}

// A typed value as the section setter takes it (as in src/game/CAIComponent/CAIComponent_StoreFloat_b49.cpp).
struct UnkTypedValue {
    const base::reflection::CType* type;
    void* value;
};
void fn_7100332c80(CBlackboard::CSection* section, UnkPropertyName* name, const UnkTypedValue* value);

void fn_7100b46744(UnkMapMenuController_ovb0203* self, f32 dt) {
    if (self->field_0xc > 0.0f) {
        self->field_0xc = self->field_0xc - dt;
        if (self->field_0xc <= 0.0f)
            self->vf_0x40();
    }
    if (self->field_0xe5) {
        fn_7100eb7470(self->transition_0x10, dt);
        fn_7100e94054(reinterpret_cast<UnkMapTimersView_b43a3*>(g_unk_7101d47310->field_0x2228), self->field_0xe0);
    }
    if (self->mode == 3) {
        f32 previous = self->timer;
        f32 progress = dt * 0.5f;
        progress += previous;
        self->timer = progress < 1.0f ? progress : 1.0f;
        UnkGameManager* gm = g_unk_7101d47310;
        CBlackboard* blackboard = &gm->field_0x968;
        CStrId section = fn_7100dfbda4(gm, reinterpret_cast<const CStrId&>(g_unk_71727c8000));
        UnkTypedValue typed;
        base::math::CVector2D value;
        value.x = 0.0f;
        value.y = self->timer * 400.0f;
        {
            UnkSubId_7100fc4b34 id = SubId_ovb0203(g_unk_717273e258, g_unk_7172763d88);
            if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
                if (!blackboard->field_0xe8) {
                    typed.type = fn_7100024504();
                    typed.value = &value;
                    fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&id.id), &typed);
                }
            }
        }
        if (self->timer >= 1.0f) {
            self->field_0xc = 2.0f;
            self->mode = 4;
        }
    }
}

// ---- fn_7100b42ff0
struct UnkRefObject_ovb0203 {
    u8 pad_0x0[0x10];
    UnkGuiDisplayObject* target;  // 0x10
    u8 pad_0x18[0x2c];
    union {
        u32 field_0x44;
    };
};
void fn_71010c0e70(UnkGuiDisplayObject* target, const char* name, bool flag);

struct UnkMapMenuTail_ovb0203 {
    u8 pad_0x0[0xfa8];
    UnkMenuListener_ovb0203 field_0xfa8;
};
struct UnkMapMenuTail2_ovb0203 {
    u8 pad_0x0[0x10a0];
    base::global::UnkStringData* field_0x10a0;
};

void fn_7100b42ff0(void* obj, u32 a, u32 b) {
    UnkMapMenuController_ovb0203* self = static_cast<UnkMapMenuController_ovb0203*>(obj);
    if (__builtin_expect(!(a & 1), 0)) {
        fn_7100b43204(self, false);
        fn_7100b2eaac(reinterpret_cast<UnkStateOwner*>(fn_7100deac84(g_unk_7101d47310)), 7, true, true);
        UnkMenuListener_ovb0203* listener = &reinterpret_cast<UnkMapMenuTail_ovb0203*>(self)->field_0xfa8;
        self->field_0x11a8 = listener;
        listener->vf_0x10();
        return;
    }
        UnkGameManager* gm = g_unk_7101d47310;
        CBlackboard* blackboard = &gm->field_0x968;
        CStrId section = fn_7100dfbda4(gm, reinterpret_cast<const CStrId&>(g_unk_71727c8000));
        UnkTypedValue typed;
        base::math::CVector2D value;
        value.x = 0.0f;
        value.y = 400.0f;
        {
            UnkSubId_7100fc4b34 id = SubId_ovb0203(g_unk_717273e258, g_unk_7172763d88);
            if (CBlackboard::CSection* s = fn_71010472e0(blackboard, section)) {
                if (!blackboard->field_0xe8) {
                    typed.type = fn_7100024504();
                    typed.value = &value;
                    fn_7100332c80(s, reinterpret_cast<UnkPropertyName*>(&id.id), &typed);
                }
            }
        }
        if (b & 1) {
            UnkMapMenuTail2_ovb0203* tail = reinterpret_cast<UnkMapMenuTail2_ovb0203*>(self);
            if (tail->field_0x10a0 != &g_unk_7101d47108->empty) {
                UnkRefObject_ovb0203* held = reinterpret_cast<UnkRefObject_ovb0203*>(tail->field_0x10a0);
                ++held->field_0x44;
                fn_71010c0e70(reinterpret_cast<UnkRefObject_ovb0203*>(tail->field_0x10a0)->target, "b", true);
            }
        }
}
