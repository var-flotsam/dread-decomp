// The music manager's constructor (include/game/sound/CMusicManager.h): sets its vtable, sizes the
// request, slot and activity arrays from the sound manager's slot count, builds four chained maps
// and the channel, bank and track sub-objects, then gives every slot a list and a 0x10-byte data
// object and clears its activity flag. Also a request's flag setter (fn_7100fd63b0).
#include <new>

#define UNK_VECTOR_GROW_AT_ATTR __attribute__((always_inline))

#include "game/sound/CMusicManager.h"

struct UnkSoundMgr;
extern UnkSoundMgr* g_unk_7101d4d820;
extern char g_unk_7101cac1e0[];  // the manager's vtable
extern char g_unk_7101cac2d0[];  // the slot data object's vtable
void fn_7100fd37f0();            // stored in the slot data object

u32 fn_7100108b7c(UnkSoundMgr* mgr, u32 flag);
void fn_7100fcee0c(UnkMusicTrack_0x38* p, u32 kind);
void fn_7101007bbc(UnkMusicTrack_0x38* p, u32 kind);
void fn_7101007cbc(UnkMusicTrack_0x32* p, u32 kind);

// NON_MATCHING(minor): same behaviour; in the slot loop the original keeps the slot array's address
// in a register and hoists the data function's address out of the loop, where this spills the former
// and reloads the latter (the code up to the loop matches).
void fn_7100fd3804(void* block) {
    UnkMusicManager* self = static_cast<UnkMusicManager*>(block);
    self->field_0x20 = 0;
    self->field_0x10 = 0;
    self->field_0x18 = 0;
    self->vtable = g_unk_7101cac1e0 + 0x10;
    self->field_0x8 = 0;
    self->field_0xa4 = 0x7fffffff;
    self->field_0xa8 = 0;
    u32 n = fn_7100108b7c(g_unk_7101d4d820, 1) >> 1;
    new (&self->entries) base::UnkVector<UnkMusicEntry_0x60>(0xb);
    fn_7100fddcb4(&self->entries, n);
    n = fn_7100108b7c(g_unk_7101d4d820, 1) >> 1;
    new (&self->slots) base::UnkVector<UnkMusicSlot_0x38>(0xb);
    fn_7100fde040(&self->slots, n);
    n = fn_7100108b7c(g_unk_7101d4d820, 1) >> 1;
    new (&self->slot_active) base::UnkVector<u8>(0xb);
    self->slot_active.Reserve(n);
    new (&self->map_0) UnkChainedMap();
    self->map_0.ResizeBucketsPtrFirst(11);
    new (&self->map_1) UnkChainedMap();
    self->map_1.ResizeBucketsPtrFirst(11);
    new (&self->map_2) UnkChainedMap();
    self->map_2.ResizeBucketsPtrFirst(11);
    new (&self->map_3) UnkChainedMap();
    self->map_3.ResizeBucketsPtrFirst(11);
    self->slot_count = fn_7100108b7c(g_unk_7101d4d820, 1) >> 1;
    fn_7100fe1028(&self->channel_1, 1);
    fn_7100fe1028(&self->channel_2, 2);
    fn_7100fcfcdc(&self->bank_3, 3);
    fn_7100fcee0c(&self->track_4, 4);
    fn_7100fcee0c(&self->track_5, 5);
    fn_7101007bbc(&self->track_6, 6);
    fn_7101007cbc(&self->track_7, 7);
    self->field_0xf0a = 0;
    self->field_0xf10 = 0;
    for (u64 i = 0; i < self->slot_count; ++i) {
        UnkMusicSlot_0x38* slot;
        if (self->slots.field_0x14 && self->slots.count <= i) {
            u64 cap = self->slots.capacity;
            if (cap <= i && self->slots.step != 0)
                cap = ((u32)i / self->slots.step + 1) * self->slots.step;
            fn_7100fde040(&self->slots, (u32)cap);
            u64 end = i + 1;
            fn_7100fde040(&self->slots, (u32)end);
            u64 k = self->slots.count;
            if (k <= i) {
                UnkMusicSlot_0x38* e = &self->slots.data[k];
                do {
                    ++k;
                    new (e++) UnkMusicSlot_0x38();
                } while (end != (k & 0xffffffff));
            }
            self->slots.count = (u32)end;
        }
        slot = &self->slots.data[i];
        UnkMusicSlotData_0x10* d = static_cast<UnkMusicSlotData_0x10*>(fn_71000839a0(0x10, 0xb));
        d->vtable = g_unk_7101cac2d0 + 0x10;
        d->field_0x8 = fn_7100fd37f0;
        slot->field_0x30 = d;
        base::UnkVector<u8>& active = self->slot_active;
        if (active.field_0x14 && active.count <= i) {
            u64 cap = active.capacity;
            if (cap <= i && active.step != 0) {
                u32 grown = ((u32)i / active.step + 1) * active.step;
                if (active.capacity < grown) {
                    cap = grown;
                    active.Reserve(grown);
                }
            }
            u32 end = (u32)(i + 1);
            u8* p;
            if (i < cap) {
                p = active.data;
            } else {
                p = static_cast<u8*>(active.allocator->vf_0x40(i + 1, 8, active.flags));
                if (active.data) {
                    __builtin_memcpy(p, active.data, active.count);
                    active.allocator->vf_0x48(active.data);
                }
                active.data = p;
                active.capacity = end;
            }
            __builtin_memset(p + active.count, 0, end - active.count);
            active.count = end;
        }
        active.data[i] = 0;
    }
}

// An entry of the request array (+0xb8) as the flag setter below reads it.
struct UnkMusicEntryView {
    u8 pad_0x0[0xc];
    f32 field_0xc;  // passed to the sound manager
    u8 pad_0x10[0x14];
    bool flag;  // 0x24
    u8 pad_0x25[0x3];
    u64 handle;  // 0x28
    s32 active;  // 0x30
    u8 pad_0x34[0x2c];
};
static_assert(sizeof(UnkMusicEntryView) == 0x60, "UnkMusicEntryView layout");

void fn_7100108a98(UnkSoundMgr* mgr, u64 handle, bool flag, f32 value);

// Sets the flag of request `index` when it is active and the flag changes, and tells the sound
// manager; returns whether it did.
bool fn_7100fd63b0(UnkMusicManager* self, u32 index, bool flag) {
    if (index < self->entries.count) {
        UnkMusicEntryView* entry = reinterpret_cast<UnkMusicEntryView*>(self->entries.data) + index;
        if (entry->active != 0 && entry->flag != flag) {
            u64 handle = entry->handle;
            entry->flag = flag;
            fn_7100108a98(g_unk_7101d4d820, handle, flag, entry->field_0xc);
            return true;
        }
    }
    return false;
}
