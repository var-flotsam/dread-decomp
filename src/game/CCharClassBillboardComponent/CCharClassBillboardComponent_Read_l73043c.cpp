// CCharClassBillboardComponent's unit (tools/tu_map.py unit 1154): the read. After the base read, the
// billboard's name and its requests (an id and seven values each, handed to the billboard manager with
// fn_7100e1cd34), then the second name and its records (an id, a flag and a list of offsets, registered
// with fn_7100e1cfec). Names and layouts are placeholders.
#include "base/global/CRntFile.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "game/CCharClassComponent.h"
#include "game/CDizzeanSwarmControllerComponent.h"
#include "game/UnkTU0.h"
#include "game/UnkTU2560.h"
#include "game/UnkTU56.h"
#include "game/UnkXformNode.h"
#include "types.h"

using base::global::CRntFile;
using base::global::CRntVector;
using base::global::CStrId;
using base::global::UnkStrId;
using base::global::UnkStrIdBase;

struct UnkCollisionClassDef;
struct UnkDropTarget_ovb0208;
struct UnkDropRequest_ovb0208;

// A request: an id and seven values.
struct UnkBillboardRequest_l73043c {
    CStrId id;  // 0x0
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u8 field_0x14;
    u32 field_0x18;
    u32 field_0x1c;
    f32 field_0x20;

    UnkBillboardRequest_l73043c()
        : id(g_unk_710162e2ed, true), field_0x8(0), field_0xc(0), field_0x10(0), field_0x14(0), field_0x18(0),
          field_0x1c(0), field_0x20(1.0f) {}
};
static_assert(__builtin_offsetof(UnkBillboardRequest_l73043c, field_0x20) == 0x20, "request layout");

// A record: an id, a flag and one value per key in four arrays (only the offsets are read here).
struct UnkBillboardRecord_l73043c {
    CStrId id;
    bool field_0x8;
    CRntVector<UnkVec3f> offsets;  // 0x10
    CRntVector<f32> scales;        // 0x38
    CRntVector<u8> flags;          // 0x60
    CRntVector<f32> alphas;        // 0x88

    UnkBillboardRecord_l73043c()
        : id(g_unk_710162e2ed, true), field_0x8(false), offsets(0xf), scales(0xf), flags(0xf), alphas(0xf) {}
    ~UnkBillboardRecord_l73043c() {
        alphas.Free();
        flags.Free();
        scales.Free();
        offsets.Free();
    }
};
static_assert(sizeof(UnkBillboardRecord_l73043c) == 0xb0, "record layout");

struct UnkBillboardClass_l73043c {
    u8 pad_0x0[0xd0];
    UnkStrId name;         // 0xd0
    UnkStrId record_name;  // 0xd8
};

static inline void* BillboardManager() {
    void* p = g_unk_71726da048;
    if (!p) {
        p = fn_71000839a0(0xe0, 0x3f);
        fn_7100e1bca4(p);
        g_unk_71726da048 = p;
    }
    return p;
}

bool fn_710073043c(UnkBillboardClass_l73043c* self, CRntFile* file) {
    bool result = fn_71011aced4(reinterpret_cast<UnkCollisionClassDef*>(self), file);
    if (result) {
        const char* name;
        CRntFile_ReadCString(file, &name);
        self->name = UnkStrId(name, true);

        s32 request_count = 0;
        CRntFile_Read(file, &request_count, 4, 1);
        for (s32 i = 0; i < request_count; ++i) {
            UnkBillboardRequest_l73043c request;
            const char* id;
            CRntFile_ReadCString(file, &id);
            CStrId request_id(id, true);
            request.id = request_id;
            CRntFile_Read(file, &request.field_0x8, 4, 1);
            CRntFile_Read(file, &request.field_0xc, 4, 1);
            CRntFile_Read(file, &request.field_0x10, 4, 1);
            CRntFile_Read(file, &request.field_0x14, 1, 1);
            CRntFile_Read(file, &request.field_0x18, 4, 1);
            CRntFile_Read(file, &request.field_0x1c, 4, 1);
            CRntFile_Read(file, &request.field_0x20, 4, 1);
            fn_7100e1cd34(BillboardManager(), reinterpret_cast<UnkDropTarget_ovb0208*>(&self->name),
                          reinterpret_cast<UnkDropRequest_ovb0208*>(&request));
        }

        // The second name is read, but the id is built from the first one again, as in the original.
        const char* record_name;
        CRntFile_ReadCString(file, &record_name);
        self->record_name = UnkStrId(name, true);

        s32 record_count = 0;
        CRntFile_Read(file, &record_count, 4, 1);
        for (s32 i = 0; i < record_count; ++i) {
            UnkBillboardRecord_l73043c record;
            const char* id;
            CRntFile_ReadCString(file, &id);
            CStrId record_id(id, true);
            record.id = record_id;
            CRntFile_Read(file, &record.field_0x8, 1, 1);
            s32 offset_count = 0;
            CRntFile_Read(file, &offset_count, 4, 1);
            for (s32 j = 0; j < offset_count; ++j) {
                UnkVec3f offset(0.0f, 0.0f, 0.0f);
                CRntFile_Read(file, &offset, 0xc, 1);
                record.offsets.PushBackCopy(offset);
            }
            fn_7100e1cfec(BillboardManager(), static_cast<CStrId*>(static_cast<UnkStrIdBase*>(&self->record_name)), reinterpret_cast<UnkRecord_b218*>(&record));
        }
    }
    return result;
}
