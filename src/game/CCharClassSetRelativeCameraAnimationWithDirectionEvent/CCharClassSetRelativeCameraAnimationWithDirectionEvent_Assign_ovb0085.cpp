// CCharClassSetRelativeCameraAnimationWithDirectionEvent's unit (tools/tu_map.py): the event's constructor
// (fn_7100bddff8: on the base event fn_7100bda678, the vtable and four empty file-path ids at +0x70..+0x88,
// one per direction) and its copy assignment (fn_7100bde2c4: the base's, fn_7100bdb724, then the four ids).
// Names and layouts are placeholders.
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "base/reflection/UnkReflAttributes.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;

// The base event's constructor, as
// src/game/CCharClassSetRelativeCameraAnimationBaseEvent/CCharClassSetRelativeCameraAnimationBaseEvent_b43.cpp
// defines it.
struct UnkSetRelativeCameraAnimationBaseEvent_b43a3;
void fn_7100bda678(UnkSetRelativeCameraAnimationBaseEvent_b43a3* self);
extern char g_unk_7101aa1c90[];

// A file-path id assigned as CStrId is (on the file-path pool), as src/game/CRumbleComponent.cpp's
// UnkRumbleParamsPath.
struct UnkDirectionPath_ovb0085 : CFilePathStrId {
    UnkDirectionPath_ovb0085& operator=(const UnkDirectionPath_ovb0085& other) {
        if (data != other.data) {
            UnkStringData* d = other.data;
            UnkStringPool* pool = g_unk_7101d47200;
            if (d != &pool->empty)
                ++d->ref_count;
            if (data != &pool->empty) {
                --data->ref_count;
                if (data->ref_count == 0)
                    fn_71000803a4(pool, data);
            }
            data = d;
        }
        return *this;
    }
};

struct UnkSetRelativeCameraAnimationWithDirectionEvent_ovb0085 {
    void* vtable;
    u8 unk_0x8[0x8];
    CFilePathStrId path_0x10;  // the base event's animation
    u8 unk_0x18[0x58];
    UnkDirectionPath_ovb0085 path_0x70;  // +x
    UnkDirectionPath_ovb0085 path_0x78;  // -x
    UnkDirectionPath_ovb0085 path_0x80;  // +y
    UnkDirectionPath_ovb0085 path_0x88;  // -y
};

void fn_7100bddff8(UnkSetRelativeCameraAnimationWithDirectionEvent_ovb0085* self) {
    fn_7100bda678(reinterpret_cast<UnkSetRelativeCameraAnimationBaseEvent_b43a3*>(self));
    self->vtable = g_unk_7101aa1c90 + 0x10;
    fn_710001a270(&self->path_0x70, refl_GlobalRntString(), true);
    fn_710001a270(&self->path_0x78, refl_GlobalRntString(), true);
    fn_710001a270(&self->path_0x80, refl_GlobalRntString(), true);
    fn_710001a270(&self->path_0x88, refl_GlobalRntString(), true);
}

void fn_7100bdb724(UnkSetRelativeCameraAnimationWithDirectionEvent_ovb0085* self,
                   const UnkSetRelativeCameraAnimationWithDirectionEvent_ovb0085* other);

UnkSetRelativeCameraAnimationWithDirectionEvent_ovb0085* fn_7100bde2c4(
    UnkSetRelativeCameraAnimationWithDirectionEvent_ovb0085* self,
    const UnkSetRelativeCameraAnimationWithDirectionEvent_ovb0085* other) {
    fn_7100bdb724(self, other);
    self->path_0x70 = other->path_0x70;
    self->path_0x78 = other->path_0x78;
    self->path_0x80 = other->path_0x80;
    self->path_0x88 = other->path_0x88;
    return self;
}
