#include "../../libs/util/memory_buffer.hpp"

#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <cassert>

namespace fs = std::filesystem;
namespace mb = memory_buffer;

using ByteBuffer = MemoryBuffer<u8>;


namespace a2b
{
namespace internal
{
    static fs::path out_bin_path(cstr out_dir, cstr tag)
    {
        return fs::path(out_dir) / (std::string(tag) + ".bin");
    }


    static bool is_file(fs::path const& entry)
    {
        return fs::is_regular_file(entry) && entry.has_filename() && entry.has_extension();
    }


    static std::string file_to_var(fs::path const& filepath)
    {
        auto str = std::string(filepath.filename());
        std::replace(str.begin(), str.end(), '.', '_');

        return str;
    }


    static ByteBuffer read_bytes(fs::path const& path)
    {
        ByteBuffer buffer;

        auto size = fs::file_size(path);
        if (size == 0 || !mb::create_buffer(buffer, size, ""))
        {
            assert(false);
            return buffer;
        }

        std::ifstream file(path, std::ios::binary);
        if (!file.is_open())
        {
            assert(false);
            mb::destroy_buffer(buffer);
            return buffer;
        }

        file.read((char*)buffer.data_, size);
        buffer.size_ = size;

        file.close();
        return buffer;
    }
    

    class DataSize    
    {
    public:
        std::string name;
        u32 size;

        DataSize(std::string&& n, u32 s)
        {
            name = std::move(n);
            size = s;
        }
    };


    std::string to_struct_str(std::vector<DataSize> const& data, cstr tag)
    {
        cstr tab = "    ";
        auto name = std::string(tag) + "_sizes";

        std::ostringstream oss;

        oss
        << "static constexpr struct\n"
        << "{\n";

        for (auto const& item : data)
        {
            oss << tab << "unsigned " << item.name << ";\n";
        }

        oss
        << "}\n"
        << name << " = \n"
        << "{\n";

        for (auto const& item : data)
        {
            oss << tab << item.size << ",\n";
        }

        oss
        << "};\n";

        return oss.str();
    }


    static bool write_cpp_file(std::string code_str, cstr dst_dir, cstr tag)
    {
        auto filename = std::string(tag) + "_sizes.cpp";
        auto filepath = fs::path(dst_dir) / filename;
    
        std::ofstream file(filepath);
    
        if (!file.is_open())
        {
            return false;
        }

        file << "// " << filename << "\n\n";    
        file << code_str;
    
        file.close();
    
        return true;
    }

}
}


namespace a2b
{
    inline bool assets_to_binary(cstr src_dir, cstr dst_dir, cstr tag)
    {
        printf("check src directory: ");
        if (!fs::exists(src_dir) || !fs::is_directory(src_dir))
        {
            printf("FAIL\n");
            return false;
        }
        printf("OK\n");

        printf("check dst directory: ");
        if (!fs::exists(dst_dir) || !fs::is_directory(dst_dir))
        {
            printf("FAIL\n");
            return false;
        }
        printf("OK\n");
        
        printf("open bin file: ");
        auto bin_path = internal::out_bin_path(dst_dir, tag);
        std::ofstream bin_file(bin_path);
        if (!bin_file.is_open())
        {
            printf("FAIL\n");
            return false;
        }
        printf("OK\n");

        std::vector<internal::DataSize> data;
        u32 total = 0;
        for (auto const& entry : fs::directory_iterator(src_dir))
        {
            if (!internal::is_file(entry))
            {
                continue;
            }

            auto buffer = internal::read_bytes(entry);

            data.emplace_back(internal::file_to_var(entry), buffer.size_);
            bin_file.write((char*)buffer.data_, buffer.size_);
            total += buffer.size_;

            mb::destroy_buffer(buffer);
        }

        bin_file.close();

        auto file_size = fs::file_size(bin_path);

        printf("bin size: ");
        if (file_size != total)
        {
            printf("FAIL\n");
        }
        else
        {
            printf("OK | %u bytes\n", total);
        }

        auto struct_str = to_struct_str(data, tag);

        printf("sizes: ");
        if (!internal::write_cpp_file(struct_str, dst_dir, tag))
        {
            printf("FAIL\n");
            return false;
        }
        printf("OK\n");

        return true;
    }
}