// CModelUpdaterComponent unit (tools/tu_map.py): the model updater's property handler. It reads the
// layer, a hashed name, a stream seek, a random part shown among numbered ones, a visibility flag, two
// model flags, a bounding sphere and a flag at +0x158; unknown keys go to the base handler
// (fn_71010c4250). Names and layouts are placeholders.
#include <stdio.h>

#include "../../game_s7/UnkPropS7.h"
#include "base/UnkCloneHelpers.h"
#include "base/UnkHashStream.h"
#include "base/global/CRntFile.h"
#include "base/global/CStrId.h"
#include "base/global/timeline/CEvent.h"
#include "game/CSceneComponent.h"
#include "game/UnkComponent.h"
#include "game/UnkSphere.h"
#include "game/UnkTU218.h"
#include "game/UnkTU308.h"
#include "game/UnkTU56.h"
#include "types.h"

using namespace unk_prop;
using base::global::CRntFile;

extern "C" int rand();
void fn_710020dd0c(void* shape, const UnkSphere* sphere);

struct UnkModelSlots_l171fcc {
    u8 pad_0x0[0x78];
    void* parts;  // 0x78
    u8 pad_0x80[0x2dc];
    bool field_0x35c;
    bool field_0x35d;
};

struct UnkModel_l171fcc {
    u8 pad_0x0[0x20];
    u8 shape[0x118];  // 0x20
    UnkModelSlots_l171fcc* slots;  // 0x138
};

struct UnkModelUpdater_l171fcc {
    u8 pad_0x0[0x40];
    s32 layer;  // 0x40
    u8 pad_0x44[0xc];
    UnkModel_l171fcc* model;  // 0x50
    u8 pad_0x58[0xa0];
    u64 name_hash;  // 0xf8
    u8 pad_0x100[0x58];
    bool field_0x158;
};

// Shows or hides the model's part named `name`.
static inline void ShowPart(UnkModelUpdater_l171fcc* self, const char* name, bool show) {
    u64 hash;
    fn_710006663c(&hash, name);
    if (self->model)
        fn_7100199420(self->model->slots->parts, &hash, show);
}

u32 fn_7101171fcc(UnkModelUpdater_l171fcc* self, UnkProp* p) {
    if (Is(p, Key(0x2018))) {
        s32 layer = -1;
        CRntFile_Read(reinterpret_cast<CRntFile*>(p->stream), &layer, 4, 1);
        self->layer = layer;
        return true;
    }
    if (Is(p, Key(0x2000))) {
        const char* name;
        CRntFile_ReadCString(p->stream, &name);
        self->name_hash = fn_7100001570(name, __builtin_strlen(name));
        return true;
    }
    if (Is(p, Key(0x1668))) {
        auto* stream = reinterpret_cast<UnkHashStream_0x38*>(p->stream);
        CRntFile_Seek(reinterpret_cast<CRntFile*>(p->stream), fn_710007c1a4(stream), 0);
        return self->model != nullptr;
    }
    if (Is(p, Key(0x20c0))) {
        // Shows one of the parts named <prefix><first> to <prefix><last>, picked at random.
        s32 first;
        s32 last;
        const char* prefix;
        CRntFile_ReadCString(p->stream, &prefix);
        CRntFile_Read(reinterpret_cast<CRntFile*>(p->stream), &first, 4, 1);
        CRntFile_Read(reinterpret_cast<CRntFile*>(p->stream), &last, 4, 1);
        s32 r = rand();
        if (last >= first) {
            s32 pick = r % (last - first + 1);
            // `pick` counts down to the chosen part.
            for (s32 i = first; i <= last; ++i, --pick) {
                char name[0x40];
                sprintf(name, "%s%i", prefix, i);
                if (pick == 0)
                    ShowPart(self, name, true);
                else
                    ShowPart(self, name, false);
            }
        }
        return true;
    }
    if (Is(p, Key(0x2060))) {
        bool visible;
        CRntFile_ReadFormat(p->stream, "b", &visible);
        fn_71009f19b0(self, visible);
        return true;
    }
    if (Is(p, Key(0x2030))) {
        bool a;
        bool b;
        CRntFile_ReadFormat(p->stream, "bb", &a, &b);
        if (!self->model)
            return false;
        self->model->slots->field_0x35c = a;
        self->model->slots->field_0x35d = b;
        return true;
    }
    if (Is(p, Key(0x1fb0)) && p->set) {
        UnkVec3f center(0.0f, 0.0f, 0.0f);
        f32 radius = -1.0f;
        CRntFile_ReadFormat(p->stream, "ffff", &radius, &center.x, &center.y, &center.z);
        if (radius >= 0.0f) {
            UnkSphere sphere;
            fn_710021685c(&sphere);
            fn_7100216978(&sphere, &center);
            fn_710021693c(&sphere, radius);
            fn_710020dd0c(self->model->shape, &sphere);
            fn_7100216934(reinterpret_cast<char*>(&sphere));
        }
        return true;
    }
    if (Is(p, Key(0x2040)) && p->set) {
        CRntFile_ReadFormat(p->stream, "b", &self->field_0x158);
        return true;
    }
    // The base handler takes the component and the property record.
    return fn_71010c4250(reinterpret_cast<UnkBaseComponent*>(self), reinterpret_cast<UnkComponentProp*>(p));
}
