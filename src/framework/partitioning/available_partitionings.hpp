#pragma once

#include <array>
#include <string>
#include <utility>
#include <vector>


// PartitioningMethod definition
enum class PartitioningMethod {
    K,
    Elbow,
    Threshold,
    ThresholdElbow,
    QCoverage,
    QCoverageElbow,
    QStem,
    QStemElbow,
    LcaNoiseElbow,
    LcaNoiseElbowNoTriangle,
    MedianOfElbows,
    MeanOfElbows,
    Stability,
    NormalizedStability,
    // PythonFunction,
};


// Helper: Array of PartitioningMethod-string pairs
constexpr std::array<std::pair<PartitioningMethod, std::string_view>, 14> partitioningMethodStrings{{
    {PartitioningMethod::K, "K"},
    {PartitioningMethod::Elbow, "Elbow"},
    {PartitioningMethod::Threshold, "Threshold"},
    {PartitioningMethod::ThresholdElbow, "ThresholdElbow"},
    {PartitioningMethod::QCoverage, "QCoverage"},
    {PartitioningMethod::QCoverageElbow, "QCoverageElbow"},
    {PartitioningMethod::QStem, "QStem"},
    {PartitioningMethod::QStemElbow, "QStemElbow"},
    {PartitioningMethod::LcaNoiseElbow, "LcaNoiseElbow"},
    {PartitioningMethod::LcaNoiseElbowNoTriangle, "LcaNoiseElbowNoTriangle"},
    {PartitioningMethod::MedianOfElbows, "MedianOfElbows"},
    {PartitioningMethod::MeanOfElbows, "MeanOfElbows"},
    {PartitioningMethod::Stability, "Stability"},
    {PartitioningMethod::NormalizedStability, "NormalizedStability"},
    // {PartitioningMethod::PythonFunction, "PythonFunction"},
}};


// From PartitioningMethod to string
std::string partitioning_method_to_string(PartitioningMethod type);

// From string to PartitioningMethod
PartitioningMethod string_to_partitioning_method(const std::string& type);

// Get all available PartitioningMethods as strings
std::vector<PartitioningMethod> get_available_partitioning_methods();

// Get all available PartitioningMethods as strings
std::vector<std::string> get_available_partitioning_methods_as_strings();
