#pragma once

#include <array>
#include <string>
#include <utility>
#include <vector>


/// \brief Available methods for partitioning a hierarchy into flat clusters.
enum class PartitioningMethod {
    /// Partition into a fixed number of clusters $k$.
    K,

    /// Use the Elbow method to determine the optimal number of clusters.
    Elbow,

    /// Cut the hierarchy at a fixed similarity threshold.
    Threshold,

    /// Combine threshold cutting and the Elbow method for more adaptive partitioning.
    ThresholdElbow,

    /// Use $q$-coverage to ensure that a given proportion of mass is retained in the clustering.
    QCoverage,

    /// Combine $q$-coverage and the Elbow method for adaptive and coverage-aware partitioning.
    QCoverageElbow,

    /// Use the $q$-stem criterion to extract partitions based on internal branch structure.
    QStem,

    /// Combine $q$-stem with Elbow-based refinement for improved robustness.
    QStemElbow,

    /// Elbow method that incorporates noise suppression using least common ancestors (LCA).
    LcaNoiseElbow,

    /// Noise-aware Elbow method that excludes triangular merging to improve sharpness.
    LcaNoiseElbowNoTriangle,

    /// Take the median of multiple Elbow criteria to find a more stable clustering.
    MedianOfElbows,

    /// Take the mean of multiple Elbow criteria to smooth over noise in the hierarchy.
    MeanOfElbows,

    /// Optimize cluster stability across multiple resolutions to find a robust partitioning, see also HDBSCAN.
    Stability,

    /// Use normalized cluster stability to allow comparisons across hierarchies of different sizes.
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
