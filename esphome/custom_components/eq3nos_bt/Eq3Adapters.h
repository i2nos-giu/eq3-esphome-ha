#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include <chrono>
#include <ctime>

class Eq3InvalidDataException : public std::runtime_error {
public:
    explicit Eq3InvalidDataException(const std::string& msg)
        : std::runtime_error(msg) {}
};

// -----------------------------------------------
//  Eq3AwayTime
// -----------------------------------------------
class Eq3AwayTime {
public:
    static std::vector<uint8_t> fromDatetime(const std::tm& value);
    static std::tm toDatetime(const std::vector<uint8_t>& value);
};

// -----------------------------------------------
//  Eq3Duration
// -----------------------------------------------
class Eq3Duration {
public:
    static int encode(const std::chrono::seconds& value);
    static std::chrono::seconds decode(int value);
};

// -----------------------------------------------
//  Eq3ScheduleTime
// -----------------------------------------------
class Eq3ScheduleTime {
public:
    static int encode(int hour, int minute);
    static void decode(int value, int& hour, int& minute);
};

// -----------------------------------------------
//  Eq3Serial
// -----------------------------------------------
class Eq3Serial {
public:
    static std::vector<uint8_t> encode(const std::string& value);
    static std::string decode(const std::vector<uint8_t>& value);
};

// -----------------------------------------------
//  Eq3TemperatureOffset
// -----------------------------------------------
class Eq3TemperatureOffset {
public:
    static constexpr float EQ3_MIN_OFFSET = -3.5f;
    static constexpr float EQ3_MAX_OFFSET = 3.5f;
    static int encode(float value);
    static float decode(int value);
};

// -----------------------------------------------
//  Eq3Temperature
// -----------------------------------------------
class Eq3Temperature {
public:
    static constexpr float EQ3_OFF_TEMP = 4.5f;
    static constexpr float EQ3_ON_TEMP  = 30.5f;
    static int encode(float value);
    static float decode(int value);
};

// -----------------------------------------------
//  Eq3Time
// -----------------------------------------------
class Eq3Time {
public:
    static std::vector<uint8_t> encode(const std::tm& value);
    static std::tm decode(const std::vector<uint8_t>& value);
};


