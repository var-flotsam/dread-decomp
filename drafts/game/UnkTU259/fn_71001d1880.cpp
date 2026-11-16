// Unit 259 (the graphics system's unit): submits a run of model instances for drawing. Instances that
// share a model are grouped; each part of the model goes as one draw entry per instance into the
// list its material picks, or as one instanced batch. Names and layouts are placeholders.
#include "game/UnkTU259.h"
#include "types.h"

struct UnkGfxSubmit;
void fn_71001c4720(UnkGfxSubmit* self);

struct UnkMaterial_l1d1880 {
    u8 pad_0x0[0x18];
    u32 kind;  // 0x18
    u32 type;  // 0x1c
    u8 pad_0x20[0x10];
    u8 field_0x30;
};

struct UnkModelPart_l1d1880 {
    u8 pad_0x0[0x8];
    UnkMaterial_l1d1880* material;  // 0x8
    u8 pad_0x10[0x10];
};

struct UnkModel_l1d1880 {
    u8 pad_0x0[0x34];
    u32 part_count;               // 0x34
    UnkModelPart_l1d1880* parts;  // 0x38
};

struct UnkModelResource_l1d1880 {
    u8 pad_0x0[0x78];
    UnkModel_l1d1880* model;  // 0x78
};

struct UnkInstance_l1d1880 {
    UnkModelResource_l1d1880* resource;  // 0x0
    u8 data[0xd4];                       // 0x8
    u32 id;                              // 0xdc
};

struct UnkDrawEntry_l1d1880 {
    u32 field_0x0;
    u8 pad_0x4[0x4];
    UnkModel_l1d1880* model;  // 0x8
    u32 kind;                 // 0x10
    u8 pad_0x14[0x4];
    UnkModelPart_l1d1880* part;      // 0x18
    UnkMaterial_l1d1880* material;   // 0x20
    s32 field_0x28;
    u8 pad_0x2c[0x4];
    void* data;  // 0x30
    u32 id;      // 0x38
    u8 pad_0x3c[0x4];
    void (*callback)(UnkGfxSubmit* self);  // 0x40
    bool field_0x48;
    u8 pad_0x49[0x7];
};

struct UnkDrawEntryList_l1d1880 {
    UnkDrawEntry_l1d1880* entries[0x2000];
    u32 count;
};

struct UnkDrawBatch_l1d1880 {
    UnkModel_l1d1880* model;     // 0x0
    UnkModelPart_l1d1880* part;  // 0x8
    void* data[0x100];           // 0x10
    u32 ids[0x100];              // 0x810
    u32 count;                   // 0xc10
};

struct UnkGfx_l1d1880 {
    u8 pad_0x0[0x128];
    UnkDrawEntry_l1d1880 entries[0x2000];  // 0x128
    u32 entry_count;                       // 0xa0128
    u8 pad_0xa012c[0xb0138 - 0xa012c];
    UnkDrawEntryList_l1d1880 opaque;       // 0xb0138
    UnkDrawEntryList_l1d1880 field_0xc0140;
    UnkDrawEntryList_l1d1880 special;      // 0xd0148
    u8 pad_0xe0150[0x110164 - 0xe0150];
    u32 group_sizes[0x2000];  // 0x110164
    u8 pad_0x118164[0x4];
    UnkDrawBatch_l1d1880 batches[0x2000];  // 0x118168
    u32 batch_count;                       // 0x1948168
};

static_assert(__builtin_offsetof(UnkGfx_l1d1880, special) == 0xd0148, "");
static_assert(__builtin_offsetof(UnkGfx_l1d1880, group_sizes) == 0x110164, "");
static_assert(__builtin_offsetof(UnkGfx_l1d1880, batch_count) == 0x1948168, "");

inline void QueueEntry(UnkGfx_l1d1880* self, UnkDrawEntryList_l1d1880* list, UnkModel_l1d1880* model,
                       UnkModelPart_l1d1880* part, UnkMaterial_l1d1880* material,
                       UnkInstance_l1d1880* instance) {
    UnkDrawEntry_l1d1880* entry = &self->entries[self->entry_count++];
    entry->field_0x0 = 0;
    entry->model = model;
    entry->kind = material->kind;
    entry->part = part;
    entry->material = material;
    entry->field_0x28 = -1;
    entry->data = instance->data;
    entry->id = instance->id;
    entry->callback = fn_71001c4720;
    entry->field_0x48 = true;
    list->entries[list->count++] = entry;
}

void fn_71001d1880(UnkRenderTargetA1* target, void* p, u32 n, u32 a) {
    UnkGfx_l1d1880* self = reinterpret_cast<UnkGfx_l1d1880*>(target);
    UnkInstance_l1d1880** instances = static_cast<UnkInstance_l1d1880**>(p);
    if (n == 0)
        return;
    u32 count = n < 0x2000 ? n : 0x2000;
    u32 group_count = 0;
    UnkModelResource_l1d1880* previous = nullptr;
    for (s32 i = 0; i < count; i++) {
        UnkModelResource_l1d1880* resource = instances[i]->resource;
        if (resource == previous) {
            self->group_sizes[group_count - 1]++;
        } else {
            self->group_sizes[group_count++] = 1;
            previous = resource;
        }
    }
    s32 start = 0;
    for (u32 group = 0; group < group_count; group++) {
        UnkModel_l1d1880* model = instances[start]->resource->model;
        u32* size = &self->group_sizes[group];
        u32 part_count = model->part_count;
        for (u32 j = 0; j < part_count; j++) {
            UnkModelPart_l1d1880* part = &model->parts[j];
            UnkMaterial_l1d1880* material = part->material;
            if (material->type - 0x37 <= 5 && material->kind == 1 && material->field_0x30) {
                for (u32 k = 0; k < *size; k++)
                    QueueEntry(self, &self->special, model, part, material, instances[start + k]);
            } else if (material->kind == 1 || material->kind == 0x10) {
                UnkDrawBatch_l1d1880* batch = &self->batches[self->batch_count++];
                u32 batch_size = *size < 0x100 ? *size : 0x100;
                batch->count = batch_size;
                batch->model = model;
                batch->part = part;
                for (s32 k = 0; k < batch_size; k++) {
                    UnkInstance_l1d1880* instance = instances[start + k];
                    batch->data[k] = instance->data;
                    batch->ids[k] = instance->id;
                }
            } else {
                for (u32 k = 0; k < *size; k++)
                    QueueEntry(self, &self->opaque, model, part, material, instances[start + k]);
            }
        }
        start += *size;
    }
}
