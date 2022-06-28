// Standard headers
#include <cstdlib>
#include <filesystem>
#include <memory>
#include <string>

// Vendor headers
#include "CLI/CLI.hpp"
#include "fmt/core.h"
#include "spdlog/spdlog.h"
#include "xeus/xkernel.hpp"
#include "xeus/xserver_zmq.hpp"

// Internal headers
#include "interpreter.hpp"

using interpreter_ptr = std::unique_ptr<ifred::interpreter>;
using xeus_context = std::unique_ptr<xeus::xcontext_impl<zmq::context_t>>;

int main(int argc, char* argv[])
{
    CLI::App app{ "IFred" };

    //-------------------------------------------------------------------------
    // Options
    std::string connection_file{ "connection.json" };
    app.add_option(
        "-f,--connection_file",
        connection_file,
        fmt::format("Specify the kernel connection file (default: {})", connection_file)
    );

    //-------------------------------------------------------------------------

    // Parse command line arguments
    try
    {
        app.parse(argc, argv);
    }
    catch (const CLI::ParseError &e)
    {
        return app.exit(e);
    }

    // Find connection file
    if ( !std::filesystem::exists(connection_file) )
    {
        std::string cwd = std::filesystem::current_path().string();
        connection_file = fmt::format("{}/{}", cwd, connection_file);
        if ( !std::filesystem::exists(connection_file) )
        {
            spdlog::error("Cannot find connection file: {}", connection_file);
            std::exit(-1);
        }
    }
    spdlog::info("Connection file: {}", connection_file);

    // Load connection file
    xeus::xconfiguration config = xeus::load_configuration(connection_file);

    // Create kernel instance and start it
    xeus_context context = xeus::make_context<zmq::context_t>();
    interpreter_ptr interpreter = interpreter_ptr(new ifred::interpreter());
    xeus::xkernel kernel(
        config,
        xeus::get_user_name(),
        std::move(context),
        std::move(interpreter),
        xeus::make_xserver_zmq
    );
    kernel.start();

    return 0;
}
