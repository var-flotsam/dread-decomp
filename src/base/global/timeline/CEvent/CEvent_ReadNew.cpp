// From unit 41 of tools/tu_map.py (base::global::timeline::CEvent): reads one timeline event from a
// file: its class's 64-bit id, the class looked up in the registry of reflected types and, when it is
// a CEvent with an in-place constructor, the object created (0x58 allocation flags), constructed and
// read by fn_710006790c. Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/animation/CBlendSpaceResource.h"
#include "base/global/CRntFile.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkTU83.h"
#include "reflected/base/global/timeline/CEvent.h"
#include "types.h"

// The registry of reflected types by 64-bit id (as src/base/global/UnkTimelineTracks.cpp declares it).
struct UnkVersionRegistry;
struct UnkVersionEntry;

struct UnkTimelineEvent_068f5c;

// CEvent's class: the static inside its getter CEvent::refl_GetClass.
extern char g_unk_7101d57b80[] __attribute__((visibility("hidden")));

// What a type's descriptor gives: the object's size and its in-place constructor.
struct UnkEventTypeView {
    u8 pad_0x0[0x18];
    u64 size;
    u8 pad_0x20[0x8];
    void (*construct)(void* object);
};

// Reads the rest of an event from the file.
void fn_710006790c(UnkTimelineEvent_068f5c* event, base::global::CRntFile* file);

UnkTimelineEvent_068f5c* CEvent_ReadNewFromFile(base::global::CRntFile* file) {
    u64 raw;
    CRntFile_Read(file, &raw, 8, 1);
    UnkVersionRegistry* registry = g_unk_7101d4c8d0;
    u64 id = raw;
    auto* type = reinterpret_cast<const base::reflection::CType*>(fn_71000a5118(registry, &id));
    base::global::timeline::CEvent::refl_GetClass();
    UnkTimelineEvent_068f5c* event = nullptr;
    if (fn_710009d50c(type, reinterpret_cast<const base::reflection::CType*>(g_unk_7101d57b80))) {
        const auto* view = reinterpret_cast<const UnkEventTypeView*>(type);
        if (view->construct) {
            void* p = GetDefaultAllocator()->vf_0x40(view->size, 8, 0x58);
            if (p)
                view->construct(p);
            event = static_cast<UnkTimelineEvent_068f5c*>(p);
        }
    }
    fn_710006790c(event, file);
    return event;
}
