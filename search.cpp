#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <queue>
#include <vector>
#include <mutex>
#include <algorithm>
#include <sstream>
#include <omp.h>

namespace fs = std::filesystem;

enum class SearchMode{EXACT, CASE_INSENSITIVE, FUZZY};

std::mutex cout_mutex;

bool file_contains_word_fuzzy(const fs::path& file_path, const std::string& word, int threshold);
bool file_contains_word(const fs::path& file_path, const std::string& word, SearchMode mode);
bool line_matches(const std::string& line, const std::string& word, SearchMode mode , const fs::path& file_path);
int levenshtein_distance(const std::string& s1, const std::string& s2);


SearchMode parse_mode_flag(const std::string& flag) {
    if (flag == "--mode=case") return SearchMode::CASE_INSENSITIVE;
    if (flag == "--mode=fuzzy") return SearchMode::FUZZY;
    return SearchMode::EXACT; // default
}

int main(int argc, char* argv[]) {
    freopen("log.txt", "w", stdout);

    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <root_path> <word> [--mode=exact|case|fuzzy]\n";
        return 1;
    }

    fs::path root_path = argv[1];
    std::string word = argv[2];
    SearchMode mode = SearchMode::EXACT;

    if (argc >= 4) {
        mode = parse_mode_flag(argv[3]);
    }

    if (!fs::exists(root_path)) {
        std::cerr << "Path does not exist!\n";
        return 1;
    }

    std::queue<fs::path> path_queue;
    path_queue.push(root_path);

    std::cout << "Files containing the word \"" << word << "\" \n\n";

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

            if (file_contains_word(file_path, word , mode)) {
                std::ostringstream match_info;
                match_info << "Thread " << thread_id << " found word in file: " << file_path << "\n";
                match_info << "--> Inside Directory: " << file_path.parent_path() << "\n\n";
            
                std::lock_guard<std::mutex> lock(cout_mutex);
                std::cout << match_info.str();      // goes to log.txt
                std::cerr << match_info.str();      // also shown on CLI
            }
        }
    }

    return 0;
}


bool line_matches(const std::string& line, const std::string& word, SearchMode mode, const fs::path& file_path) {
    switch (mode) {
        case SearchMode::EXACT:
            return line.find(word) != std::string::npos;
        case SearchMode::CASE_INSENSITIVE: {
            std::string lower_line = line;
            std::string lower_word = word;
            std::transform(lower_line.begin(), lower_line.end(), lower_line.begin(), ::tolower);
            std::transform(lower_word.begin(), lower_word.end(), lower_word.begin(), ::tolower);
            return lower_line.find(lower_word) != std::string::npos;
        }
        case SearchMode::FUZZY: {
            return file_contains_word_fuzzy(file_path, word, 2); // Example threshold
        }
        default:
            return false;
    }
}

std::string clean_word(const std::string& word) {
    std::string cleaned_word;
    for (char c : word) {
        if (std::isalnum(c)) {
            cleaned_word += c;
        }
    }
    return cleaned_word;
}

int levenshtein_distance(const std::string& s1, const std::string& s2) {
    const size_t len1 = s1.size(), len2 = s2.size();
    std::vector<std::vector<int>> d(len1 + 1, std::vector<int>(len2 + 1));

    for (size_t i = 0; i <= len1; ++i) d[i][0] = i;
    for (size_t j = 0; j <= len2; ++j) d[0][j] = j;

    for (size_t i = 1; i <= len1; ++i)
        for (size_t j = 1; j <= len2; ++j)
            d[i][j] = std::min({ 
                d[i - 1][j] + 1,
                d[i][j - 1] + 1,
                d[i - 1][j - 1] + (s1[i - 1] == s2[j - 1] ? 0 : 1)
            });

    return d[len1][len2];
}
// 
bool file_contains_word_fuzzy(const fs::path& file_path, const std::string& word, int threshold) {
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string token;
        while (iss >> token) {
            if (levenshtein_distance(clean_word(token), clean_word(word)) <= threshold) {
                return true;
            }
        }
    }
    return false;
}

bool file_contains_word(const fs::path& file_path, const std::string& word, SearchMode mode) {
    std::ifstream file(file_path);
    std::string line;
    while (std::getline(file, line)) {
        if (line_matches(line, word, mode, file_path)) {
            return true;
        }
    }
    return false;
}