// Vendor headers
#include "xeus/xkernel.hpp"
#include "CLI/CLI.hpp"

int main(int argc, char* argv[])
{
    CLI::App app{ "IFred" };

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    return 0;
}
