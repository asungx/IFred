#pragma once

// Standard headers
#include <string>

// Vendor headers
#include "nlohmann/json.hpp"
#include "xeus/xinterpreter.hpp"

using nlohmann::json;

namespace ifred
{
    class interpreter : public xeus::xinterpreter
    {
    public:
        interpreter() = default;
        ~interpreter() = default;

    private:
        void configure_impl() override;

        json execute_request_impl(
            int execution_counter,
            const std::string& code,
            bool silent,
            bool store_history,
            json user_expressions,
            bool allow_stdin
        ) override;

        json complete_request_impl(const std::string& code, int cursor_pos) override;

        json inspect_request_impl(
            const std::string& code,
            int cursor_pos,
            int detail_level
        ) override;

        json is_complete_request_impl(const std::string& code) override;

        json kernel_info_request_impl() override;

        void shutdown_request_impl() override;

        json internal_request_impl(const json& message) override;
    };
}
