#include "interpreter.hpp"

namespace ifred
{
    void interpreter::configure_impl()
    {
        return;
    }

    json interpreter::execute_request_impl(
        int execution_counter,
        const std::string& code,
        bool silent,
        bool store_history,
        json user_expressions,
        bool allow_stdin
    )
    {
        json j;
        return j;
    }

    json interpreter::complete_request_impl(
        const std::string& code,
        int cursor_pos
    )
    {
        json j;
        return j;
    }

    json interpreter::inspect_request_impl(
        const std::string& code,
        int cursor_pos,
        int detail_level
    )
    {
        json j;
        return j;
    }

    json interpreter::is_complete_request_impl(const std::string& code)
    {
        json j;
        return j;
    }

    json interpreter::kernel_info_request_impl()
    {
        json j;
        return j;
    }

    void interpreter::shutdown_request_impl()
    {
        return;
    }

    json interpreter::internal_request_impl(const json& message)
    {
        json j;
        return j;
    }
}
