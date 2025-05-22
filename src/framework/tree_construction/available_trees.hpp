#pragma once

#include <array>
#include <string>
#include <utility>
#include <vector>


// UltrametricTreeType definition
enum class UltrametricTreeType {
    LoadTree,
    DCTree,
    HST,
    CoverTree,
    KDTree,
    MeanSplitKDTree,
    BallTree,
    MeanSplitBallTree,
    RPTree,
    MaxRPTree,
    UBTree,
    RTree,
    RStarTree,
    XTree,
    HilbertRTree,
    RPlusTree,
    RPlusPlusTree,
};


// Helper: Array of UltrametricTreeType-string pairs
constexpr std::array<std::pair<UltrametricTreeType, std::string_view>, 17> ultrametricTreeTypeStrings = {{
    {UltrametricTreeType::LoadTree, "LoadTree"},
    {UltrametricTreeType::DCTree, "DCTree"},
    {UltrametricTreeType::HST, "HST"},
    {UltrametricTreeType::CoverTree, "CoverTree"},
    {UltrametricTreeType::KDTree, "KDTree"},
    {UltrametricTreeType::MeanSplitKDTree, "MeanSplitKDTree"},
    {UltrametricTreeType::BallTree, "BallTree"},
    {UltrametricTreeType::MeanSplitBallTree, "MeanSplitBallTree"},
    {UltrametricTreeType::RPTree, "RPTree"},
    {UltrametricTreeType::MaxRPTree, "MaxRPTree"},
    {UltrametricTreeType::UBTree, "UBTree"},
    {UltrametricTreeType::RTree, "RTree"},
    {UltrametricTreeType::RStarTree, "RStarTree"},
    {UltrametricTreeType::XTree, "XTree"},
    {UltrametricTreeType::HilbertRTree, "HilbertRTree"},
    {UltrametricTreeType::RPlusTree, "RPlusTree"},
    {UltrametricTreeType::RPlusPlusTree, "RPlusPlusTree"},
}};


// From UltrametricTreeType to string
std::string ultrametric_tree_type_to_string(UltrametricTreeType type);

// From string to UltrametricTreeType
UltrametricTreeType string_to_ultrametric_tree_type(const std::string& type);

// Get all available UltrametricTreeTypes
std::vector<UltrametricTreeType> get_available_ultrametric_tree_types();

// Get all available UltrametricTreeTypes as strings
std::vector<std::string> get_available_ultrametric_tree_types_as_strings();
