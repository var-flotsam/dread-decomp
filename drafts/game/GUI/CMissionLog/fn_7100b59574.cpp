// GUI::CMissionLog: creates the next entry for a request (unless it is the ignored kind), sends it
// a GUI event and sizes the entry list for one more entry. Names and layouts are placeholders.
#include <new>

#include "base/global/CStrId.h"
#include "base/math/CVector.h"
#include "game/UnkGuiEvent.h"
#include "types.h"

using base::global::CStrId;

struct UnkMissionLogRequest {
    s32 kind;
    CStrId id;
};

struct UnkMissionLogList {
    u8 pad_0x0[0x1c8];
    s32 count;  // 0x1c8
};

struct UnkMissionLogEntry {
    u8 pad_0x0[0x8c];
    f32 width;   // 0x8c
    f32 height;  // 0x90
};

struct UnkMissionLogSelf {
    u8 pad_0x0[0x238];
    UnkMissionLogList* list;  // 0x238
};

UnkMissionLogEntry* fn_71011c08e4(UnkMissionLogList* list, CStrId* id, s32 mode, bool flag);
struct UnkSprite_spr;
void fn_71011cf8d0(UnkSprite_spr* self, const base::math::CVector2D* size);

void fn_7100b59574(UnkMissionLogSelf* self, UnkMissionLogRequest* request, bool flag) {
    if (request->kind == 2)
        return;
    UnkMissionLogList* list = self->list;
    s32 count = list->count;
    union IdStorage {
        CStrId id;
        IdStorage() {}
        ~IdStorage() {}
    } storage;
    new (&storage.id) CStrId(request->id);
    UnkMissionLogEntry* entry = fn_71011c08e4(list, &storage.id, 2, flag);
    storage.id.~CStrId();
    UnkGuiEvent event;
    s32 next = count + 1;
    {
        s32 kind = 0x2b;
        fn_710120235c(&event, reinterpret_cast<UnkGuiDisplayObject*>(self), &kind, nullptr);
    }
    (*reinterpret_cast<void (***)(UnkMissionLogEntry*, UnkGuiEvent*)>(entry))[0xd8 / 8](entry, &event);
    base::math::CVector2D size;
    f32 height = entry->height;
    UnkSprite_spr* sprite = reinterpret_cast<UnkSprite_spr*>(self->list);
    height *= static_cast<f32>(next);
    size.x = entry->width;
    size.y = height;
    fn_71011cf8d0(sprite, &size);
    fn_7101202488(&event);
}
