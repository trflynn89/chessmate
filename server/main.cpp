#include <chrono>
#include <thread>

#include <fly/system/system.h>

#include <chessmate.h>

//==============================================================================
int main()
{
    chessmate::ChessMatePtr spChessMate(
        std::make_shared<chessmate::ChessMate>(true)
    );

    if (spChessMate && spChessMate->Start())
    {
        while (fly::System::KeepRunning())
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        spChessMate->Stop();
    }

    return fly::System::GetExitCode();
}
