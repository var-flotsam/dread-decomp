// Unit 235 (tools/tu_map.py): releases what an active holder took from its source: the pooled objects
// the source's first two lists hold (fn_7100161b90, fn_71001541a0), then the holder's own state
// (fn_71001ad4fc), and drops the reference it keeps at +0x18. Names and layouts are placeholders.
#include "base/UnkCountedResource.h"
#include "types.h"

struct UnkObj_71001541a0;

void fn_7100161b90(void* obj);
void fn_71001541a0(UnkObj_71001541a0* obj);

struct UnkPooledNode_b0231 {
    void* object;
    UnkPooledNode_b0231* next;
};

struct UnkPooledNode2_b0231 {
    UnkObj_71001541a0* object;
    UnkPooledNode2_b0231* next;
};

struct UnkPlainNode_b0231 {
    u8 pad_0x0[8];
    UnkPlainNode_b0231* next;
};

struct UnkHolderSource_b0231 {
    u8 pad_0x0[0x8];
    UnkPooledNode_b0231* first;  // 0x8
    UnkPooledNode2_b0231* second;  // 0x10
    u8 pad_0x18[8];
    UnkPlainNode_b0231* third;  // 0x20
};

struct UnkHolder_b0231 {
    UnkHolderSource_b0231* source;  // 0x0
    bool active;                    // 0x8
    u8 pad_0x9[0xf];
    UnkCountedResource* resource;   // 0x18
    u8 pad_0x20[8];
    u64 field_0x28;
    u64 field_0x30;
};

void fn_71001ad4fc(UnkHolder_b0231* self, UnkHolderSource_b0231* source, void* arg);

void fn_71001accdc(UnkHolder_b0231* self) {
    if (self->source && self->active) {
        self->field_0x28 = 0;
        self->field_0x30 = 0;
        for (UnkPooledNode_b0231* node = self->source->first; node;) {
            void* object = node->object;
            node = node->next;
            fn_7100161b90(*static_cast<void**>(object));
        }
        for (UnkPooledNode2_b0231* node = self->source->second; node;) {
            UnkObj_71001541a0* object = node->object;
            node = node->next;
            fn_71001541a0(*reinterpret_cast<UnkObj_71001541a0**>(object));
        }
        // walks the third list to its end (the original does, and uses nothing from it)
        UnkPlainNode_b0231* last = self->source->third;
        if (last) {
            do
                last = last->next;
            while (last);
        }
        fn_71001ad4fc(self, self->source, nullptr);
        self->active = false;
    }
    if (UnkCountedResource* resource = self->resource) {
        if (--resource->ref_count == 0)
            fn_710001737c(resource);
    }
}
