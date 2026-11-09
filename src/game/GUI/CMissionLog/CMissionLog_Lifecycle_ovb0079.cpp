// GUI::CMissionLog's unit (tools/tu_map.py): the mission log's destructor (fn_7100b55bdc: its
// callback slot at 0x308, six counted references, the two record arrays, then the display object's
// base destructor), the step that sends event 0x12 and points the record list's id and callback at
// the list object (fn_7100b59f20), the "Button<n>" naming of a button with its text (fn_7100b599f0),
// and the copy constructor of a pooled list of 0x10-byte nodes (fn_7100b59c4c; a source without a
// pool only gives its allocator). Names and layouts are placeholders.
#include "base/UnkBlockFreeList.h"
#include "base/UnkDestructors.h"
#include "base/UnkNamedRegistry.h"
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntString.h"
#include "base/global/CStrId.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiEvent.h"
#include "types.h"

using base::UnkRefPtrMember;
using base::global::CRntString;
using base::global::CStrId;


struct UnkElem38;
template <class Elem>
struct UnkElemArray;
using UnkElem38Array = UnkElemArray<UnkElem38>;

void fn_7100b55d4c(UnkElem38Array* self);
void fn_71004567dc(void* set, void** slot);
extern char g_unk_7101a9d220[];

struct UnkMissionLog_ovb0079 {
    char* vtable;
    u8 unk_0x8[0x240];
    u8 entries_0x248[0x28];
    u8 entries_0x270[0x28];
    void* field_0x298;
    u8 unk_0x2a0[0x38];
    UnkRefPtrMember refs[6];  // 0x2d8
    void* callback_set;       // 0x308
    void* callback_obj;       // 0x310
    void* callback_fn;        // 0x318
};
static_assert(__builtin_offsetof(UnkMissionLog_ovb0079, callback_set) == 0x308, "layout");

void fn_7100b55bdc(UnkMissionLog_ovb0079* self) {
    self->vtable = g_unk_7101a9d220 + 0x10;
    self->field_0x298 = nullptr;
    if (self->callback_set) {
        fn_71004567dc(self->callback_set, &self->callback_set);
        self->callback_set = nullptr;
    }
    self->callback_fn = nullptr;
    self->callback_obj = nullptr;
    for (s32 i = 5; i >= 0; --i)
        self->refs[i].~UnkRefPtrMember();
    fn_7100b55d4c(reinterpret_cast<UnkElem38Array*>(self->entries_0x270));
    fn_7100b55d4c(reinterpret_cast<UnkElem38Array*>(self->entries_0x248));
    fn_71011d8e90(reinterpret_cast<UnkGuiDisplayObject*>(self));
}


extern CStrId g_unk_7172765068 __attribute__((visibility("hidden")));
void fn_7100a58e80(void* set, void** slot);

struct UnkMissionLogTarget_ovb0079 {
    u8 unk_0x0[0x260];
    CStrId field_0x260;
    u8 field_0x268[0x8];
};

void fn_7100b018fc(UnkMissionLogTarget_ovb0079* self, UnkGuiEvent* event);

struct UnkMissionLogEvent_ovb0079 {
    u8 unk_0x0[0x298];
    UnkMissionLogTarget_ovb0079* field_0x298;
    u8 unk_0x2a0[0x8];
    void* field_0x2a8;
    u32 field_0x2b0;
    u8 unk_0x2b4[0x54];
    void* callback_set;  // 0x308
};
static_assert(__builtin_offsetof(UnkMissionLogEvent_ovb0079, callback_set) == 0x308, "layout");

void fn_7100b59f20(UnkMissionLogEvent_ovb0079* self) {
    self->field_0x2b0 = 0;
    self->field_0x2a8 = nullptr;
    s32 kind = 0x12;
    UnkGuiEvent event;
    fn_710120235c(&event, reinterpret_cast<UnkGuiDisplayObject*>(self), &kind, nullptr);
    self->field_0x298->field_0x260 = g_unk_7172765068;
    fn_7100b018fc(self->field_0x298, &event);
    UnkMissionLogTarget_ovb0079* owner = self->field_0x298;
    void** slot = &self->callback_set;
    if (*slot) {
        fn_71004567dc(*slot, slot);
        *slot = nullptr;
    }
    void* next = owner->field_0x268;
    *slot = next;
    fn_7100a58e80(next, slot);
    fn_7101202488(&event);
}


struct UnkGuiObj;
void fn_71011ecfbc(UnkGuiObj*, const char32_t*);
struct UnkMissionLogName_ovb0079;
void fn_710007f688(UnkMissionLogName_ovb0079* self, u32 value);

struct UnkMissionLogButton_ovb0079 {
    u8 unk_0x0[0x130];
    CStrId field_0x130;
};

UnkGuiObj* fn_71011c08dc(UnkMissionLogButton_ovb0079* self);

struct UnkMissionLogName_ovb0079 {
    const char* data;
    u8 unk_0x8[0x28];
};

struct UnkMissionLogButtonText_ovb0079 {
    u64 field_0x0;
    char32_t text[1];
};

void fn_7100b599f0(void* self, u32 index, const UnkMissionLogButtonText_ovb0079* text,
                   UnkMissionLogButton_ovb0079* button) {
    if (!button)
        return;
    UnkMissionLogName_ovb0079 name;
    fn_710007e704(reinterpret_cast<base::UnkNamedEntry*>(&name), "Button");
    fn_710007f688(&name, index);
    {
        CStrId id(name.data ? name.data : g_unk_710162e2ed, true);
        button->field_0x130 = id;
    }
    fn_71011ecfbc(fn_71011c08dc(button), text->text);
    fn_710007ecd4(reinterpret_cast<CRntString*>(&name));
}

extern char g_unk_7101a9d490[];
extern char g_unk_7101a9d4b0[];

struct UnkMissionLogPool_ovb0079 {
    char* vtable;
    base::UnkAllocator* parent;
    UnkBlockFreeList blocks;
};

struct UnkMissionLogNode_ovb0079 {
    u64 value;
    UnkMissionLogNode_ovb0079* next;
};

struct UnkMissionLogList_ovb0079 {
    char* vtable;
    u32 count;
    UnkMissionLogNode_ovb0079* head;
    UnkMissionLogNode_ovb0079* tail;
    base::UnkAllocator* allocator;
    u32 flags;
    UnkMissionLogPool_ovb0079 pool;  // +0x30
    u32 field_0x68;
    bool field_0x6c;

    void PushBack(UnkMissionLogNode_ovb0079* node) {
        if (count == 0) {
            head = node;
            tail = node;
        } else {
            __builtin_memcpy(&tail->next, &node, sizeof(node));
            node->next = nullptr;
            tail = node;
        }
        ++count;
    }
};
static_assert(sizeof(UnkMissionLogList_ovb0079) == 0x70, "layout");

void fn_7100b59c4c(UnkMissionLogList_ovb0079* self, const UnkMissionLogList_ovb0079* other) {
    self->vtable = g_unk_7101a9d4b0 + 0x10;
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    self->allocator = fn_7100083990();
    self->flags = 0x13;
    self->vtable = g_unk_7101a9d490 + 0x10;
    self->pool.vtable = g_unk_7101a1a4c8 + 0x10;
    self->pool.parent = nullptr;
    fn_710008377c(&self->pool.blocks);
    self->field_0x68 = 0;
    self->field_0x6c = false;
    u32 flags = other->flags;
    self->flags = flags;
    base::UnkAllocator* source = other->pool.parent;
    if (!source) {
        self->allocator = other->allocator;
        return;
    }
    u64 other_size = other->pool.blocks.size;
    if (void* buffer = self->pool.blocks.buffer) {
        fn_71000837f4(&self->pool.blocks);
        self->pool.parent->vf_0x48(buffer);
        self->pool.parent = nullptr;
    }
    u32 capacity = (u32)(other_size / sizeof(UnkMissionLogNode_ovb0079));
    if (capacity) {
        self->pool.parent = source;
        u64 size = (u64)capacity * sizeof(UnkMissionLogNode_ovb0079);
        void* buffer = source->vf_0x40(size, 8, flags);
        fn_710008378c(&self->pool.blocks, buffer, size, sizeof(UnkMissionLogNode_ovb0079));
    }
    self->allocator = reinterpret_cast<base::UnkAllocator*>(reinterpret_cast<char*>(self) +
                                                            __builtin_offsetof(UnkMissionLogList_ovb0079, pool));
    if (self == other)
        return;
    for (UnkMissionLogNode_ovb0079* n = self->head; n;) {
        UnkMissionLogNode_ovb0079* next = n->next;
        self->allocator->vf_0x48(n);
        n = next;
    }
    self->count = 0;
    self->head = nullptr;
    self->tail = nullptr;
    for (UnkMissionLogNode_ovb0079* n = other->head; n; n = n->next) {
        void* mem = self->allocator->vf_0x40(sizeof(UnkMissionLogNode_ovb0079), 8, self->flags);
        if (!mem)
            continue;
        UnkMissionLogNode_ovb0079* node = static_cast<UnkMissionLogNode_ovb0079*>(mem);
        node->value = n->value;
        node->next = nullptr;
        self->PushBack(node);
    }
}
