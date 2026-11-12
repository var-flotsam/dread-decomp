// CAIManager's unit (tools/tu_map.py, unit 2540): the manager's per-frame update. It updates its seven
// subsystems with the frame time, counts down two timers, stops when an EMMI component is in the state
// fn_71005436e8 tests (fn_7100da9990), and keeps two lists of screen labels: with the AI debug display
// on (the tunable g_unk_71727bc558, +0x30), each AI's label follows it and shows two of its values;
// and the floating labels rise, fade out over their last half second and are removed when their time
// is up. Names and layouts are placeholders.
#include <wchar.h>

#include "base/UnkVector.h"
#include "base/reflection/CClass.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/CAIManager/CTunableAIManager.h"
#include "game/UnkGameManager.h"
#include "game/UnkRenderManager.h"
#include "game/UnkTU257.h"
#include "game/UnkTU811.h"
#include "game/UnkTUProperty.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

struct UnkTimedRef;
struct UnkEmmyAIComponent_b46a1;
struct UnkItemBase_71011a91c4;

// The objects fn_7100552d7c lists (as src/game/CBreakableTileGroup.cpp sees them): a component at +0x10.
struct UnkListedItem {
    u8 pad_0x0[0x10];
    base::reflection::UnkReflObject* field_0x10;
};
base::UnkVector<UnkListedItem*>* fn_7100552d7c();


// An AI's state (+0x1b8 of its component): two values the debug label shows, and its mode (2: the
// label goes away).
struct UnkAIState_da92b0 {
    u8 pad_0x0[0x50];
    f32 field_0x50;
    f32 field_0x54;
    u8 pad_0x58[0x18];
    s32 mode;
};

// What a debug label follows: its position (+0x44, +0x54, +0x64), its state, and a handle at +0x240
// whose object, when it is of the class fn_7100408b2c gets and +0x4dc is set, pins the label in place.
struct UnkAITarget_da92b0 {
    u8 pad_0x0[0x44];
    f32 x;
    u8 pad_0x48[0xc];
    f32 y;
    u8 pad_0x58[0xc];
    f32 z;
    u8 pad_0x68[0x150];
    UnkAIState_da92b0* state;
    u8 pad_0x1c0[0x80];
    UnkWeakHandle* field_0x240;
};
static_assert(__builtin_offsetof(UnkAITarget_da92b0, z) == 0x64, "UnkAITarget_da92b0 placeholder layout");
static_assert(__builtin_offsetof(UnkAITarget_da92b0, state) == 0x1b8, "UnkAITarget_da92b0 placeholder layout");
static_assert(__builtin_offsetof(UnkAITarget_da92b0, field_0x240) == 0x240, "UnkAITarget_da92b0 placeholder layout");

// A screen label: its position (+0x20), whether it changed (+0x11b) and its text (+0x130), whose
// style (+0x78) holds the colour at +0x150.
struct UnkTextStyle_da92b0 {
    u8 pad_0x0[0x150];
    f32 r, g, b;
};
struct UnkText_da92b0 {
    u8 pad_0x0[0x78];
    UnkTextStyle_da92b0* style;
};
struct UnkLabel_da92b0 {
    u8 pad_0x0[0x20];
    u8 position[0xfb];
    bool changed;
    u8 pad_0x11c[0x14];
    UnkText_da92b0* text;
};
static_assert(__builtin_offsetof(UnkLabel_da92b0, changed) == 0x11b, "UnkLabel_da92b0 placeholder layout");
static_assert(__builtin_offsetof(UnkLabel_da92b0, text) == 0x130, "UnkLabel_da92b0 placeholder layout");

static UnkItemBase_71011a91c4* AsItem(UnkLabel_da92b0* label) {
    return reinterpret_cast<UnkItemBase_71011a91c4*>(label);
}

// A node of a list the pool at its end gives out and takes back (vf_0x48).
struct UnkNodePool_da92b0 {
    virtual void vf_0x0();
    virtual void vf_0x8();
    virtual void vf_0x10();
    virtual void vf_0x18();
    virtual void vf_0x20();
    virtual void vf_0x28();
    virtual void vf_0x30();
    virtual void vf_0x38();
    virtual void vf_0x40();
    virtual void Free(void* node);
};

struct UnkDebugLabel_da92b0 {
    UnkLabel_da92b0* label;
    UnkWeakHandle* target;  // its object is an UnkAITarget_da92b0
    UnkDebugLabel_da92b0* next;
};

struct UnkFloatingLabel_da92b0 {
    UnkLabel_da92b0* label;
    f32 time;
    f32 position[3];
    UnkFloatingLabel_da92b0* next;
};
static_assert(__builtin_offsetof(UnkFloatingLabel_da92b0, next) == 0x18, "UnkFloatingLabel_da92b0 placeholder layout");

template <class Node>
struct UnkNodeList_da92b0 {
    u32 count;
    Node* head;
    Node* tail;
    UnkNodePool_da92b0* pool;

    // Unlinks `node`; false when it is not in the list (the update then stops walking it).
    bool Remove(Node* node) {
        Node* prev = nullptr;
        Node* it = head;
        for (;; prev = it, it = it->next) {
            if (!it)
                return false;
            if (it == node)
                break;
        }
        if (prev) {
            prev->next = node->next;
            if (tail == node)
                tail = prev;
        } else {
            head = node->next;
        }
        if (--count == 0) {
            head = nullptr;
            tail = nullptr;
        }
        return true;
    }
};

struct UnkAIManagerLink_da92b0 {
    UnkAITarget_da92b0* object;
    UnkAIManagerLink_da92b0* next;
};


struct UnkAIManager_da92b0 {
    u8 pad_0x0[0x20];
    UnkAIManagerLink_da92b0* field_0x20;  // the AIs whose state may get a debug label (fn_7100da9bd4)
    u8 pad_0x28[0x2410 - 0x28];
    UnkWeakHandle* field_0x2410;  // while its object lives, field_0x2418 counts down to fn_7100da9194
    f32 field_0x2418;
    bool field_0x241c;  // while set, field_0x2420 counts down to zero
    u8 pad_0x241d[3];
    f32 field_0x2420;
    u8 pad_0x2424[0x2530 - 0x2424];
    UnkNodeList_da92b0<UnkDebugLabel_da92b0> debug_labels;  // +0x2530
    u8 pad_0x2550[0x2598 - 0x2550];
    bool debug_labels_made;  // +0x2598
    u8 pad_0x2599[0x25a8 - 0x2599];
    UnkNodeList_da92b0<UnkFloatingLabel_da92b0> floating_labels;  // +0x25a8
};
static_assert(__builtin_offsetof(UnkAIManager_da92b0, debug_labels) == 0x2530, "UnkAIManager placeholder layout");
static_assert(__builtin_offsetof(UnkAIManager_da92b0, floating_labels) == 0x25a8, "UnkAIManager placeholder layout");

// The subsystems (+0x80, +0x228, +0x330, +0x368, +0x3f0 and +0x470, +0x5e0), each updated with the frame time.
void fn_710063a7d4(void* subsystem, f32 dt);
void fn_7100475604(void* subsystem, f32 dt);
void fn_71003fa378(void* subsystem, f32 dt);
void fn_71006dee0c(void* subsystem, f32 dt);
void fn_71006bd5a8(void* subsystem, f32 dt);
void fn_71006a1ef0(void* subsystem, f32 dt);
void fn_7100da9990(UnkAIManager_da92b0* self, bool a, bool b, bool c);
// The render manager's projection of a world position to screen coordinates, and its screen size.
void fn_7100219698(UnkRenderManager* self, const f32* position, s32* a, s32* b, u32 c, u32 d, u32 e);
u32 fn_7100219780(UnkRenderManager* self);
u32 fn_7100219744(UnkRenderManager* self);
void fn_710020bbe4(void* position, const f32* value);
void fn_710019db08(UnkTextStyle_da92b0* style, const f32* colour);

namespace {

// A world position as the label's normalised screen position.
void ToScreen(const f32* position, f32* out) {
    UnkRenderManager* render = g_unk_7101d47300;
    s32 a;
    s32 b = 0;
    fn_7100219698(render, position, &a, &b, 0, 0, 1);
    f32 width = static_cast<f32>(fn_7100219780(render));
    f32 height = static_cast<f32>(fn_7100219744(render));
    out[0] = static_cast<f32>(a * 2) / height + -1.0f;
    out[1] = width / height * (static_cast<f32>(b * 2) / width + -1.0f);
    out[2] = 0.0f;
}

template <class Node>
void FreeLabel(Node* node) {
    if (node->label) {
        fn_71002199d8(g_unk_7101d47300, AsItem(node->label), 0);
        node->label = nullptr;
    }
}

bool DebugLabelsShown() {
    return reinterpret_cast<const u8*>(&g_unk_71727bc558)[0x30] != 0;
}

}  // namespace

// NON_MATCHING(major): written for the port from the original's behaviour (1,760 bytes); not compared
// instruction by instruction.
void fn_7100da92b0(UnkAIManager_da92b0* self, f32 dt) {
    u8* base = reinterpret_cast<u8*>(self);
    fn_710063a7d4(base + 0x80, dt);
    fn_7100475604(base + 0x228, dt);
    fn_71003fa378(base + 0x330, dt);
    fn_71006dee0c(base + 0x368, dt);
    fn_71006bd5a8(base + 0x3f0, dt);
    fn_71006bd5a8(base + 0x470, dt);
    fn_71006a1ef0(base + 0x5e0, dt);

    if (self->field_0x2410 && self->field_0x2410->target) {
        self->field_0x2418 -= dt;
        if (self->field_0x2418 <= 0.0f)
            fn_7100da9194(reinterpret_cast<UnkTimedRef*>(self));
    }
    if (self->field_0x241c) {
        self->field_0x2420 -= dt;
        if (self->field_0x2420 <= 0.0f)
            self->field_0x2420 = 0.0f;
    }

    if (!fn_7100deabe4(g_unk_7101d47310)) {
        base::UnkVector<UnkListedItem*>* items = fn_7100552d7c();
        for (u32 i = 0; i < items->count; ++i) {
            UnkListedItem* item = items->data[i];
            base::reflection::UnkReflObject* component = item ? item->field_0x10 : nullptr;
            if (!component || !fn_710009d50c(component->vf_0x0(), fn_710053b934()))
                continue;
            if (fn_71005436e8(reinterpret_cast<UnkEmmyAIComponent_b46a1*>(component))) {
                fn_7100da9990(self, true, true, true);
                break;
            }
        }
    }

    UnkNodeList_da92b0<UnkDebugLabel_da92b0>& labels = self->debug_labels;
    if (!DebugLabelsShown()) {
        for (UnkDebugLabel_da92b0* node = labels.head; node;) {
            UnkDebugLabel_da92b0* next = node->next;
            FreeLabel(node);
            ResetRef(&node->target);
            labels.pool->Free(node);
            node = next;
        }
        labels.count = 0;
        labels.head = nullptr;
        labels.tail = nullptr;
        self->debug_labels_made = false;
    } else {
        if (!self->debug_labels_made) {
            self->debug_labels_made = true;
            for (UnkAIManagerLink_da92b0* l = self->field_0x20; l; l = l->next) {
                if (l->object->state)
                    fn_7100da9bd4(self, l->object);
            }
        }
        for (UnkDebugLabel_da92b0* node = labels.head; node;) {
            UnkAITarget_da92b0* target = node->target ? reinterpret_cast<UnkAITarget_da92b0*>(node->target->target) : nullptr;
            bool gone = !target || !target->state || target->state->mode == 2 || !node->label;
            if (gone) {
                UnkDebugLabel_da92b0* next = node->next;
                if (!labels.Remove(node))
                    break;
                FreeLabel(node);
                ResetRef(&node->target);
                labels.pool->Free(node);
                node = next;
                continue;
            }
            f32 position[3];
            base::reflection::UnkReflObject* pin = target->field_0x240 ? reinterpret_cast<base::reflection::UnkReflObject*>(target->field_0x240->target)
                                                     : nullptr;
            if (pin && fn_710009d50c(pin->vf_0x0(), fn_7100408b2c()) && reinterpret_cast<u8*>(pin)[0x4dc]) {
                position[0] = -0.1f;
                position[1] = -0.5f;
                position[2] = 0.0f;
            } else {
                const f32 world[3] = {target->x, target->y, target->z};
                ToScreen(world, position);
            }
            fn_710020bbe4(node->label->position, position);
            wchar_t text[0x20];
            UnkAIState_da92b0* state = target->state;
            swprintf(text, 0x20, L"%.2f/%.2f", static_cast<double>(state->field_0x54),
                     static_cast<double>(state->field_0x50));
            UnkLabel_da92b0* label = node->label;
            fn_71001c4418(label->text, reinterpret_cast<const s32*>(text));
            label->changed = true;
            node = node->next;
        }
    }

    UnkNodeList_da92b0<UnkFloatingLabel_da92b0>& floating = self->floating_labels;
    for (UnkFloatingLabel_da92b0* node = floating.head; node;) {
        node->time -= dt;
        bool running = !(node->time <= 0.0f);  // (a NaN time counts as running, as the original's b.ls)
        if (running && node->label) {
            node->position[1] += dt * 200.0f;
            f32 position[3];
            ToScreen(node->position, position);
            fn_710020bbe4(node->label->position, position);
            UnkTextStyle_da92b0* style = node->label->text->style;
            f32 alpha = running ? (node->time < 0.5f ? node->time : 0.5f) * 2.0f : 0.0f;
            const f32 colour[4] = {style->r, style->g, style->b, alpha};
            fn_710019db08(node->label->text->style, colour);
            node = node->next;
            continue;
        }
        UnkFloatingLabel_da92b0* next = node->next;
        if (!floating.Remove(node))
            break;
        FreeLabel(node);
        floating.pool->Free(node);
        node = next;
    }
}
