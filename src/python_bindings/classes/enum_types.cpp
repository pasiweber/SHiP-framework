#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <framework/partitioning/available_partitionings.hpp>
#include <framework/tree_construction/available_trees.hpp>
#include <pch.hpp>

namespace py = pybind11;


void bind_UltrametricTreeTypes(py::module_ &m) {
    py::module ultrametric_tree_submodule = m.def_submodule("ultrametric_tree");

    // UltrametricTreeTypes
    auto ultrametricTreeTypes = py::enum_<UltrametricTreeType>(ultrametric_tree_submodule, "UltrametricTreeType");
    for (auto const &[type, name] : ultrametricTreeTypeStrings) {
        ultrametricTreeTypes.value(name.data(), type);
    }
    ultrametricTreeTypes.export_values();

    ultrametricTreeTypes.def(py::init([](const std::string &name) {
        return string_to_ultrametric_tree_type(name);
    }));
    py::implicitly_convertible<std::string, UltrametricTreeType>();

    // Provide a list of available UltrametricTreeTypes
    ultrametric_tree_submodule.attr("AVAILABLE_ULTRAMETRIC_TREE_TYPES") = py::cast(get_available_ultrametric_tree_types());
    ultrametric_tree_submodule.attr("AVAILABLE_ULTRAMETRIC_TREE_TYPES_AS_STRINGS") = py::cast(get_available_ultrametric_tree_types_as_strings());

    // Provide converter functions
    ultrametric_tree_submodule.def("stringToUltrametricTreeType",
                                   string_to_ultrametric_tree_type,
                                   "Convert a string to a UltrametricTreeType enum.");
    ultrametric_tree_submodule.def("ultrametricTreeTypeToString",
                                   ultrametric_tree_type_to_string,
                                   "Convert a UltrametricTreeType enum to a string.");
}


void bind_PartitioningMethods(py::module_ &m) {
    py::module partitioning_submodule = m.def_submodule("partitioning");

    // PartitioningMethods
    auto partitioningMethods = py::enum_<PartitioningMethod>(partitioning_submodule, "PartitioningMethod");
    for (auto const &[type, name] : partitioningMethodStrings) {
        partitioningMethods.value(name.data(), type);
    }
    partitioningMethods.export_values();

    partitioningMethods.def(py::init([](const std::string &name) {
        return string_to_partitioning_method(name);
    }));
    py::implicitly_convertible<std::string, PartitioningMethod>();

    // Provide a list of available PartitioningMethods
    partitioning_submodule.attr("AVAILABLE_PARTITIONING_METHODS") = py::cast(get_available_partitioning_methods());
    partitioning_submodule.attr("AVAILABLE_PARTITIONING_METHODS_AS_STRINGS") = py::cast(get_available_partitioning_methods_as_strings());

    // Provide converter functions
    partitioning_submodule.def("stringToPartitioningMethod",
                               &string_to_partitioning_method,
                               "Convert a string to a PartitioningMethod enum.");
    partitioning_submodule.def("partitioningMethodToString",
                               &partitioning_method_to_string,
                               "Convert a PartitioningMethod enum to a string.");
}
