#include "devel.h"
#include "terminal.h"
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
        Terminal term;
        DevelDog devel_dog = DevelDog(PROC_PATH);

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
            term.move_cursor(0, 0);
            term.write("Frames Processed: " + std::to_string(frame_count++));
            term.move_cursor(0, 1);
            term.write("Press 'q' to exit...");
            term.move_cursor(0, 2);
            sleep(1);
            term.write(devel_dog.display_utilization());
            term.flush();

            // Cap the speed to reduce CPU overutilization
            std::this_thread::sleep_for(std::chrono::milliseconds(16));
        }
    } catch (...) {
    }
    return 0;
}
