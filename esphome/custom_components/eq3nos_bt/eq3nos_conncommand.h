#pragma once

namespace esphome {
namespace eq3nos_bt {

enum class ConnCommandType {
    SEND_RAW,
    STATUS
};

struct ConnCommand {
    ConnCommandType type;
    uint8_t length;
    uint8_t data[16];   // spazio per un comando EQ3 (ne bastano 1–3 bytes)
};


/*
struct ConnCommand {
    ConnCommandType type;
    std::vector<uint8_t> raw;
    
    ConnCommand() = default;

    // costruttore RAW da vector
    ConnCommand(const std::vector<uint8_t>& data)
        : type(ConnCommandType::RAW), raw(data) {}

    // costruttore RAW da array + length
    ConnCommand(const uint8_t *data, size_t len)
        : type(ConnCommandType::RAW), raw(data, data + len) {}

    // costruttore STATUS
    ConnCommand(ConnCommandType t)
        : type(t) {}
};
*/

}  // namespace eq3nos_bt
}  // namespace esphome

