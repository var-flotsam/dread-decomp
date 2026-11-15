// Unit 2744 (gui/scripts/guimanager.lua): creates a GUI object by its class name ("GUI::" + name,
// looked up among the reflected classes), names it, attaches it to its parent and, when it needs
// updating, adds it to the list of objects to update. Names and layouts are placeholders.
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntString.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkGuiDisplayObject.h"
#include "game/UnkTU2747.h"
#include "game/UnkTU57.h"
#include "game/UnkTU83.h"
#include "types.h"

using base::global::CRntString;
using base::global::CStrId;
using base::reflection::CType;
using base::reflection::UnkReflObject;

// What a class descriptor gives: the object's size and its in-place constructor.
struct UnkGuiTypeView {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x8];
    void (*construct)(void* object);
};

// The created object, as far as this function uses it.
struct UnkGuiCreatedObject {
    u8 pad_0x0[0x130];
    CStrId name;
    u8 pad_0x138[0x4c];
    bool field_0x184;
    u8 pad_0x185[2];
    bool queued;  // 0x187: in the update list
    bool field_0x188;
};
static_assert(__builtin_offsetof(UnkGuiCreatedObject, field_0x188) == 0x188, "UnkGuiCreatedObject placeholder layout");

struct UnkGuiObjectFactory {
    u8 pad_0x0[0xa0];
    base::global::CRntVector<UnkGuiCreatedObject*> queue;
};

// Adds the object to the factory's update list unless it is in it or needs no update.
static void Enqueue(UnkGuiObjectFactory* self, UnkGuiCreatedObject* object) {
    UnkGuiDisplayObject* display = reinterpret_cast<UnkGuiDisplayObject*>(object);
    if (object->queued)
        return;
    if (object->field_0x188 || object->field_0x184 ||
        (fn_71011cde14(display)->field_0x3c && (fn_71011ceac0(display) || fn_71011ceb34(display)))) {
        self->queue.PushBack(object);
        object->queued = true;
    }
}
static_assert(__builtin_offsetof(UnkGuiObjectFactory, queue) == 0xa0, "UnkGuiObjectFactory placeholder layout");

UnkGuiCreatedObject* fn_71011b5aa4(UnkGuiObjectFactory* self, const CStrId* name, const CRntString* class_name,
                                   UnkGuiDisplayObject* parent) {
    CRntString full_name = fn_710007f050("GUI::", class_name);
    UnkVersionRegistry* registry = g_unk_7101d4c8d0;
    u64 id;
    fn_7100066630(&id, &full_name);
    UnkReflObject* found = reinterpret_cast<UnkReflObject*>(fn_71000a5118(registry, &id));
    UnkGuiCreatedObject* object = nullptr;
    if (found && fn_710009d50c(found->vf_0x0(), fn_7100096100())) {
        const CType* type = reinterpret_cast<const CType*>(found);
        const UnkGuiTypeView* view = reinterpret_cast<const UnkGuiTypeView*>(found);
        if (fn_710009d50c(type, fn_71011c3868()) && view->construct) {
            void* p = fn_7100083990()->vf_0x40(view->size, 8, 0xc);
            if (p) {
                view->construct(p);
                object = static_cast<UnkGuiCreatedObject*>(p);
                object->name = *name;
                if (parent)
                    fn_71011cd424(reinterpret_cast<UnkGuiDisplayObject*>(object), parent);
            }
        }
        Enqueue(self, object);
    }
    return object;
}
