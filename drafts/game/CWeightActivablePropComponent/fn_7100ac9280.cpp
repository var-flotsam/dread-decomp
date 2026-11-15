// CWeightActivablePropComponent's unit: initialisation. Runs the base method, keeps the object at +0x10 when
// it is a CCharClassWeightActivablePropComponent, wires two callbacks and registers its two listeners in the
// list at +0x150 unless one with the same id is there already. Names and layouts are placeholders.
#include "base/reflection/UnkClassGetters.h"
#include "base/reflection/UnkReflCast.h"
#include "game/UnkComponent.h"
#include "types.h"

using base::reflection::UnkReflObject;

char* fn_7100acb144(char* p0);
char* fn_7100acb148(char* p0);

struct UnkWeightListener_lac9280 {
    void* field_0x0;
    void* field_0x8;
    u32 id;
};

struct __attribute__((packed)) UnkWeightListenerList_lac9280 {
    u32 count;
    UnkWeightListener_lac9280* items[16];
};

struct UnkWeightActivable_lac9280 {
    u8 pad_0x0[0x10];
    UnkReflObject* object;
    u8 pad_0x18[0x30];
    UnkReflObject* field_0x48;
    UnkWeightListener_lac9280 listener_a;  // 0x50
    u8 pad_0x6c[0x80 - 0x6c];
    void* field_0x80;
    char* (*field_0x88)(char*);
    u8 pad_0x90[0xc0 - 0x90];
    UnkWeightListener_lac9280 listener_b;  // 0xc0
    u8 pad_0xdc[0xf0 - 0xdc];
    void* field_0xf0;
    char* (*field_0xf8)(char*);
    u8 pad_0x100[0x130 - 0x100];
    u8 list[8];  // 0x130
    void* field_0x138;
    u8 pad_0x140[0x150 - 0x140];
    UnkWeightListenerList_lac9280 listeners;  // 0x150
};
static_assert(__builtin_offsetof(UnkWeightActivable_lac9280, listener_a.id) == 0x60, "id a");
static_assert(__builtin_offsetof(UnkWeightActivable_lac9280, listener_b.id) == 0xd0, "id b");
static_assert(__builtin_offsetof(UnkWeightActivable_lac9280, listeners) == 0x150, "listeners");

// Adds the listener unless one with its id is in the list; the new one points at the owner's list.
static inline void AddListener(UnkWeightActivable_lac9280* self, UnkWeightListener_lac9280* listener) {
    UnkWeightListenerList_lac9280& list = self->listeners;
    u32 count = list.count;
    UnkWeightListener_lac9280** items = list.items;
    for (UnkWeightListener_lac9280** it = items; it != items + count; ++it) {
        if ((*it)->id == listener->id)
            return;
    }
    listener->field_0x8 = self->list;
    items[count] = listener;
    list.count = count + 1;
}

bool fn_7100ac9280(UnkWeightActivable_lac9280* self, void* arg) {
    fn_71010c41bc(reinterpret_cast<UnkBaseComponent*>(self), arg);
    UnkReflObject* object = self->object;
    self->field_0x48 = object && fn_710009d50c(object->vf_0x0(), fn_7100ac88e0()) ? object : nullptr;
    self->field_0x138 = self;
    self->field_0xf0 = self;
    self->field_0xf8 = fn_7100acb148;
    self->field_0x80 = self;
    self->field_0x88 = fn_7100acb144;
    AddListener(self, &self->listener_a);
    AddListener(self, &self->listener_b);
    return true;
}
