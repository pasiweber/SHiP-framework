#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <framework/SHiP.hpp>
#include <framework/partitioning/available_partitionings.hpp>
#include <framework/tree_construction/available_trees.hpp>
#include <pch.hpp>

#include "helper.hpp"

namespace py = pybind11;

void bind_UltrametricTreeTypes(py::module &m);
void bind_PartitioningMethods(py::module &m);
void bind_SHiP(py::module &m);
void bind_Tree(py::module &m);
void bind_Node(py::module &m);


PYBIND11_MODULE(SHiP, m) {
    m.doc() = "Python bindings for the SHiP framework";


    ///// Enum Types /////
    bind_UltrametricTreeTypes(m);
    bind_PartitioningMethods(m);


    ///// Classes /////
    bind_SHiP(m);
    bind_Tree(m);
    bind_Node(m);


    ///// Logger Module /////
    py::module logger_submodule = m.def_submodule("logger");

    // LoggerTypes
    auto logLevels = py::enum_<LogLevel>(logger_submodule, "LogLevel");
    for (auto const &[type, name] : logLevelStrings) {
        logLevels.value(name.data(), type);
    }
    logLevels.export_values();

    // Provide a list of available UltrametricTreeTypes
    logger_submodule.def("setLogLevel",
                         py::overload_cast<LogLevel>(&::setLogLevel),
                         "Set the log level using a LogLevel enum");
    logger_submodule.def(
        "setLogLevel",
        [](const std::string &lvl) {
            setLogLevel(const_cast<std::string &>(lvl));
        },
        "Set the log level using a string name");
}
