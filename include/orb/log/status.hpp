#pragma once

// STDLIB
#include <cstdint>
#include <string>
#include <string_view>

namespace logging
{

class LoadingBar
{
public:
    LoadingBar(std::string_view name, float percent, std::string_view description, bool deleteBar);
    // Void needs to be there for some reason, idk why
    ~LoadingBar(void);

    void UpdateBar(float percent, std::string_view description);
    void UpdatePercent(float percent);
    void UpdateDescription(std::string_view description);

private:
    float percent;
    std::string name;
    std::string description;
    int rowNum;
    bool deleteBar;

    static constexpr uint16_t barLength = 20;
    void Draw();
};

} // namespace logging
