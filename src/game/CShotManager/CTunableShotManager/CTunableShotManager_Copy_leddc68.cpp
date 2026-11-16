// CShotManager::CTunableShotManager's unit (tools/tu_map.py, unit 2576): a shot's copy constructor (the
// header calls it the move constructor; the shot vector uses it to move shots over), member by member.
// Names and layouts are placeholders.
#include <new>

#include "base/UnkCountedResource.h"
#include "base/global/CFilePathStrId.h"
#include "base/global/CStrId.h"
#include "game/CShotManager/CTunableShotManager.h"
#include "game_s2/UnkWeakHandle.h"
#include "types.h"

using base::global::CFilePathStrId;
using base::global::CStrId;

extern void* g_unk_7101ca9fe0[];

template <class T>
struct UnkCopyVec_b0011;
struct UnkRefRecord10_b0011;
struct UnkRefRecord18_b0011;
using UnkRefRecord10Vec_b0011 = UnkCopyVec_b0011<UnkRefRecord10_b0011>;
using UnkRefRecord18Vec_b0011 = UnkCopyVec_b0011<UnkRefRecord18_b0011>;
void fn_7100ede32c(UnkRefRecord10Vec_b0011* self, const UnkRefRecord10Vec_b0011* other);
void fn_7100ede1f0(UnkRefRecord18Vec_b0011* self, const UnkRefRecord18Vec_b0011* other);

struct UnkWords2_leddc68 {
    u32 field_0x0;
    u32 field_0x4;
};
struct UnkWords3_leddc68 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
};
struct UnkWords5_leddc68 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
};
struct UnkWords7_leddc68 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;
};

// Seven words of a shot part, copied one by one.
struct UnkPartWords_leddc68 {
    u32 field_0x0;
    u32 field_0x4;
    u32 field_0x8;
    u32 field_0xc;
    u32 field_0x10;
    u32 field_0x14;
    u32 field_0x18;

    UnkPartWords_leddc68(const UnkPartWords_leddc68& other)
        : field_0x0(other.field_0x0), field_0x4(other.field_0x4), field_0x8(other.field_0x8),
          field_0xc(other.field_0xc), field_0x10(other.field_0x10), field_0x14(other.field_0x14),
          field_0x18(other.field_0x18) {}
};

// One of the shot's five parts.
struct UnkShotPart_leddc68 {
    UnkPartWords_leddc68 field_0x0;
    UnkWords2_leddc68 field_0x1c;
    bool field_0x24;
};
static_assert(sizeof(UnkShotPart_leddc68) == 0x28, "placeholder layout");

struct UnkShot_leddc68 {
    void** vtable;
    UnkWords7_leddc68 field_0x8;
    UnkRefObject* field_0x28;
    UnkRefObject* field_0x30;
    u32 field_0x38[12];
    UnkRefObject* field_0x68;
    UnkRefObject* field_0x70;
    UnkRefObject* field_0x78;
    UnkWords5_leddc68 field_0x80;
    u32 field_0x94[24];
    u8 pad_0xf4[0x4];
    u8 field_0xf8[0x28];   // UnkRefRecord18Vec_b0011
    u8 field_0x120[0x28];  // UnkRefRecord10Vec_b0011
    UnkWords3_leddc68 field_0x148;
    CStrId field_0x158;
    CFilePathStrId field_0x160;
    UnkCountedResource* field_0x168;
    UnkWords3_leddc68 field_0x170;
    u32 field_0x17c[16];
    UnkWords2_leddc68 field_0x1bc;
    UnkShotPart_leddc68 field_0x1c4[5];
    bool field_0x28c;
};
static_assert(__builtin_offsetof(UnkShot_leddc68, field_0x158) == 0x158, "placeholder layout");
static_assert(__builtin_offsetof(UnkShot_leddc68, field_0x1c4) == 0x1c4, "placeholder layout");
static_assert(sizeof(UnkShot_leddc68) == 0x290, "placeholder layout");

static inline void AssignResource(UnkCountedResource** slot, UnkCountedResource* p) {
    if (*slot != p) {
        if (p)
            ++p->ref_count;
        if (UnkCountedResource* old = *slot) {
            if (--old->ref_count == 0)
                fn_710001737c(old);
        }
        *slot = p;
    }
}

void fn_7100eddc68(UnkObj290_b43a3* selfObj, UnkObj290_b43a3* otherObj) {
    UnkShot_leddc68* self = reinterpret_cast<UnkShot_leddc68*>(selfObj);
    const UnkShot_leddc68* other = reinterpret_cast<const UnkShot_leddc68*>(otherObj);
    self->vtable = g_unk_7101ca9fe0 + 2;
    self->field_0x8 = other->field_0x8;
    self->field_0x28 = nullptr;
    AssignRef(&self->field_0x28, other->field_0x28);
    self->field_0x30 = nullptr;
    AssignRef(&self->field_0x30, other->field_0x30);
    self->field_0x38[0] = other->field_0x38[0];
    self->field_0x38[1] = other->field_0x38[1];
    self->field_0x38[2] = other->field_0x38[2];
    self->field_0x38[3] = other->field_0x38[3];
    self->field_0x38[4] = other->field_0x38[4];
    self->field_0x38[5] = other->field_0x38[5];
    self->field_0x38[6] = other->field_0x38[6];
    self->field_0x38[7] = other->field_0x38[7];
    self->field_0x38[8] = other->field_0x38[8];
    self->field_0x38[9] = other->field_0x38[9];
    self->field_0x38[10] = other->field_0x38[10];
    self->field_0x38[11] = other->field_0x38[11];
    self->field_0x68 = nullptr;
    AssignRef(&self->field_0x68, other->field_0x68);
    self->field_0x70 = nullptr;
    AssignRef(&self->field_0x70, other->field_0x70);
    self->field_0x78 = nullptr;
    AssignRef(&self->field_0x78, other->field_0x78);
    self->field_0x80 = other->field_0x80;
    self->field_0x94[0] = other->field_0x94[0];
    self->field_0x94[1] = other->field_0x94[1];
    self->field_0x94[2] = other->field_0x94[2];
    self->field_0x94[3] = other->field_0x94[3];
    self->field_0x94[4] = other->field_0x94[4];
    self->field_0x94[5] = other->field_0x94[5];
    self->field_0x94[6] = other->field_0x94[6];
    self->field_0x94[7] = other->field_0x94[7];
    self->field_0x94[8] = other->field_0x94[8];
    self->field_0x94[9] = other->field_0x94[9];
    self->field_0x94[10] = other->field_0x94[10];
    self->field_0x94[11] = other->field_0x94[11];
    self->field_0x94[12] = other->field_0x94[12];
    self->field_0x94[13] = other->field_0x94[13];
    self->field_0x94[14] = other->field_0x94[14];
    self->field_0x94[15] = other->field_0x94[15];
    self->field_0x94[16] = other->field_0x94[16];
    self->field_0x94[17] = other->field_0x94[17];
    self->field_0x94[18] = other->field_0x94[18];
    self->field_0x94[19] = other->field_0x94[19];
    self->field_0x94[20] = other->field_0x94[20];
    self->field_0x94[21] = other->field_0x94[21];
    self->field_0x94[22] = other->field_0x94[22];
    self->field_0x94[23] = other->field_0x94[23];
    fn_7100ede1f0(reinterpret_cast<UnkRefRecord18Vec_b0011*>(self->field_0xf8),
                  reinterpret_cast<const UnkRefRecord18Vec_b0011*>(other->field_0xf8));
    fn_7100ede32c(reinterpret_cast<UnkRefRecord10Vec_b0011*>(self->field_0x120),
                  reinterpret_cast<const UnkRefRecord10Vec_b0011*>(other->field_0x120));
    self->field_0x148 = other->field_0x148;
    new (&self->field_0x158) CStrId(other->field_0x158);
    new (&self->field_0x160) CFilePathStrId(other->field_0x160);
    self->field_0x168 = nullptr;
    AssignResource(&self->field_0x168, other->field_0x168);
    self->field_0x170 = other->field_0x170;
    self->field_0x17c[0] = other->field_0x17c[0];
    self->field_0x17c[1] = other->field_0x17c[1];
    self->field_0x17c[2] = other->field_0x17c[2];
    self->field_0x17c[3] = other->field_0x17c[3];
    self->field_0x17c[4] = other->field_0x17c[4];
    self->field_0x17c[5] = other->field_0x17c[5];
    self->field_0x17c[6] = other->field_0x17c[6];
    self->field_0x17c[7] = other->field_0x17c[7];
    self->field_0x17c[8] = other->field_0x17c[8];
    self->field_0x17c[9] = other->field_0x17c[9];
    self->field_0x17c[10] = other->field_0x17c[10];
    self->field_0x17c[11] = other->field_0x17c[11];
    self->field_0x17c[12] = other->field_0x17c[12];
    self->field_0x17c[13] = other->field_0x17c[13];
    self->field_0x17c[14] = other->field_0x17c[14];
    self->field_0x17c[15] = other->field_0x17c[15];
    new (&self->field_0x1bc) UnkWords2_leddc68(other->field_0x1bc);
    for (u32 i = 0; i < 5; ++i)
        new (&self->field_0x1c4[i]) UnkShotPart_leddc68(other->field_0x1c4[i]);
    self->field_0x28c = other->field_0x28c;
}
