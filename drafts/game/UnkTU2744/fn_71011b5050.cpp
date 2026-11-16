// Unit 2744 (gui/scripts/guimanager.lua): moves the GUI root's focus to the next object of its focus ring
// that can take it, after the focused one and wrapping round to the head. Names and layouts are
// placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkGuiRoot.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

bool fn_71011cffe4(UnkGuiDisplayObject* self, UnkGuiDisplayObject* container);

// A node of the focus ring: the weak handle of an object and the next node.
struct UnkGuiFocusNode_b5050 {
    UnkWeakHandle* handle;
    UnkGuiFocusNode_b5050* next;
};

// The root's focus ring as this function reads it: the count and the head.
struct UnkGuiRootFocus_b5050 {
    u8 pad_0x0[0x1c0];
    u32 count;                    // 0x1c0
    UnkGuiFocusNode_b5050* head;  // 0x1c8
};
static_assert(__builtin_offsetof(UnkGuiRootFocus_b5050, head) == 0x1c8, "UnkGuiRootFocus_b5050 layout");

// A display object's value at +0x138: negative keeps it out of the ring.
struct UnkGuiFocusable_b5050 {
    u8 pad_0x0[0x138];
    s32 field_0x138;
};

static inline UnkGuiDisplayObject* Target(UnkWeakHandle* handle) {
    return handle ? reinterpret_cast<UnkGuiDisplayObject*>(handle->target) : nullptr;
}

// The node of `object`, or null.
static inline UnkGuiFocusNode_b5050* Find(UnkGuiFocusNode_b5050* head, UnkGuiDisplayObject* object) {
    for (UnkGuiFocusNode_b5050* node = head; node; node = node->next)
        if (Target(node->handle) == object)
            return node;
    return nullptr;
}

static inline bool CanFocus(UnkGuiRoot* root, UnkGuiFocusNode_b5050* node) {
    UnkGuiDisplayObject* object = Target(node->handle);
    if (!object)
        return false;
    if (!fn_71011cde14(object)->field_0x3e || !object->field_0x181)
        return false;
    UnkGuiDisplayObject* other = Target(root->field_0x238);
    if (other && other != object) {
        UnkGuiDisplayObject* container = fn_710009d50c(other->GetClass(), fn_71011d7b84()) ? other : nullptr;
        if (!fn_71011cffe4(object, container))
            return false;
    }
    return reinterpret_cast<UnkGuiFocusable_b5050*>(node->handle->target)->field_0x138 >= 0;
}

void fn_71011b5050(UnkGuiRoot* root) {
    UnkGuiRootFocus_b5050* self = reinterpret_cast<UnkGuiRootFocus_b5050*>(root);
    if (!self->count)
        return;
    UnkGuiFocusNode_b5050* found = nullptr;
    UnkGuiFocusNode_b5050* it;
    if (UnkGuiDisplayObject* focused = Target(root->field_0x230)) {
        found = Find(self->head, focused);
        it = found->next ? found->next : self->head;
    } else {
        it = self->head;
    }
    UnkGuiDisplayObject* target = nullptr;
    do {
        if (CanFocus(root, it)) {
            target = Target(it->handle);
        } else {
            // Wraps round to the head, unless the walk is back where it started.
            it = it->next;
            if (it == found)
                continue;
            if (!it)
                it = self->head;
        }
    } while (!target && it != found);
    if (target)
        fn_71011b6bbc(root, target, false);
}
