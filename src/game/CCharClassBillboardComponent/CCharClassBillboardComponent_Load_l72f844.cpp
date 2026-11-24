// CCharClassBillboardComponent's unit (tools/tu_map.py unit 1154): a load step. After the base step, reserves
// both resource arrays for the billboard count and, for each billboard, resolves the first path (and the
// second, when it is set) into the first array. Names and layouts are placeholders.
#include "base/UnkNoArg.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "types.h"

using base::global::CFilePathStrId;

struct UnkBillboardResource_l72f844;

// Resolves `path` into `out` (the same unit; it does not read its first argument).
void fn_710072fb50(UnkNoArg, UnkBillboardResource_l72f844** out, const CFilePathStrId* path);

struct UnkBillboardComponent_l72f844 {
    u8 pad_0x0[0x58];
    u32 count;  // 0x58
    u8 pad_0x5c[0xc];
    CFilePathStrId path;         // 0x68
    CFilePathStrId second_path;  // 0x70
    u8 pad_0x78[0x68];
    base::global::CRntVector<UnkBillboardResource_l72f844*> resources;        // 0xe0
    base::global::CRntVector<UnkBillboardResource_l72f844*> second_resources;  // 0x108
};
static_assert(__builtin_offsetof(UnkBillboardComponent_l72f844, path) == 0x68, "layout");
static_assert(__builtin_offsetof(UnkBillboardComponent_l72f844, resources) == 0xe0, "layout");
static_assert(__builtin_offsetof(UnkBillboardComponent_l72f844, second_resources) == 0x108, "layout");

// `arg` (x1) reaches the base step untouched.
bool CCharClassBillboardComponent_FillPoolFromPaths(UnkBillboardComponent_l72f844* self, void* arg) {
    bool r = fn_71011ace94(reinterpret_cast<UnkModelEntity*>(self), arg);
    if (self->count != 0) {
        self->resources.Reserve(self->count);
        self->second_resources.Reserve(self->count);
        for (u32 i = 0; i < self->count; ++i) {
            UnkBillboardResource_l72f844* resource = nullptr;
            fn_710072fb50(UnkNoArg(), &resource, &self->path);
            self->resources.PushBack(resource);
            if (self->second_path.data != &g_unk_7101d47200->empty) {
                fn_710072fb50(UnkNoArg(), &resource, &self->second_path);
                self->resources.PushBack(resource);
            }
        }
    }
    return r;
}
