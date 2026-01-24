#include "devel.h"
#include <cmath>
#include <format>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <sys/types.h>
#include <unistd.h>

// Constructor
DevelDog::DevelDog(const std::string file_path) : m_file_path(file_path) {
    m_num_cpus = 0;
    m_num_keys = 0;
    m_cur_data_map = {};
    m_prev_data_map = {};
    m_cpu_utilization_map = {};
}

std::string DevelDog::display_utilization() {
    if (m_cur_data_map.empty()) {
        m_cur_data_map = stat_query();
        for (auto it = m_cur_data_map.begin(); it != m_cur_data_map.end(); ++it) {
            ++m_num_keys;
        }
        m_num_cpus = m_num_keys - 1;
    }

    std::cout << std::endl;
    // sleep(sleep_time);
    m_prev_data_map = m_cur_data_map;
    m_cur_data_map = stat_query();
    m_cpu_utilization_map = calculate_cpu_utilization(m_cur_data_map, m_prev_data_map);

    return (render_utilization(m_cpu_utilization_map));
}

std::string DevelDog::render_utilization(std::map<std::string, float> &cpu_utilization_map) {
    std::string out_string = "";

    for (int i = 0; i < m_num_cpus; ++i) {
        out_string += std::to_string(i) + "\t" +
                      progress_bar(cpu_utilization_map[std::format("{}", i)]) + "\n";
    }
    out_string += "Total\t" + progress_bar(cpu_utilization_map["Total"]) + "\n";

    return out_string;
}

std::string DevelDog::progress_bar(float util_percent) {
    std::string prog_char = "|";

    int bar_width = 50;
    int num_prog_chars = static_cast<int>(std::round((util_percent / 100) * bar_width));

    std::string prog_bar = "[";
    std::string invisible_color = "\e[8m";
    std::string end_color = "\e[0m";

    std::string color = get_color(util_percent);

    for (int i = 0; i < bar_width; ++i) {
        if (i < num_prog_chars) {
            prog_bar.append(color + prog_char + end_color);
        } else {
            prog_bar.append(invisible_color + prog_char + end_color);
        }
    }

    // Convert util_percent to a formatted float before adding to output string
    std::string s_util_percent = std::format(
        "{:^6.2f}%", util_percent); // ^6.2f means center-align in width of 6 to 2 decimal places
    prog_bar.append("] " + color + s_util_percent + end_color);

    return prog_bar;
}

std::string DevelDog::get_color(float util_percent) {
    // Clamp the value to ensure it's never negative
    if (util_percent < 0) {
        util_percent = 0;
    }

    // Convert to a "bucket" index (0, 1, 2, 3+)
    int bucket = static_cast<int>(std::floor(util_percent / 25.0f));

    switch (bucket) {
    case 0:
        return "\e[38;5;46m"; // 0-24.99 (Green)
    case 1:
        return "\e[38;5;190m"; // 25-49.99 (Yellow)
    case 2:
        return "\e[38;5;208m"; // 50-74.99 (Orange)
    default:
        return "\e[38;5;196m"; // 75+ (Red)
    }
}

std::map<std::string, std::vector<u_int64_t>> DevelDog::stat_query() {
    std::ifstream file(m_file_path);
    std::string file_line;
    std::vector<std::string> data_vec;
    std::map<std::string, std::vector<u_int64_t>> data_map;

    if (!file.is_open()) {
        throw std::runtime_error("File cannot be opened: " + m_file_path);
    }

    while (std::getline(file, file_line)) {
        data_vec.emplace_back(file_line);
    }

    for (const auto &line : data_vec) {
        std::stringstream ss(line);
        std::string key;

        ss >> key;
        if (key == "cpu") {
            key = "Total";
            data_map[key] = {};
            continue;
        } else if (key.starts_with("cpu")) {
            std::string to_remove = "cpu";
            size_t pos = key.find(to_remove);
            if (pos != std::string::npos) {
                key.erase(pos, to_remove.length());
            }
        } else {
            continue;
        }

        // Process the rest of the stringstream
        std::string s_value;
        u_int64_t value;
        std::vector<u_int64_t> cpu_times;
        while (ss >> s_value) {
            value = std::stoll(s_value);
            cpu_times.emplace_back(value);
        }

        // Insert the key and cpu_times into the map
        data_map[key] = cpu_times;
    }

    return data_map;
}

std::map<std::string, float>
DevelDog::calculate_cpu_utilization(std::map<std::string, std::vector<u_int64_t>> &cur_data_map,
                                    std::map<std::string, std::vector<u_int64_t>> &prev_data_map) {

    std::map<std::string, float> cpu_utilization_map;
    std::map<std::string, std::vector<float>> delta_map;
    float all_cpus_total = 0.0f;

    // Iterate over current data map, and calculate deltas in relation to
    // equivalent previous data map keys
    for (auto it = cur_data_map.begin(); it != cur_data_map.end(); ++it) {
        const std::string &key = it->first;
        std::vector<u_int64_t> cur_data_vec = it->second;
        std::vector<u_int64_t> prev_data_vec = prev_data_map[key];
        float util_percent = 0.0f;

        if (key == "Total") {
            cpu_utilization_map[key] = all_cpus_total / m_num_cpus;
        } else {
            float cur_total = static_cast<float>(
                cur_data_vec[0] + cur_data_vec[1] + cur_data_vec[2] + cur_data_vec[3] +
                cur_data_vec[4] + cur_data_vec[5] + cur_data_vec[6] + cur_data_vec[7] +
                cur_data_vec[8] + cur_data_vec[9]);
            float prev_total = static_cast<float>(
                prev_data_vec[0] + prev_data_vec[1] + prev_data_vec[2] + prev_data_vec[3] +
                prev_data_vec[4] + prev_data_vec[5] + prev_data_vec[6] + prev_data_vec[7] +
                prev_data_vec[8] + prev_data_vec[9]);
            float cur_idle = static_cast<float>(cur_data_vec[3]);
            float prev_idle = static_cast<float>(prev_data_vec[3]);
            float delta_total = cur_total - prev_total;
            float delta_idle = cur_idle - prev_idle;

            if (delta_total > 0.0f) {
                util_percent = std::abs(((delta_total - delta_idle) / delta_total) * 100);
            } else {
                util_percent = 0.0f;
            }

            cpu_utilization_map[key] = util_percent;
            all_cpus_total += util_percent;
        }
    }
    return cpu_utilization_map;
}
