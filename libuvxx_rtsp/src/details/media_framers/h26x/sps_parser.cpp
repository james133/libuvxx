#include <stdexcept>
#include "details/media_framers/h26x/sps_parser.hpp"

using namespace uvxx::rtsp::details::media_framers::h26x;

#pragma warning(push, 3)

sps_parser::sps_parser(const std::vector<uint8_t>& sequence_parameter_set) :
    _video_width(0),
    _video_height(0)
{

    const size_t MIN_SPS_BUFFER_SIZE_IN_BITS = 88 /* min bits required to read metadata */;

    size_t buffer_size = sequence_parameter_set.size() * 8 /* convert to bits */;

    if (!buffer_size || buffer_size < MIN_SPS_BUFFER_SIZE_IN_BITS)
    {
        return;
    }

    _buffer = sequence_parameter_set.data();

    _buffer_length = sequence_parameter_set.size();

    _current_bit = 0;

    try
    {
        parse_sps();
    }
    catch (std::out_of_range&)
    {
        
    }
}

int sps_parser::video_width() const
{
    return _video_width;
}

int sps_parser::video_height() const
{
    return _video_height;
}

unsigned int sps_parser::read_bit()
{
    if (!(_current_bit <= _buffer_length * 8))
    {
        throw std::out_of_range("sps parser does not have enough bits to parse");
    }

    int index = _current_bit / 8;

    int offset = _current_bit % 8 + 1;

    _current_bit++;

    return (_buffer[index] >> (8 - offset)) & 0x01;
}

unsigned int sps_parser::read_bits(int n)
{
    int r = 0;

    int i;

    for (i = 0; i < n; i++)
    {
        r |= (read_bit() << (n - i - 1));
    }

    return r;
}

unsigned int sps_parser::read_exponential_golomb_code()
{
    int r = 0;

    int i = 0;

    while ((read_bit() == 0) && (i < 32))
    {
        i++;
    }

    r = read_bits(i);

    r += (1 << i) - 1;

    return r;
}

unsigned int sps_parser::read_se()
{
    int r = read_exponential_golomb_code();

    if (r & 0x01)
    {
        r = (r + 1) / 2;
    }
    else
    {
        r = -(r / 2);
    }

    return r;
}

void sps_parser::parse_sps()
{
    int frame_crop_left_offset = 0;

    int frame_crop_right_offset = 0;

    int frame_crop_top_offset = 0;

    int frame_crop_bottom_offset = 0;

    int forbidden = read_bit();

    int nal_ref_idc = read_bits(2);

    int nal_unit_type = read_bits(5);

    int profile_idc = read_bits(8);

    if (nal_unit_type != 7)
    {
        return;
    }

    int constraint_set0_flag = read_bit();

    int constraint_set1_flag = read_bit();

    int constraint_set2_flag = read_bit();

    int constraint_set3_flag = read_bit();

    int constraint_set4_flag = read_bit();

    int constraint_set5_flag = read_bit();

    int reserved_zero_2bits = read_bits(2);

    int level_idc = read_bits(8);

    int seq_parameter_set_id = read_exponential_golomb_code();

    if (profile_idc == 100 || profile_idc == 110 ||
        profile_idc == 122 || profile_idc == 244 ||
        profile_idc == 44  || profile_idc == 83  ||
        profile_idc == 86  || profile_idc == 118)
    {
        int chroma_format_idc = read_exponential_golomb_code();

        if (chroma_format_idc == 3)
        {
            int residual_colour_transform_flag = read_bit();
        }

        int bit_depth_luma_minus8 = read_exponential_golomb_code();

        int bit_depth_chroma_minus8 = read_exponential_golomb_code();

        int qpprime_y_zero_transform_bypass_flag = read_bit();

        int seq_scaling_matrix_present_flag = read_bit();

        if (seq_scaling_matrix_present_flag)
        {
            int i = 0;

            for (i = 0; i < 8; i++)
            {
                int seq_scaling_list_present_flag = read_bit();

                if (seq_scaling_list_present_flag)
                {
                    int sizeOfScalingList = (i < 6) ? 16 : 64;

                    int lastScale = 8;

                    int nextScale = 8;

                    int j = 0;

                    for (j = 0; j < sizeOfScalingList; j++)
                    {
                        if (nextScale != 0)
                        {
                            int delta_scale = read_se();

                            nextScale = (lastScale + delta_scale + 256) % 256;
                        }

                        lastScale = (nextScale == 0) ? lastScale : nextScale;
                    }
                }
            }
        }
    }

    int log2_max_frame_num_minus4 = read_exponential_golomb_code();

    int pic_order_cnt_type = read_exponential_golomb_code();

    if (pic_order_cnt_type == 0)
    {
        int log2_max_pic_order_cnt_lsb_minus4 = read_exponential_golomb_code();
    }
    else if (pic_order_cnt_type == 1)
    {
        int delta_pic_order_always_zero_flag = read_bit();

        int offset_for_non_ref_pic = read_se();

        int offset_for_top_to_bottom_field = read_se();

        int num_ref_frames_in_pic_order_cnt_cycle = read_exponential_golomb_code();

        for (int i = 0; i < num_ref_frames_in_pic_order_cnt_cycle; i++)
        {
            read_se();
        }
    }

    int max_num_ref_frames = read_exponential_golomb_code();

    int gaps_in_frame_num_value_allowed_flag = read_bit();

    int pic_width_in_mbs_minus1 = read_exponential_golomb_code();

    int pic_height_in_map_units_minus1 = read_exponential_golomb_code();

    int frame_mbs_only_flag = read_bit();

    if (!frame_mbs_only_flag)
    {
        int mb_adaptive_frame_field_flag =  read_bit();
    }

    int direct_8x8_inference_flag = read_bit();

    int frame_cropping_flag = read_bit();

    if (frame_cropping_flag)
    {
        frame_crop_left_offset = read_exponential_golomb_code();

        frame_crop_right_offset = read_exponential_golomb_code();

        frame_crop_top_offset = read_exponential_golomb_code();

        frame_crop_bottom_offset = read_exponential_golomb_code();
    }

    int vui_parameters_present_flag = read_bit();

    _video_width = ((pic_width_in_mbs_minus1 + 1) * 16) -
                    (frame_crop_right_offset * 2 - frame_crop_left_offset * 2);

    _video_height = ((2 - frame_mbs_only_flag)* (pic_height_in_map_units_minus1 + 1) * 16) -
                    (frame_crop_top_offset * 2) - (frame_crop_bottom_offset * 2);
}
#pragma warning(pop)