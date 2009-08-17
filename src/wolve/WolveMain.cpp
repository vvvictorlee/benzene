//----------------------------------------------------------------------------
/** @file WolveMain.cpp
 */
//----------------------------------------------------------------------------

#include "SgSystem.h"

#include "config.h"
#include "HexProgram.hpp"
#include "PlayerFactory.hpp"
#include "WolveEngine.hpp"
#include "WolvePlayer.hpp"

using namespace benzene;

namespace {

const char* build_date = __DATE__;

}

//----------------------------------------------------------------------------

int main(int argc, char** argv)
{
    HexProgram& program = HexProgram::Get();
    program.SetInfo("Wolve", VERSION, build_date);
    program.PrintStartupMessage();
    program.Initialize(argc, argv);

    boost::scoped_ptr<BenzenePlayer> 
        player(PlayerFactory::CreatePlayerWithBook(new WolvePlayer()));

    try
    {
        WolveEngine gh(std::cin, std::cout, program.BoardSize(), *player);
    
        std::string config = program.ConfigFileToExecute();
        if (config != "")
            gh.ExecuteFile(config);
        gh.MainLoop();
    
        program.Shutdown();
    }
    catch (const GtpFailure& f)
    {
        std::cerr << f.Response() << std::endl;
        return 1;
    }

    return 0;
}

//----------------------------------------------------------------------------
