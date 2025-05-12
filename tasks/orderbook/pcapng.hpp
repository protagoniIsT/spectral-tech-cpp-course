#pragma once

#include "types.hpp"
#include <cstdint>
#include <cstring>
#include <string_view>
#include <vector>

const uint32_t ENHANCED_PACKET_BLOCK_TYPE = 0x00000006;
const size_t BLOCK_HEADER_LEN = 8;
const size_t BLOCK_MIN_TOTAL_LEN = 12;
const size_t EPB_HEADER_LEN = 20;
const size_t EPB_CAP_LEN_OFFSET = 12;

const size_t ETHERNET_HEADER_LEN = 14;
const size_t IPV4_MIN_HEADER_LEN = 20;
const size_t UDP_HEADER_LEN  = 8;
constexpr size_t FIXED_HEADERS_LEN = ETHERNET_HEADER_LEN + IPV4_MIN_HEADER_LEN + UDP_HEADER_LEN;

inline std::vector<Message> Parse(std::string_view buf) {
    std::vector<Message> result;
    size_t i = 0, n = buf.size();

    auto read_u32 = [&](size_t pos) -> uint32_t {
        uint32_t v;
        std::memcpy(&v, buf.data() + pos, sizeof(v));
        return v;
    };

    while (i + BLOCK_HEADER_LEN <= n) {
        uint32_t block_type = read_u32(i);
        uint32_t total_block_len = read_u32(i + 4);
        if (total_block_len < BLOCK_MIN_TOTAL_LEN || i + total_block_len > n) {
            break;
        }

        if (block_type == ENHANCED_PACKET_BLOCK_TYPE) {
            size_t header_start = i + BLOCK_HEADER_LEN;
            if (header_start + EPB_HEADER_LEN > i + total_block_len) {
                i += total_block_len;
                continue;
            }

            uint32_t cap_len = read_u32(header_start + EPB_CAP_LEN_OFFSET);
            size_t payload_start = header_start + EPB_HEADER_LEN;
            if (payload_start + cap_len > i + total_block_len) {
                i += total_block_len;
                continue;
            }

            if (cap_len >= FIXED_HEADERS_LEN + sizeof(Message)) {
                size_t p = payload_start;
                p += ETHERNET_HEADER_LEN;
                uint8_t ihl = static_cast<uint8_t>(buf[p]) & 0x0F;
                p += ihl * 4;
                p += UDP_HEADER_LEN;

                Message m;
                std::memcpy(&m, buf.data() + p, sizeof(m));
                result.push_back(m);
            }
        }

        i += total_block_len;
    }

    return result;
}
