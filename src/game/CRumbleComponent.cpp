// CRumbleComponent's unit (its reflection is src/reflection/CRumbleComponent.cpp): the component
// that plays controller rumbles through the rumble manager (base/input/UnkRumbleManager.h) and
// keeps a record of each one it started.
// Holds fn_71009dacbc (start a rumble). Names and layouts are placeholders.
#include "base/UnkVector.h"
#include "base/global/CFilePathStrId.h"
#include "base/input/UnkRumbleManager.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::UnkStringData;
using base::global::UnkStringPool;
using base::input::UnkRumbleManager;

// A handle to a playing rumble (as in src/game/CAbilityEnergyWave/fn_71003053c4.cpp): current while the target's
// serial is the handle's.
struct UnkHandleTarget {
    void* field_0x0;
    u64 field_0x8;
};

struct UnkHandle {
    UnkHandleTarget* target;
    u64 serial;
};

// The null handle.
extern UnkHandle g_unk_7101d586c0;

// The rumble manager, created on first use.
extern UnkRumbleManager* g_unk_7101d47348;

static inline UnkRumbleManager* RumbleManager() {
    if (!g_unk_7101d47348) {
        UnkRumbleManager* m = static_cast<UnkRumbleManager*>(fn_71000839a0(sizeof(UnkRumbleManager), 0x3f));
        fn_71000733e4(m);
        g_unk_7101d47348 = m;
    }
    return g_unk_7101d47348;
}

// The rumble's file: a CFilePathStrId assigned as CStrId is (on the file-path pool).
struct UnkRumbleParamsPath : CFilePathStrId {
    UnkRumbleParamsPath& operator=(const UnkRumbleParamsPath& other) {
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

// Two of its members have a user-written assignment: three words copied one by one, and a single
// 8-byte word. The fields around them are copied as blocks.
struct UnkRumbleWords3 {
    u32 x, y, z;
    UnkRumbleWords3& operator=(const UnkRumbleWords3& o) {
        x = o.x;
        y = o.y;
        z = o.z;
        return *this;
    }
};

struct UnkRumbleWord {
    u64 value;
    UnkRumbleWord& operator=(const UnkRumbleWord& o) {
        value = o.value;
        return *this;
    }
};

// What a rumble is played from (0x48 bytes): its file and settings.
struct UnkRumbleParams {
    UnkRumbleParamsPath path;
    u64 field_0x8;
    u32 field_0x10;
    UnkRumbleWords3 field_0x14;
    UnkRumbleWord field_0x20;
    u64 field_0x28;
    u64 field_0x30;
    u64 field_0x38;
    u16 field_0x40;
};

bool fn_710007453c(UnkRumbleManager* self);
UnkHandle fn_7100074354(UnkRumbleManager* self, const UnkRumbleParams* params);
void fn_7100072030(void* rumble, void* arg);

// A rumble the component started (0x68 bytes): its handle, what it was played from and two flags.
struct UnkRumbleRecord {
    UnkHandle handle;
    UnkRumbleParams params;
    u8 unk_0x58[8];
    bool field_0x60;
    bool field_0x61;
    bool field_0x62;
};
static_assert(sizeof(UnkRumbleRecord) == 0x68, "UnkRumbleRecord layout");

struct UnkRumbleRecords : base::UnkVector<UnkRumbleRecord> {
    UnkRumbleRecord& Back() {
        u32 n = count;
        UnkRumbleRecord* end = data + n;
        return n ? end[-1] : *end;
    }
};

void fn_71009dbdb8(UnkRumbleRecords* self, u32 count);  // resizes

struct CRumbleComponent {
    u8 unk_0x0[0x40];
    u8 field_0x40[8];
    UnkRumbleRecords records;  // 0x48
};

// Starts a rumble when the manager is enabled, hands it the component's field at +0x40 and records
// it with the two flags; returns its handle (the null handle when disabled).
UnkHandle fn_71009dacbc(CRumbleComponent* self, const UnkRumbleParams* params, bool a, bool b) {
    if (!fn_710007453c(RumbleManager()))
        return g_unk_7101d586c0;
    UnkHandle handle = fn_7100074354(RumbleManager(), params);
    if (handle.target && handle.serial != ~0ull && handle.target->field_0x8 == handle.serial &&
        handle.target->field_0x0)
        fn_7100072030(handle.target->field_0x0, self->field_0x40);
    fn_71009dbdb8(&self->records, self->records.count + 1);
    UnkRumbleRecord& r = self->records.Back();
    r.handle = handle;
    r.params = *params;
    r.field_0x60 = a;
    r.field_0x61 = b;
    r.field_0x62 = false;
    return handle;
}
