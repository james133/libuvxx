#include "details/_event_dispatcher_impl.hpp"
#include "fs/details/_file_impl.hpp"
#include "fs/details/_static_file_impl.hpp"
#include "fs/file.hpp"

using namespace std;
using namespace uvxx::pplx;

namespace uvxx { namespace fs
{
    file::file() : __file_impl(std::make_shared<details::_file_impl>())
    {

    }

    file::file(file&& rhs) : event_dispatcher_object(std::move(rhs))
    {

    }

    file& file::operator=(file&& rhs)
    {
        __file_impl = std::move(__file_impl);
        return static_cast<file&>(event_dispatcher_object::operator=(std::move(rhs))); 
    }

    task<void> file::open_async(std::string const& file_name, std::ios_base::openmode mode) const
    {
        return __file_impl->open_async(file_name, mode);
    }

    task<void> file::close_async() const
    {
        return __file_impl->close_async();
    }

    task<size_t> file::read_async(io::memory_buffer const & buffer, size_t start_pos, size_t count, int64_t file_position) const
    {
        return __file_impl->read_async(buffer, start_pos, count, file_position);
    }

    task<size_t> file::write_async(io::memory_buffer const & buffer, size_t start_pos, size_t count, int64_t file_position) const
    {
        return __file_impl->write_async(buffer, start_pos, count, file_position);
    }

    task<size_t> file::write_async(const uint8_t* buffer, size_t buffer_size, size_t start_pos, size_t count, int64_t file_position) const
    {
         return __file_impl->write_async(buffer, buffer_size, start_pos, count, file_position);
    }

    int64_t file::file_position_get() const
    {
        return __file_impl->file_position_get();
    }

    void file::file_position_set(int64_t position) const
    {
        __file_impl->file_position_set(position);
    }

    task<void> file::delete_async(std::string const& file_name)
    {
        auto file_delete = std::make_shared<uvxx::fs::details::_file_delete_async>();

        return file_delete->delete_async(file_name).then([file_delete](task<void> t)
        {
            t.get();
        });
    }

    task<void> file::move_async(std::string const& source_file, std::string const& destination_file)
    {
        auto file_delete = std::make_shared<details::_file_move_async>();

        return file_delete->move_async(source_file, destination_file).then([file_delete](task<void> t)
        {
            t.get();
        });
    }

    task<file_info> file::get_file_info_async(std::string const& filename)
    {
        auto file_stat = std::make_shared<details::_uv_file_stat>();

        return file_stat->get_file_info_async(filename).
        then([file_stat](task<file_info> t)
        {
            return t.get();
        });
    }
}}