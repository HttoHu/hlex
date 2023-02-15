#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

namespace Utils
{
    std::string read_file(const std::string &filename);
    
    struct FileSlice
    {
        std::string content;
        bool is_mutable = false;
    };

    struct Slicer
    {
        Slicer(const std::string &file_content);

        std::string &get_mut(const std::string &idx_name);
        std::string merge() const;

        std::vector<FileSlice> slices;
        std::map<std::string, int> mut_idxs;
    };

}