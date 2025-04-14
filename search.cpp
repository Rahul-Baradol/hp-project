#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include <omp.h>

namespace fs = std::filesystem;

std::mutex cout_mutex;

bool file_contains_word(const fs::path& file_path, const std::string& word) {
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        if (line.find(word) != std::string::npos) {
            return true;
        }
    }
    return false;
}

int main() {
    freopen("log.txt", "w", stdout);
    std::string root_path, word;
    std::cin >> root_path >> word;

    if (!fs::exists(root_path)) {
        std::cerr << "Path does not exist!\n";
        return 1;
    }

    std::queue<fs::path> path_queue;
    path_queue.push(root_path);

    std::cout << "Files containing the word \"" << word << "\n\n";

    omp_set_num_threads(4);
    
    #pragma omp parallel
    {
        #pragma omp single
        {
            std::cout << "Using " << omp_get_num_threads() << " threads.\n\n";
        }
    }

    while (!path_queue.empty()) {
        fs::path current = path_queue.front();
        path_queue.pop();

        std::vector<fs::path> files_to_check;

        {
            std::lock_guard<std::mutex> lock(cout_mutex);
            std::cout << "Processing directory: " << current << "\n\n";
        }

        for (const auto& entry : fs::directory_iterator(current)) {
            if (entry.is_directory()) {
                path_queue.push(entry.path());
            } else if (entry.is_regular_file()) {
                files_to_check.push_back(entry.path());
            }
        }

        #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < files_to_check.size(); ++i) {
            const auto& file_path = files_to_check[i];

            int thread_id = omp_get_thread_num();
            {
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << "Thread " << thread_id << " checking file: " << file_path << "\n\n";
            }

            if (file_contains_word(file_path, word)) {
                {
                    std::lock_guard<std::mutex> lock(cout_mutex);
                    std::cout << "Thread " << thread_id << " found word in file: " << file_path << "\n\n";
                }
            }
        }
    }

    return 0;
}
