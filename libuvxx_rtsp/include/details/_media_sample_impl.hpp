#pragma once
#include <memory>
#include <chrono>
#include <string>
#include <vector>

#include "_media_attributes_impl.h"

namespace uvxx { namespace rtsp { namespace details 
{
    class _media_sample_impl : public _media_attributes_impl
    {
    public:
        _media_sample_impl();

        _media_sample_impl(const _media_sample_impl&) = default;

        _media_sample_impl& operator=(const _media_sample_impl&) = default;

        virtual ~_media_sample_impl();

    public:
        int stream_number() const;

        void stream_number_set(int stream_number);

        size_t size() const;

        void size_set(size_t size);

        size_t capacity();

        void capacity_set(size_t size);

        const uint8_t* data() const;

        std::chrono::microseconds presentation_time() const;
       
        void presentation_time_set(std::chrono::microseconds presentation_time);

        bool is_truncated() const;

        void is_truncated_set(bool truncated);

        std::string codec_name() const;

        void codec_name_set(const std::string& codec_name);

    private:
        int _stream_number;
        
        size_t _size;

        bool _is_truncated;

        std::string _codec_name;

        std::vector<uint8_t> _buffer;

        std::chrono::microseconds _presentation_time;

    };
}}}