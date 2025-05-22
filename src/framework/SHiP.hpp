#pragma once

#include <framework/partitioning/available_partitionings.hpp>
#include <framework/tree_construction/available_trees.hpp>
#include <framework/tree_structure.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


const UltrametricTreeType DEFAULT_ULTRAMETRIC_TREE_TYPE = UltrametricTreeType::DCTree;
const std::string DEFAULT_ULTRAMETRIC_TREE_TYPE_STRING = ultrametric_tree_type_to_string(DEFAULT_ULTRAMETRIC_TREE_TYPE);
const long long DEFAULT_POWER = 1;
const PartitioningMethod DEFAULT_PARTITIONING_METHOD = PartitioningMethod::MedianOfElbows;
const std::string DEFAULT_PARTITIONING_METHOD_STRING = partitioning_method_to_string(DEFAULT_PARTITIONING_METHOD);
const std::unordered_map<std::string, std::string> DEFAULT_CONFIG = {};



class SHiP {
private:
    std::vector<std::vector<double>> data;
    std::unordered_map<long long, std::shared_ptr<Tree>> trees;

public:
    UltrametricTreeType tree_type;
    long long power;
    PartitioningMethod partitioning_method;

    // Config:
    // - UltramtricTrees:
    //   - DCTree: min_points (5), relaxed (false)
    //   - HST: seed (-1)
    //   - mlpack_trees: min_leaf_size (1), max_leaf_size (5)
    //   - LoadTree: json_tree_filepath, tree_type (optional)
    // - Partitionings:
    //   - K: k (2)
    //   - Threshold: k (2)
    //   - ThresholdElbow: -
    //   - QCoverage: k (2), min_cluster_size (5)
    //   - QCoverageElbow: min_cluster_size (5)
    //   - QStem: k (2), min_cluster_size (5)
    //   - QStemElbow: min_cluster_size (5)
    //   - Stability: min_cluster_size (5)
    //   - ElbowOld: -
    //   - MedianOfElbows: elbow_start_z (1), elbow_end_z (5)
    //   - MeanOfElbows: elbow_start_z (1), elbow_end_z (5)
    std::unordered_map<std::string, std::string> config;

    // Cluster Labels
    std::vector<long long> labels_;

    // Runtimes
    long long partitioning_runtime = 0;
    std::unordered_map<long long, long long> tree_construction_runtime;


    // Constructors
    // Original constructor
    SHiP(std::vector<std::vector<double>>& data,
         UltrametricTreeType tree_type = DEFAULT_ULTRAMETRIC_TREE_TYPE,
         long long power = DEFAULT_POWER,
         PartitioningMethod partitioning_method = DEFAULT_PARTITIONING_METHOD,
         const std::unordered_map<std::string, std::string>& config = DEFAULT_CONFIG);


    // Methods
    void fit(std::optional<long long> power = std::nullopt, std::optional<PartitioningMethod> partitioning_method = std::nullopt, const std::unordered_map<std::string, std::string>& config = {});

    std::vector<long long> fit_predict(std::optional<long long> power = std::nullopt, std::optional<PartitioningMethod> partitioning_method = std::nullopt, const std::unordered_map<std::string, std::string>& config = {});

    std::shared_ptr<Tree> get_tree(long long power = 0);


private:
    std::vector<long long> partitioning();

    std::shared_ptr<Tree> construct_base_tree(std::vector<std::vector<double>>& data);
};
