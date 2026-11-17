// Unit 1449 of tools/tu_map.py (no name hint): the constructor of an event source with a state record that
// holds three states (ids 1 to 3), each with up to four callbacks, and a weak-link listener.
// Names and layouts are placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/CAbilityGhostAura.h"
#include "game/UnkTU1449.h"
#include "types.h"

extern void* g_unk_7101a85d00[];

u32 fn_710097eb88(char* p0, char* p1);
char* fn_710097eba0(char* p0);
char* fn_710097eba4(char* p0);
char* fn_710097ec3c(char* p0, char* p1);
char* fn_710097ec40(char* p0);
char* fn_710097ec44(char* p0, u32 p1);
void fn_710097ece0();

struct UnkCallback_l97bcb8 {
    void* object;
    void* function;

    void Init() {
        object = nullptr;
        function = nullptr;
    }
    void Bind(void* o, void* f) {
        object = o;
        function = f;
    }
};

struct UnkStateRecord_l97bcb8;

struct UnkEventBase_l97bcb8 {
    void** vtable;
    UnkStateRecord_l97bcb8* owner;
    u32 id;
    base::reflection::UnkReflValue value;

    void InitBase(u32 base_id) {
        owner = nullptr;
        id = base_id;
        value.field_0x0 = nullptr;
        value.type = nullptr;
        value.kind = 2;
        value.field_0x14 = 1;
    }
};

struct UnkState_l97bcb8 : UnkEventBase_l97bcb8 {
    UnkCallback_l97bcb8 callbacks[4];

    void Init(u32 state_id) {
        InitBase(state_id);
        vtable = g_unk_7101a85d00 + 2;
        for (UnkCallback_l97bcb8& c : callbacks)
            c.Init();
    }
};

struct UnkStateRecord_l97bcb8 {
    void** vtable;
    void* owner;
    void* field_0x10;
    void* field_0x18;
    u32 count;
    u8 items_raw[0x80];
    u32 field_0xa4;
    u16 field_0xa8;

    UnkState_l97bcb8** items() { return reinterpret_cast<UnkState_l97bcb8**>(items_raw); }

    void Init() {
        vtable = g_unk_7101a85db0 + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        count = 0;
        field_0xa4 = 5;
        field_0xa8 = 0;
    }

    void Add(UnkState_l97bcb8* state) {
        UnkState_l97bcb8** end = items() + count;
        for (UnkState_l97bcb8** it = items(); it != end; ++it) {
            if ((*it)->id == state->id)
                return;
        }
        state->owner = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkStateRecord_l97bcb8) == 0xb0, "layout");

struct UnkListener_l97bcb8 {
    void** vtable;
    UnkCallback_l97bcb8 callback;
    void* link;
    bool field_0x20;

    void Init() {
        vtable = g_unk_7101a22d48 + 2;
        callback.Init();
        link = nullptr;
        field_0x20 = false;
    }
};

struct UnkTail_l97bcb8 {
    void* field_0x0;
    void* field_0x8;
    void* field_0x10;
    void* field_0x18;
    u32 field_0x20;
};

struct UnkEventSource_l97bcb8 : UnkEventBase_l97bcb8 {
    UnkStateRecord_l97bcb8 record;
    UnkState_l97bcb8 state_1;
    UnkState_l97bcb8 state_2;
    UnkState_l97bcb8 state_3;
    UnkTail_l97bcb8 field_0x230;
    UnkListener_l97bcb8 listener;
};
static_assert(__builtin_offsetof(UnkEventSource_l97bcb8, state_1) == 0xe0, "layout");
static_assert(__builtin_offsetof(UnkEventSource_l97bcb8, field_0x230) == 0x230, "layout");
static_assert(__builtin_offsetof(UnkEventSource_l97bcb8, listener) == 0x258, "layout");

void fn_710097bcb8(UnkEventSource_l97bcb8* self, u32 id) {
    self->InitBase(id);
    self->vtable = g_unk_7101a85bd8 + 2;
    self->record.Init();
    self->state_1.Init(1);
    self->state_2.Init(2);
    self->state_3.Init(3);
    self->field_0x230.field_0x0 = nullptr;
    self->field_0x230.field_0x8 = nullptr;
    self->field_0x230.field_0x10 = nullptr;
    self->field_0x230.field_0x18 = nullptr;
    self->field_0x230.field_0x20 = 0;
    self->listener.Init();

    self->record.owner = self;
    self->record.Add(&self->state_1);
    self->record.Add(&self->state_2);
    self->state_2.callbacks[0].Bind(self, reinterpret_cast<void*>(&fn_710097eba0));
    self->state_2.callbacks[1].Bind(self, reinterpret_cast<void*>(&fn_710097eba4));
    self->state_2.callbacks[2].Bind(self, reinterpret_cast<void*>(&fn_710097ec3c));
    self->record.Add(&self->state_3);
    self->state_3.callbacks[0].Bind(self, reinterpret_cast<void*>(&fn_710097ec40));
    self->state_3.callbacks[1].Bind(self, reinterpret_cast<void*>(&fn_710097ec44));
    self->state_3.callbacks[2].Bind(self, reinterpret_cast<void*>(&fn_710097ece0));
    self->listener.callback.Bind(self, reinterpret_cast<void*>(&fn_710097eb88));
}
