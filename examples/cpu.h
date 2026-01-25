#pragma once
#include <map>
#include <string>
#include <vector>
class CPUDog {
  public:
    CPUDog(const std::string file_path);
    std::string display_utilization();

  private:
    // Member Variables
    const std::string m_file_path;
    int m_num_cpus;
    int m_num_keys;
    std::map<std::string, std::vector<u_int64_t>> m_cur_data_map;
    std::map<std::string, std::vector<u_int64_t>> m_prev_data_map;
    std::map<std::string, float> m_cpu_utilization_map;

    // Methods
    std::map<std::string, std::vector<u_int64_t>> stat_query();

    std::map<std::string, float>
    calculate_cpu_utilization(std::map<std::string, std::vector<u_int64_t>> &cur_data_map,
                              std::map<std::string, std::vector<u_int64_t>> &prev_data_map);

    std::string render_utilization(std::map<std::string, float> &cpu_utilization_map);
    std::string progress_bar(float util_percent);
    std::string get_color(float util_percent);
};
