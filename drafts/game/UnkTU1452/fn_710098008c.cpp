// Unit 1452 of tools/tu_map.py (no name hint): the constructor of an event source with a state record
// that holds three states (ids 1 to 3), each with up to four callbacks. Names and layouts are
// placeholders.
#include "base/reflection/UnkReflValue.h"
#include "game/UnkTU1452.h"
#include "types.h"

extern void* g_unk_7101a85f40[];

void fn_71009825ec();
void fn_71009825f0();
void fn_71009825f4();
void fn_71009825f8();
void fn_71009825fc();
void fn_7100982600();

struct UnkCallback_l98008c {
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

struct UnkStateRecord_l98008c;

struct UnkEventBase_l98008c {
    void** vtable;
    UnkStateRecord_l98008c* owner;
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

struct UnkState_l98008c : UnkEventBase_l98008c {
    UnkCallback_l98008c callbacks[4];

    void Init(u32 state_id) {
        InitBase(state_id);
        vtable = g_unk_7101a85f40 + 2;
        for (UnkCallback_l98008c& c : callbacks)
            c.Init();
    }
};

struct UnkStateRecord_l98008c {
    void** vtable;
    void* owner;
    void* field_0x10;
    void* field_0x18;
    u32 count;
    u8 items_raw[0x80];
    u32 field_0xa4;
    u16 field_0xa8;

    UnkState_l98008c** items() { return reinterpret_cast<UnkState_l98008c**>(items_raw); }

    void Init() {
        vtable = g_unk_7101a85ff0 + 2;
        field_0x10 = nullptr;
        field_0x18 = nullptr;
        count = 0;
        field_0xa4 = 4;
        field_0xa8 = 0;
    }

    void Add(UnkState_l98008c* state) {
        UnkState_l98008c** end = items() + count;
        for (UnkState_l98008c** it = items(); it != end; ++it) {
            if ((*it)->id == state->id)
                return;
        }
        state->owner = this;
        *end = state;
        ++count;
    }
};
static_assert(sizeof(UnkStateRecord_l98008c) == 0xb0, "layout");

struct UnkEventSource_l98008c : UnkEventBase_l98008c {
    UnkStateRecord_l98008c record;
    u32 field_0xe0;
    u32 field_0xe4;
    UnkState_l98008c state_1;
    UnkState_l98008c state_2;
    UnkState_l98008c state_3;
};
static_assert(__builtin_offsetof(UnkEventSource_l98008c, state_1) == 0xe8, "layout");
static_assert(sizeof(UnkEventSource_l98008c) == 0x238, "layout");

void fn_710098008c(UnkEventSource_l98008c* self, u32 id) {
    self->InitBase(id);
    self->vtable = g_unk_7101a85e98 + 2;
    self->record.Init();
    self->field_0xe4 = 0;
    self->state_1.Init(1);
    self->state_2.Init(2);
    self->state_3.Init(3);

    self->record.owner = self;
    self->record.Add(&self->state_1);
    self->record.Add(&self->state_2);
    self->state_2.callbacks[0].Bind(self, reinterpret_cast<void*>(&fn_71009825ec));
    self->state_2.callbacks[1].Bind(self, reinterpret_cast<void*>(&fn_71009825f0));
    self->state_2.callbacks[2].Bind(self, reinterpret_cast<void*>(&fn_71009825f4));
    self->record.Add(&self->state_3);
    self->state_3.callbacks[0].Bind(self, reinterpret_cast<void*>(&fn_71009825f8));
    self->state_3.callbacks[1].Bind(self, reinterpret_cast<void*>(&fn_71009825fc));
    self->state_3.callbacks[2].Bind(self, reinterpret_cast<void*>(&fn_7100982600));
}
