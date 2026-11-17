// The map menu's unit (tools/tu_map.py unit 1689, hint GUI::CMapMenuController): the controller's constructor.
// The display object container's constructor, the vtable, the slots, lists and named parts, then the callback
// table's wiring (fn_7100b38894). Names and layouts are placeholders.
#include <new>

#include "../../game_aca57c/UnkBufferString.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/UnkBufferStringPool.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU1689.h"
#include "types.h"

using base::global::CRntString;
using base::global::CRntVector;

struct UnkNamedWithEmptyId;
struct UnkNamedWithIdFlag;
struct UnkMapMenuCallbacks_ovb0203;
void fn_7100b38610(UnkNamedWithEmptyId* self);
void fn_7100b3874c(UnkNamedWithIdFlag* self);
void fn_7100b38894(UnkMapMenuCallbacks_ovb0203* self);

extern void* g_unk_7101a9c618[];  // The controller's vtable.
extern void* g_unk_7101a9cc50[];  // The callback list's vtable.
extern void* g_unk_7101a9cba0[];  // The callback entries' vtable.
extern void* g_unk_7101a9c938[];
extern void* g_unk_7101a9c990[];
extern void* g_unk_7101a9c9e8[];
extern void* g_unk_7101a9ca98[];

using UnkString20_lb37c90 = UnkBufferString<0x20, g_unk_7101a1b5a0>;
using UnkString100_lb37c90 = UnkBufferString<0x100, g_unk_7101a194b8>;

// The empty string id, set directly (the pool is created first if there is none yet).
struct UnkEmptyStrId_lb37c90 {
    base::global::UnkStringData* data;

    UnkEmptyStrId_lb37c90() {
        data = nullptr;
        if (!g_unk_7101d47108)
            base::global::UnkEnsureStringPool();
        data = &g_unk_7101d47108->empty;
    }
};

// A growing array with its step and allocation flags, and no flag byte.
struct UnkVector_lb37c90 {
    void* data = nullptr;
    u32 count = 0;
    u32 capacity = 0;
    u32 step = 10;
    u32 flags = 0xe;
    base::UnkAllocator* allocator = fn_7100083990();
};

// A slot of the menu (0xd8 bytes): two words, an empty name, three scales of 1 and a short string.
struct UnkMapSlot_lb37c90 {
    u64 field_0x0;
    u64 field_0x8;
    u32 field_0x10;
    CRntString name;  // 0x18
    u32 field_0x48;
    alignas(8) f32 scale[3];  // 0x50
    UnkString20_lb37c90 label;  // 0x60
    u32 field_0xc8;
    alignas(8) u32 field_0xd0;
    u16 field_0xd4;

    UnkMapSlot_lb37c90()
        : field_0x0(0), field_0x8(0), field_0x10(0), name(""), field_0x48(0), scale{1.0f, 1.0f, 1.0f},
          field_0xc8(0), field_0xd0(0), field_0xd4(1) {}
};
static_assert(sizeof(UnkMapSlot_lb37c90) == 0xd8, "UnkMapSlot_lb37c90 layout");

// The named parts' base (vtable g_unk_7101a9c8e0): a flag, two zero vectors, an empty name, three scales of
// 1, a short string and a few words.
struct UnkMapNamedBase_lb37c90 {
    void** vtable;
    bool field_0x8;
    f32 field_0xc[3];
    f32 field_0x18[3];
    CRntString name;  // 0x28
    u32 field_0x58;
    alignas(8) f32 scale[3];  // 0x60
    UnkString20_lb37c90 label;  // 0x70
    u32 field_0xd8;
    alignas(8) u32 field_0xe0;
    u16 field_0xe4;
    u32 field_0xe8;
    bool field_0xec;

    UnkMapNamedBase_lb37c90()
        : vtable(g_unk_7101a9c8e0 + 2), field_0x8(true), field_0xc{}, field_0x18{}, name(""), field_0x58(0),
          scale{1.0f, 1.0f, 1.0f}, field_0xd8(0), field_0xe0(0), field_0xe4(1), field_0xe8(0), field_0xec(true) {}
};
static_assert(sizeof(UnkMapNamedBase_lb37c90) == 0xf0, "UnkMapNamedBase_lb37c90 layout");

// The part with vtable g_unk_7101a9c938: a word and the empty string id; its flag starts cleared.
struct UnkMapNamedId_lb37c90 : UnkMapNamedBase_lb37c90 {
    u64 field_0xf0;
    UnkEmptyStrId_lb37c90 id;
    u32 field_0x100;

    UnkMapNamedId_lb37c90() : field_0xf0((vtable = g_unk_7101a9c938 + 2, 0)), field_0x100(0) { field_0xec = false; }
};
static_assert(sizeof(UnkMapNamedId_lb37c90) == 0x108, "UnkMapNamedId_lb37c90 layout");

// The parts with one word more (vtables g_unk_7101a9c990 and g_unk_7101a9c9e8).
template <void** Vtable>
struct UnkMapNamedWord_lb37c90 : UnkMapNamedBase_lb37c90 {
    u32 field_0xf0;

    UnkMapNamedWord_lb37c90() : field_0xf0((vtable = Vtable + 2, 0)) {}
};
static_assert(sizeof(UnkMapNamedWord_lb37c90<g_unk_7101a9c990>) == 0xf8, "UnkMapNamedWord_lb37c90 layout");

// The part with vtable g_unk_7101a9ca98: words and two ranges that start at -1.
struct UnkMapNamedRange_lb37c90 : UnkMapNamedBase_lb37c90 {
    u32 field_0xf0;
    u64 field_0xf8;
    u64 field_0x100;
    s64 field_0x108;
    u64 field_0x110;
    s64 field_0x118;
    u32 field_0x120;

    UnkMapNamedRange_lb37c90()
        : field_0xf0((vtable = g_unk_7101a9ca98 + 2, 0)), field_0xf8(0), field_0x100(0), field_0x108(-1),
          field_0x110(0), field_0x118(-1), field_0x120(0) {}
};

// The list the callback entries register with: its owner, a count, sixteen entries and a limit.
struct UnkCallbackList_lb37c90 {
    void** vtable;
    void* owner;
    u64 field_0x10;
    u64 field_0x18;
    u32 count;
    u8 entries_raw[0x80];
    s32 limit;  // 0xa4
};
static_assert(sizeof(UnkCallbackList_lb37c90) == 0xa8, "UnkCallbackList_lb37c90 layout");

struct UnkCallback_lb37c90 {
    void* object;
    void (*function)(void*);
};

// An entry of the controller's callback table (0x70 bytes).
struct UnkCallbackEntry_lb37c90 {
    void** vtable;
    UnkCallbackList_lb37c90* list;
    u32 key;
    u64 field_0x18;
    u64 field_0x20;
    u32 field_0x28;
    u32 field_0x2c;
    UnkCallback_lb37c90 callbacks[4];  // 0x30

    explicit UnkCallbackEntry_lb37c90(u32 key)
        : list(nullptr), key(key), field_0x18(0), field_0x20(0), field_0x28(2), field_0x2c(1), callbacks{} {
        vtable = g_unk_7101a9cba0 + 2;
    }
};
static_assert(sizeof(UnkCallbackEntry_lb37c90) == 0x70, "UnkCallbackEntry_lb37c90 layout");

struct UnkMapMenuController_lb37c90 {
    void** vtable;
    u8 pad_0x8[0x230 - 0x8];
    u16 field_0x230;
    u32 field_0x234[11];
    u16 field_0x260;
    u64 field_0x268;
    u32 field_0x270;
    u8 pad_0x274[0x2d8 - 0x274];
    void* field_0x2d8[25];
    UnkVector_lb37c90 field_0x3a0;
    UnkMapSlot_lb37c90 slot_0x3c0;
    UnkEmptyStrId_lb37c90 id_0x498;
    UnkEmptyStrId_lb37c90 id_0x4a0;
    UnkEmptyStrId_lb37c90 id_0x4a8;
    u32 field_0x4b0;
    u16 field_0x4b4;
    UnkEmptyStrId_lb37c90 id_0x4b8;
    u64 field_0x4c0;
    u64 field_0x4c8;
    UnkString100_lb37c90 text;  // 0x4d0
    UnkMapSlot_lb37c90 slot_0x618;
    u8 pad_0x6f0[8];
    UnkMapSlot_lb37c90 slot_0x6f8;
    UnkMapSlot_lb37c90 slot_0x7d0;
    u64 field_0x8a8;
    u32 field_0x8b0;
    bool field_0x8b4;
    CRntVector<void*> list_0x8b8;
    UnkEmptyStrId_lb37c90 id_0x8e0;
    u8 field_0x8e8;
    u8 field_0x8e9;
    CRntVector<void*> list_0x8f0;
    UnkCallbackList_lb37c90 callback_list;  // 0x918
    u16 field_0x9c0;
    UnkCallbackEntry_lb37c90 callback_entries[4];  // 0x9c8
    u64 field_0xb88[4];
    u8 part_0xba8[0x108];  // UnkNamedWithEmptyId
    UnkMapNamedId_lb37c90 part_0xcb0;
    UnkMapNamedWord_lb37c90<g_unk_7101a9c990> part_0xdb8;
    UnkMapNamedWord_lb37c90<g_unk_7101a9c9e8> part_0xeb0;
    u8 part_0xfa8[0x108];  // UnkNamedWithIdFlag
    UnkMapNamedRange_lb37c90 part_0x10b0;
};
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, field_0x3a0) == 0x3a0, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, text) == 0x4d0, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, slot_0x618) == 0x618, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, list_0x8b8) == 0x8b8, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, callback_list) == 0x918, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, callback_entries) == 0x9c8, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, part_0xba8) == 0xba8, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, part_0xcb0) == 0xcb0, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, part_0xfa8) == 0xfa8, "layout");
static_assert(__builtin_offsetof(UnkMapMenuController_lb37c90, part_0x10b0) == 0x10b0, "layout");

void fn_7100b37c90(UnkMapMenuController_lb37c90* self) {
    fn_71011d8d24(self);
    self->field_0x230 = 0;
    self->vtable = g_unk_7101a9c618 + 2;
    for (u32& value : self->field_0x234)
        value = 0;
    self->field_0x260 = 1;
    self->field_0x268 = 0;
    self->field_0x270 = 0;
    for (void*& child : self->field_0x2d8)
        child = nullptr;
    new (&self->field_0x3a0) UnkVector_lb37c90;
    new (&self->slot_0x3c0) UnkMapSlot_lb37c90();
    new (&self->id_0x498) UnkEmptyStrId_lb37c90();
    new (&self->id_0x4a0) UnkEmptyStrId_lb37c90();
    new (&self->id_0x4a8) UnkEmptyStrId_lb37c90();
    self->field_0x4b0 = 0;
    self->field_0x4b4 = 0;
    new (&self->id_0x4b8) UnkEmptyStrId_lb37c90();
    self->field_0x4c0 = 0;
    self->field_0x4c8 = 0;
    new (&self->text) UnkString100_lb37c90();
    new (&self->slot_0x618) UnkMapSlot_lb37c90();
    new (&self->slot_0x6f8) UnkMapSlot_lb37c90();
    new (&self->slot_0x7d0) UnkMapSlot_lb37c90();
    self->field_0x8a8 = 0;
    self->field_0x8b0 = 0;
    self->field_0x8b4 = true;
    new (&self->list_0x8b8) CRntVector<void*>(0xf);
    new (&self->id_0x8e0) UnkEmptyStrId_lb37c90();
    self->field_0x8e9 = 0;
    new (&self->list_0x8f0) CRntVector<void*>(0xf);
    self->callback_list.vtable = g_unk_7101a9cc50 + 2;
    self->callback_list.owner = nullptr;
    self->callback_list.field_0x10 = 0;
    self->callback_list.field_0x18 = 0;
    self->callback_list.count = 0;
    self->callback_list.limit = 0x7fffffff;
    self->field_0x9c0 = 0;
    new (&self->callback_entries[0]) UnkCallbackEntry_lb37c90(1);
    new (&self->callback_entries[1]) UnkCallbackEntry_lb37c90(2);
    new (&self->callback_entries[2]) UnkCallbackEntry_lb37c90(3);
    new (&self->callback_entries[3]) UnkCallbackEntry_lb37c90(4);
    for (u64& value : self->field_0xb88)
        value = 0;
    fn_7100b38610(reinterpret_cast<UnkNamedWithEmptyId*>(self->part_0xba8));
    new (&self->part_0xcb0) UnkMapNamedId_lb37c90();
    new (&self->part_0xdb8) UnkMapNamedWord_lb37c90<g_unk_7101a9c990>();
    new (&self->part_0xeb0) UnkMapNamedWord_lb37c90<g_unk_7101a9c9e8>();
    fn_7100b3874c(reinterpret_cast<UnkNamedWithIdFlag*>(self->part_0xfa8));
    new (&self->part_0x10b0) UnkMapNamedRange_lb37c90();
    fn_7100b38894(reinterpret_cast<UnkMapMenuCallbacks_ovb0203*>(self));
}
