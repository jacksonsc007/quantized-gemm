#include <chrono>
#include <iostream>
#include <map>
#include <string>
#include <iomanip>

#define PROFILE_START(x) Profiler::getInstance().start(x)
#define PROFILE_START_FLOPS(x, y) Profiler::getInstance().start(x, y)
#define PROFILE_END(x) Profiler::getInstance().stop(x)

class Profiler {
   public:
    static Profiler& getInstance() {
        static Profiler instance;
        return instance;
    }

    void start(const std::string& section) { start_times[section] = std::chrono::high_resolution_clock::now(); }

    void start(const std::string& section, const long long section_flops) {
        start_times[section] = std::chrono::high_resolution_clock::now();
        if (flops.count(section) == 0)
            flops[section] = section_flops;
        else
            flops[section] += section_flops;
    }

    void reset() {
        start_times.clear();
        durations.clear();
        counts.clear();
        flops.clear();
    }

    void stop(const std::string& section) {
        auto end_time = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_times[section]).count();
        durations[section] += duration;
        counts[section]++;
    }

    void report_internal() const {
        std::cout << std::endl;
        std::cout << std::left << std::setw(80) << "Section" 
                  << std::setw(30) << "Total time(ms)" 
                  << std::setw(30) << "Average time(ms)" 
                  << std::setw(30) << "Count" 
                  << std::setw(30) << "GOPs" 
                //   << std::setw(30) << "Mem BD GBS" 
                  << std::endl;

        for (const auto& entry : durations) {
            std::cout << std::left << std::setw(80) << entry.first 
                      << std::setw(30) << std::fixed << std::setprecision(2) << (float)(entry.second) / 1000 
                      << std::setw(30) << std::fixed << std::setprecision(2) << (float)(entry.second / counts.at(entry.first)) / 1000 
                      << std::setw(30) << counts.at(entry.first);

            if (flops.count(entry.first) == 0)
                std::cout << std::setw(30) << "N/A";
            else
                std::cout << std::setw(30) << std::fixed << std::setprecision(2) << ((((float)flops.at(entry.first)) / (float)(entry.second)) / 1000.0);
            // TODO: imprrove. Resue the flops as the number elements to fetch to calcuate memory bandwidth
            // float total_bytes = (float)flops.at(entry.first) * (float) sizeof(float);
            // float total_gbs = total_bytes / 1024.0 / 1024.0 / 1024.0;
            // std::cout << std::setw(30) << std::fixed << std::setprecision(2) << (( (  total_gbs ) / (float)(entry.second / 1e6) ));
            
            

            std::cout << std::endl;
        }
    }
    // void report_internal() const {
    //     std::cout << "Section, Total time(ms), Average time(ms), Count, GOPs" << std::endl;
    //     for (const auto& entry : durations) {
    //         std::string row;
    //         row += entry.first + ", ";
    //         row += std::to_string((float)(entry.second) / 1000) + ", ";
    //         row += std::to_string((float)(entry.second / counts.at(entry.first)) / 1000) + ", ";
    //         if (flops.count(entry.first) == 0)
    //             row += std::to_string(counts.at(entry.first)) + ", N/A";
    //         else {
    //             row += std::to_string(counts.at(entry.first)) + ", ";
    //             // ops and microsecond
    //             row += std::to_string((((float)flops.at(entry.first)) / (float)(entry.second)) / 1000.0);
    //         }
    //         std::cout << row << std::endl;
    //     }
    // }

    void report() const {
#ifdef PROFILER
        report_internal();
#endif
    }

   private:
    Profiler() {}
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;

    std::map<std::string, std::chrono::high_resolution_clock::time_point> start_times;
    std::map<std::string, long long> flops;
    std::map<std::string, long long> durations;
    std::map<std::string, int> counts;
};
