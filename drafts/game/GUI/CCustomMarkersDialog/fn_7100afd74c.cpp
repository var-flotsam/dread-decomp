// GUI::CCustomMarkersDialog's unit (tools/tu_map.py unit 1660): refreshes the selected marker. Clears
// the selection, applies the current profile entry (fn_7100afd628) to the map menu, then, while nothing
// is selected, selects the map menu's own id unless it is the "none" id or already the shown item.
// Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "base/global/CStrId.h"
#include "game/UnkGameManager.h"
#include "game/UnkGameManagerIdTable.h"
#include "types.h"

using base::global::CStrId;

struct UnkMapMenuController;
struct UnkMapMenuController_b47a3;

void* fn_7100afd628();
void fn_7100b3d16c(UnkMapMenuController* self, const CStrId* id, void* arg);
void* fn_7100b41fac(UnkMapMenuController_b47a3* self);

extern u64 g_unk_71727c8040;

struct UnkMarkerItem {
    u8 data[0xc8];
};

// A counted table of items keyed by a 64-bit id.
struct UnkMarkerTable {
    UnkCountedResource base;
    u8 pad_0x58[0xa8 - 0x58];
    UnkMarkerItem* items;
    u64* keys;
    u32 count;

    UnkMarkerItem* Find(u64 key) {
        for (u32 i = 0; i < count; ++i) {
            if (keys[i] == key)
                return &items[i];
        }
        return &items[count];
    }
    UnkMarkerItem* Get(u64 key) {
        UnkMarkerItem* item = Find(key);
        return item == &items[count] ? nullptr : item;
    }
};

// A counted reference to the table.
struct UnkMarkerTableRef {
    UnkMarkerTable* ptr;

    explicit UnkMarkerTableRef(UnkMarkerTable* table) : ptr(table) {
        if (ptr)
            ++ptr->base.ref_count;
    }
    ~UnkMarkerTableRef() {
        if (ptr && --ptr->base.ref_count == 0)
            fn_710001737c(&ptr->base);
    }
};

struct UnkMarkerMapMenu {
    u8 pad_0x0[0x4a8];
    CStrId field_0x4a8;
    u32 field_0x4b0;
};

struct UnkMarkerMapMenuSlot {
    u8 pad_0x0[0x10];
    UnkMarkerMapMenu* field_0x10;
};

struct UnkMarkerState {
    UnkMarkerMapMenuSlot* field_0x0;
    u8 pad_0x8[0x290 - 0x8];
    CStrId field_0x290;
    u32 field_0x298;
};

struct UnkMarkerWorld {
    u8 pad_0x0[0x60];
    UnkMarkerTable* field_0x60;
    u8 pad_0x68[0xd63f40 - 0x68];
    UnkMarkerState field_0xd63f40;
};

struct UnkMarkerEntry {
    u32 field_0x0;
    s32 field_0x4;
    CStrId field_0x8;
};

struct __attribute__((packed, aligned(4))) UnkMarkerShown {
    u32 field_0x0;
    UnkMarkerItem* field_0x4;
};

struct UnkCustomMarkersDialog {
    u8 pad_0x0[0x288];
    CStrId field_0x288;
    u32 field_0x290;
    s32 field_0x294;
    u8 pad_0x298[0x29d - 0x298];
    bool field_0x29d;
};

void fn_7100afd74c(UnkCustomMarkersDialog* self) {
    self->field_0x29d = false;
    self->field_0x288.Reset();

    UnkMarkerWorld* world = static_cast<UnkMarkerWorld*>(g_unk_7101d47310->field_0x2228);
    UnkMarkerState* state = &world->field_0xd63f40;
    UnkMarkerMapMenu* menu = state->field_0x0 ? state->field_0x0->field_0x10 : nullptr;

    self->field_0x294 = -1;
    if (UnkMarkerEntry* entry = static_cast<UnkMarkerEntry*>(fn_7100afd628())) {
        state->field_0x290 = entry->field_0x8;
        state->field_0x298 = 0;
        self->field_0x294 = entry->field_0x4;
        fn_7100b3d16c(reinterpret_cast<UnkMapMenuController*>(menu), &entry->field_0x8, nullptr);
    }

    CStrId id(menu->field_0x4a8);
    if (id.data != &g_unk_7101d47108->empty && self->field_0x294 == -1) {
        bool isNone = id == GameManagerIdTable().field_0x1f00;
        UnkMarkerItem* item;
        {
            UnkMarkerTableRef table(world->field_0x60);
            item = table.ptr->Get(g_unk_71727c8040);
        }
        if (!isNone) {
            UnkMarkerShown* shown = static_cast<UnkMarkerShown*>(
                fn_7100b41fac(reinterpret_cast<UnkMapMenuController_b47a3*>(menu)));
            if (shown->field_0x4 != item) {
                self->field_0x288 = id;
                self->field_0x290 = menu->field_0x4b0;
            }
        }
    }
}
