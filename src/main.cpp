#include "cpu.h"
#include "devel.h"
#include <chrono>
#include <string>
#include <thread>
#include <unistd.h>

// The Purpose of this application is to provide an in-terminal System monitor
// comparable to top, htop, btop, etc.
// The file which will inform the initial monitoring is /proc/stat

int main() {
    const std::string PROC_PATH = "/proc/stat";

    try {
        DevelDog dd = DevelDog();
        CPUDog cpu_dog = CPUDog(PROC_PATH);

        bool running = true;
        int frame_count = 0;
        char c;

        while (running) {
            // Input handling (non-blocking)
            // read() returns -1 immediately if no key is pressed
            if (read(STDIN_FILENO, &c, 1) == 1) {
                if (c == 'q')
                    running = false;
            }
            dd.move_cursor(0, 0);
            dd.write("Frames Processed: " + std::to_string(frame_count++));
            dd.move_cursor(0, 1);
            dd.write("Press 'q' to exit...");
            dd.move_cursor(0, 2);
            sleep(1);
            dd.write(cpu_dog.display_utilization());
            dd.flush();

            // Cap the speed to reduce CPU overutilization
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    } catch (...) {
    }
    return 0;
}
