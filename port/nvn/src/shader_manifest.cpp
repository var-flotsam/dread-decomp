// Reads the shader manifest tools/bshdat.py --install writes (format 3; the record list is in that
// tool's install()). One record per line, fields separated by single spaces, the free text last.

#include "nvn_shim/shader_manifest.h"

#include <cctype>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace dread::nvn {

namespace {

constexpr int kFormat = 3;

// Splits off the next space-separated field of `rest`.
bool NextField(const char*& rest, std::string* out) {
    if (*rest == '\0')
        return false;
    const char* end = std::strchr(rest, ' ');
    if (!end)
        end = rest + std::strlen(rest);
    out->assign(rest, end);
    rest = *end ? end + 1 : end;
    return !out->empty();
}

// Digits only (strtoull alone would take a sign, spaces or a 0x prefix), no overflow.
bool ParseU64(const std::string& text, int base, std::uint64_t* out) {
    if (text.empty() || !std::isxdigit(static_cast<unsigned char>(text[0])) ||
        (text.size() > 1 && (text[1] == 'x' || text[1] == 'X')))
        return false;
    char* end = nullptr;
    errno = 0;
    *out = std::strtoull(text.c_str(), &end, base);
    return *end == '\0' && errno == 0;
}

bool ParseI64(const std::string& text, std::int64_t* out) {
    if (text.empty() || !(text[0] == '-' || std::isdigit(static_cast<unsigned char>(text[0]))))
        return false;
    char* end = nullptr;
    errno = 0;
    *out = std::strtoll(text.c_str(), &end, 10);
    return *end == '\0' && errno == 0;
}

bool InRange(std::int64_t v, std::int64_t lo, std::int64_t hi) { return v >= lo && v <= hi; }

// Bounds on the manifest's numbers, far above what the dump has (at most 4 programs a package,
// 5 blocks and texture slots 0-13 a stage, bindings below 32), so that a damaged file fails here
// instead of in an allocation or a loop.
constexpr std::int64_t kMaxCount = 1 << 16;
constexpr std::int64_t kMaxBinding = 255;
constexpr std::int64_t kMaxSize = std::int64_t(1) << 30;
constexpr std::int64_t kU32Max = 0xFFFFFFFF;

// Reads `count` decimal fields, then the rest of the line as text (may be empty when no text).
bool Fields(const char* rest, std::int64_t* values, int count, std::string* text) {
    std::string field;
    for (int i = 0; i < count; ++i) {
        if (!NextField(rest, &field) || !ParseI64(field, &values[i]))
            return false;
    }
    if (text)
        text->assign(rest);
    return true;
}

bool ReadFile(const std::string& path, std::string* out) {
    std::FILE* f = std::fopen(path.c_str(), "rb");
    if (!f)
        return false;
    char buf[1 << 16];
    std::size_t n;
    while ((n = std::fread(buf, 1, sizeof(buf), f)) > 0)
        out->append(buf, n);
    bool ok = !std::ferror(f);
    std::fclose(f);
    return ok;
}

}  // namespace

std::uint64_t ShaderManifest::ControlKey(const void* control) {
    const auto* p = static_cast<const unsigned char*>(control);
    std::uint64_t h = 0xCBF29CE484222325ull;
    for (std::size_t i = 0; i < kControlKeyBytes; ++i)
        h = (h ^ p[i]) * 0x100000001B3ull;
    return h;
}

const StageInfo* ShaderManifest::FindStage(const void* control) const {
    auto it = by_control_.find(ControlKey(control));
    if (it == by_control_.end())
        return nullptr;
    const StageRef& r = it->second;
    return &packages_[r.package].programs[r.program].stages[r.stage];
}

bool ShaderManifest::Load(const std::string& dir, std::string* error) {
    *this = ShaderManifest();
    std::string text;
    const std::string path = dir + "/shaders.manifest";
    if (!ReadFile(path, &text)) {
        *error = path + ": cannot read (build the `shaders` target)";
        return false;
    }
    std::vector<PackageInfo> packages;
    std::unordered_map<std::uint64_t, StageRef> by_control;
    std::string game_version;
    std::int64_t layout[3] = {0, 1, 16};
    bool header = false, ended = false;
    // The counts the package and program records announce, checked when the next one starts.
    std::int64_t want_programs = -1, want_defines = -1;
    auto counts_ok = [&]() {
        if (packages.empty())
            return true;
        const PackageInfo& p = packages.back();
        if (static_cast<std::int64_t>(p.programs.size()) != want_programs)
            return false;
        return p.programs.empty() || static_cast<std::int64_t>(p.programs.back().defines.size()) == want_defines;
    };
    std::size_t line_no = 0, pos = 0;
    auto fail = [&](const char* what) {
        *error = path + ":" + std::to_string(line_no) + ": " + what;
        return false;
    };
    while (pos < text.size()) {
        std::size_t eol = text.find('\n', pos);
        if (eol == std::string::npos)
            eol = text.size();
        std::string line = text.substr(pos, eol - pos);
        pos = eol + 1;
        ++line_no;
        if (line.empty())
            continue;
        if (ended)
            return fail("text after the end record");
        const char* rest = line.c_str();
        std::string tag;
        NextField(rest, &tag);
        std::int64_t v[5];
        std::string name;
        if (!header) {
            // dread-shaders <format> <game version> glslang <version>
            std::string format, word;
            if (tag != "dread-shaders" || !NextField(rest, &format) || format != std::to_string(kFormat))
                return fail("not a format-3 shader manifest (rebuild the `shaders` target)");
            if (!NextField(rest, &game_version) || !NextField(rest, &word) || word != "glslang")
                return fail("malformed header");
            header = true;
            continue;
        }
        PackageInfo* pkg = packages.empty() ? nullptr : &packages.back();
        ProgramInfo* prog = pkg && !pkg->programs.empty() ? &pkg->programs.back() : nullptr;
        StageInfo* stage = prog && !prog->stages.empty() ? &prog->stages.back() : nullptr;
        if (tag == "layout") {
            if (!Fields(rest, layout, 3, nullptr) || !InRange(layout[0], 0, 31) || !InRange(layout[1], 0, 31) ||
                !InRange(layout[2], 0, kMaxBinding))
                return fail("malformed layout record");
        } else if (tag == "package") {
            std::string pname;
            if (!NextField(rest, &pname) || !Fields(rest, v, 1, nullptr) || !InRange(v[0], 0, kMaxCount))
                return fail("malformed package record");
            if (!counts_ok())
                return fail("the previous package has another number of programs or defines than announced");
            packages.push_back(PackageInfo{pname, {}, {}, {}});
            packages.back().programs.reserve(static_cast<std::size_t>(v[0]));
            want_programs = v[0];
        } else if (tag == "attribute" || tag == "uniform") {
            if (!pkg)
                return fail("name record outside a package");
            if (tag == "attribute") {
                if (!Fields(rest, v, 1, &name) || !InRange(v[0], 0, kU32Max))
                    return fail("malformed attribute record");
                pkg->attributes.push_back(NameId{static_cast<std::uint32_t>(v[0]), name});
            } else {
                if (!Fields(rest, v, 2, &name) || (v[0] != 1 && v[0] != 2) || !InRange(v[1], 0, kU32Max))
                    return fail("malformed uniform record");
                pkg->uniforms[v[0] - 1].push_back(NameId{static_cast<std::uint32_t>(v[1]), name});
            }
        } else if (tag == "program") {
            if (!pkg || !Fields(rest, v, 4, nullptr) || !InRange(v[0], 0, kU32Max) || !InRange(v[1], 0, kU32Max) ||
                !InRange(v[2], 0, kMaxCount) || !InRange(v[3], 0, kMaxCount))
                return fail("malformed program record");
            if (!pkg->programs.empty() && static_cast<std::int64_t>(pkg->programs.back().defines.size()) != want_defines)
                return fail("the previous program has another number of defines than announced");
            want_defines = v[3];
            ProgramInfo p;
            p.key = static_cast<std::uint32_t>(v[0]);
            p.key2 = static_cast<std::uint32_t>(v[1]);
            p.index = static_cast<std::uint32_t>(v[2]);
            pkg->programs.push_back(std::move(p));
        } else if (tag == "define") {
            if (!prog || !prog->stages.empty())
                return fail("define outside a program's header");
            prog->defines.emplace_back(rest);
        } else if (tag == "stage") {
            // stage <vert|frag> <spv path> <spv bytes> <control key> <control bytes> <control offset>
            std::string kind, spv, size, key, csize, coff;
            std::uint64_t n_size, n_key, n_csize, n_coff;
            if (!prog || !NextField(rest, &kind) || !NextField(rest, &spv) || !NextField(rest, &size) ||
                !NextField(rest, &key) || !NextField(rest, &csize) || !NextField(rest, &coff) || *rest != '\0' ||
                !ParseU64(size, 10, &n_size) || !ParseU64(key, 16, &n_key) || !ParseU64(csize, 10, &n_csize) ||
                !ParseU64(coff, 10, &n_coff) || n_coff > static_cast<std::uint64_t>(kU32Max) ||
                (kind != "vert" && kind != "frag") || n_size > static_cast<std::uint64_t>(kU32Max) ||
                n_csize > static_cast<std::uint64_t>(kU32Max))
                return fail("malformed stage record");
            if (n_csize < kControlKeyBytes)
                return fail("control section shorter than the key");
            StageInfo s;
            s.stage = kind == "vert" ? Stage::Vertex : Stage::Fragment;
            s.spirv_path = spv;
            s.spirv_size = static_cast<std::uint32_t>(n_size);
            s.control_key = n_key;
            s.control_size = static_cast<std::uint32_t>(n_csize);
            s.control_offset = static_cast<std::uint32_t>(n_coff);
            prog->stages.push_back(std::move(s));
            by_control.emplace(n_key, StageRef{static_cast<std::uint32_t>(packages.size() - 1),
                                               static_cast<std::uint32_t>(pkg->programs.size() - 1),
                                               static_cast<std::uint32_t>(prog->stages.size() - 1)});
        } else if (tag == "block" || tag == "sampler") {
            // A slot is -1 or 0-15; a block's size is positive and a sampler's count at least 1 (a
            // descriptor write may not have a descriptorCount of 0).
            if (!stage || !Fields(rest, v, 3, &name) || name.empty() || !InRange(v[0], -1, kTextureSlots - 1) ||
                !InRange(v[1], 0, kMaxBinding) || !InRange(v[2], 1, tag == "block" ? kMaxSize : 64))
                return fail("malformed block or sampler record");
            StageResource r{static_cast<std::int32_t>(v[0]), static_cast<std::uint32_t>(v[1]),
                            static_cast<std::uint32_t>(v[2]), name};
            (tag == "block" ? stage->blocks : stage->samplers).push_back(std::move(r));
        } else if (tag == "input" || tag == "output") {
            if (!stage || !Fields(rest, v, 1, &name) || !InRange(v[0], -1, kMaxBinding))
                return fail("malformed input or output record");
            (tag == "input" ? stage->inputs : stage->outputs).push_back(
                StageInterface{static_cast<std::int32_t>(v[0]), name});
        } else if (tag == "end") {
            std::size_t programs = 0, stages = 0;
            for (const PackageInfo& p : packages) {
                programs += p.programs.size();
                for (const ProgramInfo& q : p.programs)
                    stages += q.stages.size();
            }
            if (!counts_ok())
                return fail("the last package has another number of programs or defines than announced");
            if (!Fields(rest, v, 3, nullptr) || !InRange(v[0], 0, kMaxSize) || !InRange(v[1], 0, kMaxSize) ||
                !InRange(v[2], 0, kMaxSize) || static_cast<std::size_t>(v[0]) != packages.size() ||
                static_cast<std::size_t>(v[1]) != programs || static_cast<std::size_t>(v[2]) != stages)
                return fail("end record does not match the records read");
            ended = true;
        } else {
            return fail("unknown record");
        }
    }
    if (!ended) {
        ++line_no;
        return fail("no end record (an interrupted install?)");
    }
    dir_ = dir;
    game_version_ = game_version;
    vertex_set_ = static_cast<std::uint32_t>(layout[0]);
    fragment_set_ = static_cast<std::uint32_t>(layout[1]);
    sampler_base_ = static_cast<std::uint32_t>(layout[2]);
    packages_ = std::move(packages);
    by_control_ = std::move(by_control);
    return true;
}

}  // namespace dread::nvn
