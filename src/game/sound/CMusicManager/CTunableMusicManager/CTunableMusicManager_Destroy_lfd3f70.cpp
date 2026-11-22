// sound::CMusicManager::CTunableMusicManager's unit: the music manager's destructor (constructor
// fn_7100fd3804): its sub-objects and maps last to first, then its three arrays, then the base.
// Names and layouts are placeholders.
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "base/global/UnkPooledId.h"
#include "base/reflection.h"
#include "game/UnkBase_0x30_7100fe1050.h"
#include "game/sound/CMusicManager.h"
#include "game/sound/CMusicManager/CTunableMusicManager.h"
#include "game/sound/UnkMusicPathVec.h"
#include "types.h"

struct UnkObject_7100fcfcdc;
struct UnkOwnedC_ld8d45c;
extern char g_unk_7101cac290[];  // its base's vtable

void fn_7101007cec(UnkMusicTrack_0x32* track);
void fn_7101007bf0(UnkMusicTrack_0x38* track);
void fn_7100fcee44(UnkMusicTrack_0x38* track);
void fn_7100fd0140(UnkObject_7100fcfcdc& self);
void fn_7100fd422c(char* map);
void fn_7100fd431c(char* map);

// An entry of the request array: a list of file paths and a name.
struct UnkMusicEntry_0x60 {
    u8 pad_0x0[0x28];
    UnkMusicPathList paths;                          // 0x28
    u8 pad_0x50[0x8];
    UnkPooledId<&g_unk_7101d47108> name;                // 0x58
};
static_assert(sizeof(UnkMusicEntry_0x60) == 0x60, "UnkMusicEntry_0x60 layout");

// A slot's node list is destroyed through its vtable (slot 0).
static inline void DestroySlot(UnkMusicSlot_0x38& slot) {
    reinterpret_cast<void (**)(UnkMusicSlot_0x38*)>(slot.vtable)[0](&slot);
}

void fn_7100fd3f70(UnkMusicManager* self) {
    self->vtable = g_unk_7101cac1e0 + 0x10;
    fn_7101007cec(&self->track_7);
    fn_7101007bf0(&self->track_6);
    fn_7100fcee44(&self->track_5);
    fn_7100fcee44(&self->track_4);
    fn_7100fd0140(reinterpret_cast<UnkObject_7100fcfcdc&>(self->bank_3));
    reinterpret_cast<UnkBase_0x30_7100fe1050*>(&self->channel_2)->~UnkBase_0x30_7100fe1050();
    reinterpret_cast<UnkBase_0x30_7100fe1050*>(&self->channel_1)->~UnkBase_0x30_7100fe1050();
    fn_7100d9012c(&self->map_3);
    fn_7100d90218(reinterpret_cast<UnkOwnedC_ld8d45c*>(&self->map_2));
    fn_7100fd422c(reinterpret_cast<char*>(&self->map_1));
    fn_7100fd431c(reinterpret_cast<char*>(&self->map_0));
    self->slot_active.Free();
    if (self->slots.data) {
        for (u64 i = 0; i < self->slots.count; ++i)
            DestroySlot(self->slots.data[i]);
        self->slots.count = 0;
        self->slots.allocator->vf_0x48(self->slots.data);
        self->slots.capacity = 0;
    }
    self->entries.Free();
    self->vtable = g_unk_7101cac290 + 0x10;
    self->field_0x20 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
}
