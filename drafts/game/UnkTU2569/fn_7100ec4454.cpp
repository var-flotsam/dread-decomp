// Unit 2569 (the play report, src/game/UnkPlayReport.cpp): builds a report of the given kind and ids, fills its
// header and appends `extra`'s entries, keeps a copy in the owner's list, then sends it as an nn::prepo play
// report (each entry as an integer, a float or a string) for the current user. Names and layouts are placeholders.
#include <nn/prepo.h>

#include "base/UnkNoArg.h"
#include "base/global/CRntVector.h"
#include "base/global/CStrId.h"
#include "types.h"

using base::global::CStrId;

// A report entry: its key, a string value and the integer and float values; the first of these that is set
// (not negative, not empty) is the one sent.
struct UnkReportEntry_lec4454 {
    CStrId key;
    CStrId value;
    s64 int_value;
    f32 float_value;
};
static_assert(sizeof(UnkReportEntry_lec4454) == 0x20, "");

// The report (0x40 bytes): its kind, two ids and its entries. Its constructor, header fill, entry append,
// copy constructor and destructor are out of line.
struct UnkReport_lec4454 {
    u32 kind;
    CStrId field_0x8;
    CStrId field_0x10;
    base::global::CRntVector<UnkReportEntry_lec4454> entries;  // 0x18

    UnkReport_lec4454(u32 kind, CStrId a, CStrId b) asm("fn_7100ec489c");
    UnkReport_lec4454(const UnkReport_lec4454& other) asm("fn_7100ec536c");
    ~UnkReport_lec4454() asm("fn_7100ec4d1c");
    bool FillHeader(bool fixed_mode) asm("fn_7100ec32c0");
};
static_assert(sizeof(UnkReport_lec4454) == 0x40, "");

// Appends a copy of each of `src`'s entries to the report's.
void AppendEntries_lec4454(UnkNoArg, UnkReport_lec4454* report,
                           base::global::CRntVector<UnkReportEntry_lec4454>* src) asm("fn_7100ec4ad8");

// nn::prepo::PlayReport::Add with a float value (in the SDK this game links; lib/nnsdk's header lacks it).
nn::Result PlayReportAddFloat_lec4454(nn::prepo::PlayReport* report, const char* key, f32 value) asm(
    "_ZN2nn5prepo10PlayReport3AddEPKcf");

// The current user (src/engine/UnkUnit110/UnkUnit110_Ids_b65a1.cpp).
struct UnkUserId_b65a1 {
    u64 words[2];
};
UnkUserId_b65a1 fn_71001329f0();

// Adds one entry to the play report: its integer value when that is not negative, else its float value when
// that is not negative, else its string value unless that is empty.
inline nn::Result AddEntry_lec4454(nn::prepo::PlayReport& play_report, UnkReportEntry_lec4454 entry) {
    if (entry.int_value >= 0)
        return play_report.Add(entry.key.Text(), entry.int_value);
    if (entry.float_value >= 0.0f)
        return PlayReportAddFloat_lec4454(&play_report, entry.key.Text(), entry.float_value);
    if (entry.value.data == &g_unk_7101d47108->empty)
        return nn::ResultSuccess();
    return play_report.Add(entry.key.Text(), entry.value.Text());
}

struct UnkReporter_lec4454 {
    base::global::CRntVector<UnkReport_lec4454> reports;
    bool enabled;  // 0x28
};

bool fn_7100ec4454(UnkReporter_lec4454* self, u32 kind, const CStrId& a, const CStrId& b,
                   base::global::CRntVector<UnkReportEntry_lec4454>* extra) {
    if (!self->enabled)
        return false;
    UnkReport_lec4454 report(kind, a, b);
    if (!report.FillHeader(kind == 0))
        return false;
    if (extra)
        AppendEntries_lec4454(UnkNoArg(), &report, extra);
    self->reports.PushBackCopy(report);

    nn::prepo::PlayReport play_report(a.Text());
    char buffer[0x20b];
    play_report.SetBuffer(buffer, sizeof(buffer));
    for (const UnkReportEntry_lec4454& entry : report.entries) {
        if (AddEntry_lec4454(play_report, entry).IsFailure())
            break;
    }
    UnkUserId_b65a1 user = fn_71001329f0();
    return play_report.Save(reinterpret_cast<const nn::account::Uid&>(user)).IsSuccess();
}
