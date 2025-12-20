#include "ObjLoader.h"

#include <fstream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

static std::string TrimPathString(const std::string& s)
{
    size_t start = 0;
    while (start < s.size() && (s[start] == ' ' || s[start] == '\t' || s[start] == '\n' || s[start] == '\r')) start++;

    size_t end = s.size();
    while (end > start && (s[end - 1] == ' ' || s[end - 1] == '\t' || s[end - 1] == '\n' || s[end - 1] == '\r')) end--;

    std::string out = s.substr(start, end - start);

    // Strip surrounding quotes
    if (out.size() >= 2 && ((out.front() == '"' && out.back() == '"') || (out.front() == '\'' && out.back() == '\'')))
    {
        out = out.substr(1, out.size() - 2);
    }

    return out;
}

bool PrepareObjPathForLoading(const string& inputPath, string& outPath)
{
    namespace fs = std::filesystem;

    try
    {
        const std::string trimmed = TrimPathString(inputPath);
        if (trimmed.empty()) return false;

        fs::path src(trimmed);
        if (src.is_relative())
        {
            src = fs::current_path() / src;
        }

        // weakly_canonical avoids throwing if some parts don't exist
        src = fs::weakly_canonical(src);
        if (!fs::exists(src)) return false;

        fs::path objectsDir = fs::current_path() / "Objects";
        objectsDir = fs::weakly_canonical(objectsDir);
        fs::create_directories(objectsDir);

        // If src is already inside Objects/, don't copy.
        {
            std::error_code ec;
            fs::path srcCanon = fs::weakly_canonical(src, ec);
            fs::path objCanon = fs::weakly_canonical(objectsDir, ec);
            if (!ec)
            {
                auto srcStr = srcCanon.generic_string();
                auto objStr = objCanon.generic_string();
                if (!objStr.empty() && objStr.back() != '/') objStr.push_back('/');

                if (srcStr.rfind(objStr, 0) == 0)
                {
                    fs::path rel = fs::relative(srcCanon, fs::current_path(), ec);
                    outPath = ec ? (fs::path("Objects") / src.filename()).string() : rel.string();
                    return true;
                }
            }
        }

        // Copy into Objects/<filename>
        fs::path dest = objectsDir / src.filename();

        // If dest exists and is the same file, do nothing.
        if (fs::exists(dest))
        {
            std::error_code ec;
            if (fs::equivalent(src, dest, ec) && !ec)
            {
                outPath = (fs::path("Objects") / src.filename()).string();
                return true;
            }

            // Backup destination once: Objects/<name>_backup.obj
            fs::path backup = dest.parent_path() / (dest.stem().string() + "_backup" + dest.extension().string());
            if (!fs::exists(backup))
            {
                fs::copy_file(dest, backup, fs::copy_options::overwrite_existing);
            }
        }

        // Read entire source before writing destination (prevents accidental truncation bugs)
        std::ifstream in(src, std::ios::binary);
        if (!in.is_open()) return false;
        std::string bytes((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
        in.close();

        std::ofstream out(dest, std::ios::binary | std::ios::trunc);
        if (!out.is_open()) return false;
        out.write(bytes.data(), (std::streamsize)bytes.size());
        out.close();

        outPath = (fs::path("Objects") / src.filename()).string();
        return true;
    }
    catch (...)
    {
        return false;
    }
}

static bool ParseObjVertexIndexToken(const std::string& token, int vertexCount, int& outIndex)
{
    if (token.empty()) return false;

    // Face tokens can look like:
    //   v
    //   v/vt
    //   v//vn
    //   v/vt/vn
    // We only care about the first number (v).
    const size_t slashPos = token.find('/');
    const std::string indexStr = (slashPos == std::string::npos) ? token : token.substr(0, slashPos);
    if (indexStr.empty()) return false;

    int idx = 0;
    try {
        idx = std::stoi(indexStr);
    } catch (...) {
        return false;
    }

    // OBJ indices are 1-based. Negative indices are relative to the end.
    if (idx > 0)
    {
        outIndex = idx - 1;
    }
    else if (idx < 0)
    {
        outIndex = vertexCount + idx;
    }
    else
    {
        return false;
    }

    return outIndex >= 0 && outIndex < vertexCount;
}

OBJ::OBJ(const string& path)
{
    filePath = path;
    ifstream MyReadFile(path);
    string line;

    if (!MyReadFile.is_open()) return;

    while (getline(MyReadFile, line))
    {
        // Ignore empty lines / comments
        if (line.empty()) continue;
        if (line[0] == '#') continue;

        istringstream ss(line);
        string type;
        ss >> type;
        if (type.empty()) continue;

        if (type == "v")
        {
            float x, y, z;
            ss >> x >> y >> z;
            if (ss.fail()) continue;
            vertices.push_back({ x, y, z });
        }
        else if (type == "f")
        {
            vector<int> face;
            string token;
            while (ss >> token)
            {
                int idx = -1;
                if (ParseObjVertexIndexToken(token, (int)vertices.size(), idx))
                {
                    face.push_back(idx);
                }
            }

            if (face.size() >= 3)
            {
                faces.push_back(face);
            }
        }
    }

    // create edges for wireframe
    for (int j = 0; j < (int)faces.size(); j++)
    {
        const vector<int>& f = faces[j];
        for (int i = 0; i < (int)f.size(); i++)
        {
            int a = f[i];
            int b = f[(i + 1) % f.size()];
            edges.push_back({ a, b });
        }
    }
}

// fan triangulation: convert polygon to triangles
Mesh OBJ::ToMesh()
{
    Mesh mesh;
    mesh.vertices = vertices;

    // triangulate each face using fan method
    for (int i = 0; i < (int)faces.size(); i++)
    {
        const vector<int>& face = faces[i];
        if (face.size() < 3) continue;

        // fan triangulation: connect first vertex to all others
        for (int j = 1; j < (int)face.size() - 1; j++)
        {
            Triangle tri(face[0], face[j], face[j + 1]);
            mesh.triangles.push_back(tri);
        }
    }

    mesh.ComputeMeshCenter();
    return mesh;
}

// load obj file directly into mesh
bool LoadOBJFile(const string& path, Mesh& outMesh)
{
    OBJ obj(path);
    if (obj.vertices.empty()) return false;

    Mesh loaded = obj.ToMesh();
    if (loaded.vertices.empty() || loaded.triangles.empty()) return false;

    outMesh = std::move(loaded);
    return true;
}
