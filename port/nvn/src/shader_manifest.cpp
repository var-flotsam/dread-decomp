// Reads the shader manifest tools/bshdat.py --install writes (format 2; the record list is in that
// tool's install()). One record per line, fields separated by single spaces, the free text last.

#include "nvn_shim/shader_manifest.h"

#include <cstdio>
#include <cstdlib>
#include <cstring>

namespace dread::nvn {

namespace {

constexpr int kFormat = 2;

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

bool ParseU64(const std::string& text, int base, std::uint64_t* out) {
    if (text.empty())
        return false;
    char* end = nullptr;
    *out = std::strtoull(text.c_str(), &end, base);
    return *end == '\0';
}

bool ParseI64(const std::string& text, std::int64_t* out) {
    if (text.empty())
        return false;
    char* end = nullptr;
    *out = std::strtoll(text.c_str(), &end, 10);
    return *end == '\0';
}

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
                return fail("not a format-2 shader manifest (rebuild the `shaders` target)");
            if (!NextField(rest, &game_version) || !NextField(rest, &word) || word != "glslang")
                return fail("malformed header");
            header = true;
            continue;
        }
        PackageInfo* pkg = packages.empty() ? nullptr : &packages.back();
        ProgramInfo* prog = pkg && !pkg->programs.empty() ? &pkg->programs.back() : nullptr;
        StageInfo* stage = prog && !prog->stages.empty() ? &prog->stages.back() : nullptr;
        if (tag == "layout") {
            if (!Fields(rest, layout, 3, nullptr))
                return fail("malformed layout record");
        } else if (tag == "package") {
            std::string pname;
            if (!NextField(rest, &pname) || !Fields(rest, v, 1, nullptr))
                return fail("malformed package record");
            packages.push_back(PackageInfo{pname, {}, {}, {}});
            packages.back().programs.reserve(static_cast<std::size_t>(v[0]));
        } else if (tag == "attribute" || tag == "uniform") {
            if (!pkg)
                return fail("name record outside a package");
            if (tag == "attribute") {
                if (!Fields(rest, v, 1, &name))
                    return fail("malformed attribute record");
                pkg->attributes.push_back(NameId{static_cast<std::uint32_t>(v[0]), name});
            } else {
                if (!Fields(rest, v, 2, &name) || (v[0] != 1 && v[0] != 2))
                    return fail("malformed uniform record");
                pkg->uniforms[v[0] - 1].push_back(NameId{static_cast<std::uint32_t>(v[1]), name});
            }
        } else if (tag == "program") {
            if (!pkg || !Fields(rest, v, 4, nullptr))
                return fail("malformed program record");
            ProgramInfo p;
            p.key = static_cast<std::uint32_t>(v[0]);
            p.key2 = static_cast<std::uint32_t>(v[1]);
            p.index = static_cast<std::uint32_t>(v[2]);
            pkg->programs.push_back(std::move(p));
        } else if (tag == "define") {
            if (!prog)
                return fail("define outside a program");
            prog->defines.emplace_back(rest);
        } else if (tag == "stage") {
            // stage <vert|frag> <spv path> <spv bytes> <control key> <control bytes>
            std::string kind, spv, size, key, csize;
            std::uint64_t n_size, n_key, n_csize;
            if (!prog || !NextField(rest, &kind) || !NextField(rest, &spv) || !NextField(rest, &size) ||
                !NextField(rest, &key) || !NextField(rest, &csize) || *rest != '\0' ||
                !ParseU64(size, 10, &n_size) || !ParseU64(key, 16, &n_key) || !ParseU64(csize, 10, &n_csize) ||
                (kind != "vert" && kind != "frag"))
                return fail("malformed stage record");
            if (n_csize < kControlKeyBytes)
                return fail("control section shorter than the key");
            StageInfo s;
            s.stage = kind == "vert" ? Stage::Vertex : Stage::Fragment;
            s.spirv_path = spv;
            s.spirv_size = static_cast<std::uint32_t>(n_size);
            s.control_key = n_key;
            s.control_size = static_cast<std::uint32_t>(n_csize);
            prog->stages.push_back(std::move(s));
            by_control.emplace(n_key, StageRef{static_cast<std::uint32_t>(packages.size() - 1),
                                               static_cast<std::uint32_t>(pkg->programs.size() - 1),
                                               static_cast<std::uint32_t>(prog->stages.size() - 1)});
        } else if (tag == "block" || tag == "sampler") {
            if (!stage || !Fields(rest, v, 3, &name) || name.empty())
                return fail("malformed block or sampler record");
            StageResource r{static_cast<std::int32_t>(v[0]), static_cast<std::uint32_t>(v[1]),
                            static_cast<std::uint32_t>(v[2]), name};
            (tag == "block" ? stage->blocks : stage->samplers).push_back(std::move(r));
        } else if (tag == "input" || tag == "output") {
            if (!stage || !Fields(rest, v, 1, &name))
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
            if (!Fields(rest, v, 3, nullptr) || static_cast<std::size_t>(v[0]) != packages.size() ||
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
