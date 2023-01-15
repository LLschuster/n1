#include <chrono>

class Timer {
    public:
        Timer(const char *name): m_name(name), stopped(false) {
            timeStart = std::chrono::high_resolution_clock::now();
        }

        ~Timer(){
            if (!stopped){
                stopTimer();
            }
        }

        void stopTimer(){
            auto timeEnd = std::chrono::high_resolution_clock::now();

            int64 timeStartInMicro = std::chrono::time_point_cast<std::chrono::microseconds>(timeStart).time_since_epoch().count();
            int64 timeEndInMicro = std::chrono::time_point_cast<std::chrono::microseconds>(timeEnd).time_since_epoch().count();

            stopped = true;

            float duration = (timeEndInMicro - timeStartInMicro) * 0.001f;
            std::cout << "timer: " << m_name << " duration: " << duration << "ms\n";
        }
    private:
        const char *m_name;
        bool stopped;
        std::chrono::time_point<std::chrono::high_resolution_clock> timeStart;
};