#define IMAGE_READ
#include "../image/image.hpp"

#include <filesystem>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <algorithm>
#include <functional>
#include <cassert>


using ByteBuffer = MemoryBuffer<u8>;
namespace mb = memory_buffer;


namespace a2b
{
    namespace fs = std::filesystem;
    namespace img = image;
    using p32 = img::Pixel;
    

    template <typename T>
    using fn = std::function<T>;

    using convert = fn<ByteBuffer(ByteBuffer const&)>;
}


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


    std::string to_struct_str(std::vector<DataSize> const& data, std::string const& tag)
    {
        cstr tab = "    ";
        auto name = tag + "_sizes";

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


    static bool write_cpp_file(std::string code_str, fs::path const& dst_dir, std::string const& tag)
    {
        auto filename = tag + ".cpp";
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


    static bool validate_directories(fs::path const& src_dir, fs::path const& dst_dir)
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
        
        return true;
    }

}
}


namespace a2b
{
    /*inline bool assets_to_binary(cstr src_dir, cstr dst_dir, cstr tag)
    {
        if (!internal::validate_directories(src_dir, dst_dir))
        {
            return false;
        }

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
            
            bin_file.write((char*)buffer.data_, buffer.size_);
            total += buffer.size_;

            data.emplace_back(internal::file_to_var(entry), buffer.size_);

            printf("read %s \n", data.back().name.c_str());

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
        if (!internal::write_cpp_file(struct_str, dst_dir, std::string(tag) + "_sizes"))
        {
            printf("FAIL\n");
            return false;
        }
        printf("OK\n");

        return true;
    }*/


    inline bool assets_to_binary(cstr src_dir, cstr dst_dir, cstr tag, convert const& convert_bytes)
    {
        if (!internal::validate_directories(src_dir, dst_dir))
        {
            return false;
        }

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

            auto raw = internal::read_bytes(entry);
            auto buffer = convert_bytes(raw);
            
            bin_file.write((char*)buffer.data_, buffer.size_);
            total += buffer.size_;

            data.emplace_back(internal::file_to_var(entry), buffer.size_);

            printf("read %s \n", data.back().name.c_str());

            mb::destroy_buffer(raw);
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
        if (!internal::write_cpp_file(struct_str, dst_dir, std::string(tag) + "_sizes"))
        {
            printf("FAIL\n");
            return false;
        }
        printf("OK\n");

        return true;
    }
}





namespace a2b
{
    class AssetImageBuffer
    {
    private:

        static constexpr u32 s32 = sizeof(u32);

        // offsets
        static constexpr u32 bpp_ = 0; // bytes_per_pixel
        static constexpr u32 width_ = bpp_ + s32;
        static constexpr u32 height_ = width_ + s32;
        static constexpr u32 pixels_ = height_ + s32;        

    public:
        b8 ok = 0;

        u8* data = 0;

        u32& bpp() { return *(u32*)(data + bpp_); }
        u32& width() { return *(u32*)(data + width_); }
        u32& height() { return *(u32*)(data + height_); }
        u8* pixels() { return data + pixels_; }

        u32& bpp() const { return *(u32*)(data + bpp_); }
        u32& width() const { return *(u32*)(data + width_); }
        u32& height() const { return *(u32*)(data + height_); }
        u8* pixels() const { return data + pixels_; }

        ByteView to_span() { return { data, bpp() * width() * height() + 3 * s32 }; }
        ByteView to_span() const { return { data, bpp() * width() * height() + 3 * s32 }; }
    };



}


namespace a2b
{
namespace internal
{
    static AssetImageBuffer encode_asset_image(img::Image const& image)
    {
        auto s32 = sizeof(u32);
        auto bpp = sizeof(p32);
        auto data_len = image.width * image.height * bpp;
        auto meta_len = 3 * s32;
        auto n_bytes = data_len + meta_len;

        AssetImageBuffer im_buffer;
        im_buffer.ok = 0;

        ByteBuffer buffer;
        mb::create_buffer(buffer, n_bytes, "encode_asset_image");
        if (!buffer.ok)
        {
            return im_buffer;
        }

        im_buffer.data = buffer.data_;
        im_buffer.bpp() = bpp;
        im_buffer.width() = image.width;
        im_buffer.height() = image.height;

        auto src = span::make_view((u8*)image.data_, data_len);
        auto dst = span::make_view(im_buffer.pixels(), data_len);

        span::copy(src, dst);

        im_buffer.ok = 1;

        return im_buffer;
    }


    static AssetImageBuffer encode_asset_image(img::Image const& image, fn<u8(p32)> const& encode)
    {
        auto s32 = sizeof(u32);
        auto bpp = 1;
        auto data_len = image.width * image.height * bpp;
        auto meta_len = 3 * s32;
        auto n_bytes = data_len + meta_len;

        AssetImageBuffer im_buffer;
        im_buffer.ok = 0;

        ByteBuffer buffer;
        mb::create_buffer(buffer, n_bytes, "encode_asset_image");
        if (!buffer.ok)
        {
            return im_buffer;
        }

        im_buffer.data = buffer.data_;
        im_buffer.bpp() = bpp;
        im_buffer.width() = image.width;
        im_buffer.height() = image.height;

        auto src = span::make_view(image.data_, data_len);
        auto dst = span::make_view(im_buffer.pixels(), data_len);

        span::transform(src, dst, encode);

        im_buffer.ok = 1;

        return im_buffer;
    }


    static img::ImageView decode_asset_image32(ByteView const& bytes)
    {
        img::ImageView view{};

        AssetImageBuffer buffer{};
        buffer.data = bytes.data;

        if (buffer.bpp() != 4)
        {
            assert(false);
            return view;
        }

        auto data_len = buffer.bpp() * buffer.width() * buffer.height();
        if (data_len > bytes.length)
        {
            assert(false);
            return view;
        }

        view = img::make_view(buffer.width(), buffer.height(), (p32*)bytes.data);

        return view;
    }


    static img::GrayView decode_asset_image8(ByteView const& bytes)
    {
        img::GrayView view{};

        AssetImageBuffer buffer{};
        buffer.data = bytes.data;

        if (buffer.bpp() != 1)
        {
            assert(false);
            return view;
        }

        auto data_len = buffer.bpp() * buffer.width() * buffer.height();
        if (data_len > bytes.length)
        {
            assert(false);
            return view;
        }

        view = img::make_view(buffer.width(), buffer.height(), bytes.data);

        return view;
    }
}
}


namespace a2b
{
namespace internal
{
    class DirFiles
    {
    public:
        std::string dir_name;

        std::vector<DataSize> files;
    };


    static DirFiles read_files(fs::path const& dir, std::ofstream& bin)
    {
        DirFiles df;

        df.dir_name = dir.filename().string();

        for (auto const& entry : fs::directory_iterator(dir))
        {            
            auto p = entry.path();
            if (!fs::is_regular_file(p))
            {
                continue;
            }

            auto buffer = read_bytes(p);

            df.files.emplace_back(p.stem().string(), buffer.size_);
            
            bin.write((char*)buffer.data_, buffer.size_);

            mb::destroy_buffer(buffer);
        }

        return df;
    }


    static std::vector<DirFiles> save_directory(fs::path const& dir, std::ofstream& bin)
    {
        std::vector<DirFiles> items;

        for (auto const& entry : fs::directory_iterator(dir))
        {
            if (!fs::is_directory(entry))
            {
                continue;
            }

            items.push_back(read_files(entry, bin));
        }

        bin.close();

        return items;
    }


    static bool write_file_sizes(std::vector<DirFiles> const& data, fs::path const& dst_dir, std::string const& name)
    {
        auto tab = "    ";
        auto tabtab = "        ";
        std::ostringstream oss;

        oss
        << "const struct {\n\n";

        for (auto const& item : data)
        {
            oss
            << tab << "struct {\n";

            for (auto const& f : item.files)
            {
                oss
                << tabtab << "unsigned " << f.name << ";\n";
            }

            oss
            << tab << "} " << item.dir_name << ";\n\n";
        }

        oss
        << "}\n"
        << name << " = \n"
        << "{\n";

        for (auto const& item : data)
        {
            oss
            << tab << "{\n";

            for (auto const& f : item.files)
            {
                oss
                << tabtab << f.size << ",\n";
            }

            oss
            << tab << "},\n";
        }

        oss
        << "};\n";

        return internal::write_cpp_file(oss.str(), dst_dir, name);
    }
}
}


namespace a2b
{
    bool files_to_binary(fs::path const& src_dir, fs::path const& dst_dir, cstr tag)
    {
        if (!internal::validate_directories(src_dir, dst_dir))
        {
            return false;
        }

        std::ofstream bin_file(dst_dir / (std::string(tag) + ".bin"));
        
        auto data  = internal::save_directory(src_dir, bin_file);

        auto name = src_dir.filename().string() + "_sizes";

        return internal::write_file_sizes(data, dst_dir, name);        
    }
}