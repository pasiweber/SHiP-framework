#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <framework/SHiP.hpp>
#include <framework/partitioning/available_partitionings.hpp>
#include <framework/tree_construction/available_trees.hpp>
#include <pch.hpp>

#include "../helper.hpp"

namespace py = pybind11;


///// SHiP Module /////
void bind_SHiP(py::module_ &m) {
    py::class_<SHiP>(m, "SHiP", py::dynamic_attr())

        // Expose public data members
        .def_readonly("treeType", &SHiP::tree_type,
                      "Which UltrametricTreeType to use.")
        .def_readwrite("power", &SHiP::power,
                       "Tree power (which z-hierarchy to use).")
        .def_property("partitioningMethod",
                      // getter
                      ([](SHiP &self) {
                          return self.partitioning_method;
                      }),
                      // setter
                      ([](SHiP &self, py::object py_pm) {
                          PartitioningMethod pm;
                          if (py::isinstance<PartitioningMethod>(py_pm)) {
                              pm = py_pm.cast<PartitioningMethod>();
                          } else if (py::isinstance<py::str>(py_pm)) {
                              pm = string_to_partitioning_method(py_pm.cast<std::string>());
                          } else {
                              LOG_ERROR << "`partitioningMethod` has invalid type: expected PartitioningMethod or str";
                              throw std::invalid_argument("`partitioningMethod` has invalid type: expected PartitioningMethod or str");
                          }
                          self.partitioning_method = pm;
                      }),
                      "Partitioning strategy for extracting a clustering.")
        .def_property("config",
                      ([](SHiP &self) {
                          return self.config;
                      }),
                      ([](SHiP &self, py::object cfg_obj) {
                          if (!py::isinstance<py::dict>(cfg_obj))
                              throw std::invalid_argument("config must be a dict");
                          self.config = py_dict_to_string_map(cfg_obj.cast<py::dict>());
                      }),
                      "Additional string-based options.")
        .def_readonly("labels_", &SHiP::labels_,
                      "Computed labels.")
        .def_readonly("partitioning_runtime", &SHiP::partitioning_runtime,
                      "Time spent in the partitioning step (µs).")
        .def_readonly("tree_construction_runtime", &SHiP::tree_construction_runtime,
                      "Construction times for the UltrametricTree and the z-hierarchies (µs).")

        // Couple object attributes to the config dictionary of the SHiP object
        .def("__setattr__", [](SHiP &self, const std::string &name, py::object py_value) {
            // 0. Check for internal attributes
            static const std::set<std::string> attributes = {
                "treeType",
                "power",
                "partitioningMethod",
                "config",
                "labels_",
                "partitioning_runtime",
                "tree_construction_runtime",
                "fit",
                "fit_predict",
                "get_tree",
            };
            if (attributes.find(name) == attributes.end()) {
                // 1. Insert value into config dict
                std::string value = py::str(py_value).cast<std::string>();
                self.config[name] = value;
            }

            // 2. Delegate to built-in setattr to actually store it
            py::module::import("builtins")
                .attr("object")
                .attr("__setattr__")(
                    py::cast(&self), name, py_value);
        })


        /// Constructors ///
        .def(py::init([](py::object py_data, py::object py_tt, long long power, py::object py_pm, const py::dict &py_cfg) {
                 std::vector<std::vector<double>> data;
                 if (py::isinstance<py::array>(py_data)) {
                     data = pyarray_to_vector2D(py_data);
                 } else {
                     // You know it’s a sequence, so let STL caster handle it:
                     data = py_data.cast<std::vector<std::vector<double>>>();
                 }

                 UltrametricTreeType tt;
                 if (py::isinstance<UltrametricTreeType>(py_tt)) {
                     tt = py_tt.cast<UltrametricTreeType>();
                 } else if (py::isinstance<py::str>(py_tt)) {
                     tt = string_to_ultrametric_tree_type(py_tt.cast<std::string>());
                 } else {
                     LOG_ERROR << "SHiP(treeType): expected UltrametricTreeType or str";
                     throw std::invalid_argument(
                         "SHiP(treeType): expected UltrametricTreeType or str");
                 }

                 PartitioningMethod pm;
                 if (py::isinstance<PartitioningMethod>(py_pm)) {
                     pm = py_pm.cast<PartitioningMethod>();
                 } else if (py::isinstance<py::str>(py_pm)) {
                     pm = string_to_partitioning_method(py_pm.cast<std::string>());
                 } else {
                     LOG_ERROR << "SHiP(partitioningMethod): expected PartitioningMethod or str";
                     throw std::invalid_argument(
                         "SHiP(partitioningMethod): expected PartitioningMethod or str");
                 }

                 return new SHiP(data,
                                 tt,
                                 power,
                                 pm,
                                 py_dict_to_string_map(py_cfg));
             }),
             py::arg("data"), py::arg("treeType") = DEFAULT_ULTRAMETRIC_TREE_TYPE, py::arg("power") = DEFAULT_POWER, py::arg("partitioningMethod") = DEFAULT_PARTITIONING_METHOD, py::arg("config") = std::unordered_map<std::string, std::string>{})


        /// Methods ///
        // fit
        .def("fit", ([](SHiP &self, py::object py_power, py::object py_pm, const py::dict &py_cfg) {
                 // Handle optional power
                 std::optional<long long> power;
                 if (!py_power.is_none()) {
                     power = py_power.cast<long long>();
                 }

                 // Handle optional partitioning method
                 std::optional<PartitioningMethod> pm;
                 if (!py_pm.is_none()) {
                     if (py::isinstance<PartitioningMethod>(py_pm)) {
                         pm = py_pm.cast<PartitioningMethod>();
                     } else if (py::isinstance<py::str>(py_pm)) {
                         pm = string_to_partitioning_method(py_pm.cast<std::string>());
                     } else {
                         LOG_ERROR << "SHiP(partitioningMethod): expected PartitioningMethod or str or None";
                         throw std::invalid_argument("SHiP(partitioningMethod): expected PartitioningMethod or str or None");
                     }
                 }

                 // Call C++ function with std::optional
                 return self.fit(power, pm, py_dict_to_string_map(py_cfg));
             }),
             py::arg("power") = py::none(), py::arg("partitioningMethod") = py::none(), py::arg("config") = std::unordered_map<std::string, std::string>{}, "Extract a clustering from the computed hierarchy with the given `partitioningMethod`.")

        // fit_predict
        .def("fit_predict", ([](SHiP &self, py::object py_power, py::object py_pm, const py::dict &py_cfg) {
                 // Handle optional power
                 std::optional<long long> power;
                 if (!py_power.is_none()) {
                     power = py_power.cast<long long>();
                 }

                 // Handle optional partitioning method
                 std::optional<PartitioningMethod> pm;
                 if (!py_pm.is_none()) {
                     if (py::isinstance<PartitioningMethod>(py_pm)) {
                         pm = py_pm.cast<PartitioningMethod>();
                     } else if (py::isinstance<py::str>(py_pm)) {
                         pm = string_to_partitioning_method(py_pm.cast<std::string>());
                     } else {
                         LOG_ERROR << "SHiP(partitioningMethod): expected PartitioningMethod or str or None";
                         throw std::invalid_argument("SHiP(partitioningMethod): expected PartitioningMethod or str or None");
                     }
                 }

                 return self.fit_predict(power, pm, py_dict_to_string_map(py_cfg));
             }),
             py::arg("power") = py::none(), py::arg("partitioningMethod") = py::none(), py::arg("config") = std::unordered_map<std::string, std::string>{}, "Fit the model and return cluster labels.")

        // get_tree
        .def("get_tree", &SHiP::get_tree, py::arg("power") = 0, "Get the tree hierarchy for a specific power.");
}
