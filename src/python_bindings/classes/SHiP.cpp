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
inline void bind_SHiP(py::module_ &m) {
    py::class_<SHiP>(m, "SHiP", py::dynamic_attr(), R"pbdoc(
    SHiP clustering class.

    Constructs a SHiP object from input data.
)pbdoc")

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
             py::arg("data"), py::arg("treeType") = DEFAULT_ULTRAMETRIC_TREE_TYPE, py::arg("power") = DEFAULT_POWER, py::arg("partitioningMethod") = DEFAULT_PARTITIONING_METHOD, py::arg("config") = std::unordered_map<std::string, std::string>{}, R"pbdoc(
                Constructs a SHiP clustering object.

                Parameters
                ----------
                data : list[list[float]] or numpy.ndarray
                    The dataset to be clustered, either as a nested list of floats
                    or a 2D NumPy array (shape: ``(n_samples, n_features)``).

                treeType : Union[UltrametricTreeType, str], optional
                    The ultrametric tree type used to construct the hierarchical structure.
                    Can be specified as an enum member (e.g., ``UltrametricTreeType.DCTree``)
                    or as a string (e.g., ``"DCTree"``). Defaults to ``DCTree``.

                power : int, optional
                    Power parameter used when transforming the similarity structure.
                    Controls how distances are scaled. Default is ``2``.

                partitioningMethod : Union[PartitioningMethod, str], optional
                    Partitioning strategy to extract flat clusters from the hierarchy.
                    Can be given as an enum (e.g., ``PartitioningMethod.Elbow``) or
                    as a string (e.g., ``"Elbow"``). Defaults to ``Elbow``.

                config : dict[str, str], optional
                    Additional configuration options, passed as a dictionary of string keys
                    and values. Can be used to tweak internal parameters.

                Notes
                -----
                This constructor allows users to initialize SHiP in a highly flexible way.
                All inputs are validated and cast internally to their proper representations.
                )pbdoc")


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
             py::arg("power") = py::none(), py::arg("partitioningMethod") = py::none(), py::arg("config") = std::unordered_map<std::string, std::string>{}, R"pbdoc(
        Apply clustering by extracting a partition from the computed hierarchy.

        This method applies a specified partitioning method (or the default) to produce a flat clustering
        from the internal ultrametric tree and associated hierarchy.

        Parameters
        ----------
        power : int, optional
            Power exponent used for hierarchy construction. If not provided, the value from initialization is used.

        partitioningMethod : PartitioningMethod or str, optional
            The strategy used to extract a flat partition from the hierarchy. Can be a member of the
            `PartitioningMethod` enum or a corresponding string. If `None`, the default is used.

        config : dict[str, str], optional
            Configuration dictionary for advanced or backend-specific parameters.

        Returns
        -------
        list[int]
            Cluster labels assigned to each data point.

        Raises
        ------
        ValueError
            If the partitioning method is of an invalid type.

        Notes
        -----
        This function assumes the SHiP instance has already built a tree and hierarchy internally.
    )pbdoc")


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
             py::arg("power") = py::none(), py::arg("partitioningMethod") = py::none(), py::arg("config") = std::unordered_map<std::string, std::string>{}, R"pbdoc(
        Fit the SHiP model and return cluster labels.

        This method performs all necessary steps to build the similarity tree, generate a clustering hierarchy,
        and extract a flat clustering using the specified partitioning method.

        Parameters
        ----------
        power : int, optional
            Power exponent used in the hierarchy construction phase. If not provided, uses the default or value
            passed during initialization.

        partitioningMethod : PartitioningMethod or str, optional
            Partitioning strategy used to flatten the hierarchy into clusters. Accepts either a `PartitioningMethod`
            enum value or a corresponding string. If not provided, the default is used.

        config : dict[str, str], optional
            Dictionary of advanced or backend configuration options.

        Returns
        -------
        list[int]
            A list of cluster labels corresponding to the input data points.

        Raises
        ------
        ValueError
            If an invalid `partitioningMethod` is provided.

        Notes
        -----
        This is a convenience method equivalent to calling `.fit(...)` followed by `.labels_`.
    )pbdoc")


        // get_tree
        .def("get_tree", &SHiP::get_tree, py::arg("power") = 0,
             R"pbdoc(
           Retrieve the similarity tree constructed for a given power.
   
           Parameters
           ----------
           power : int, optional
               The power exponent used when computing the ultrametric tree. If set to `0`, uses the default power.
   
           Returns
           -------
           Tree
               The ultrametric tree representation (internal C++ structure or Python-wrapped equivalent)
               used in the SHiP clustering hierarchy.
   
           Notes
           -----
           This method is useful for inspecting or debugging the tree structure before or after
           clustering. Depending on the backend, it may return a tree object or a data structure
           that represents the tree in a format suitable for downstream analysis.
        )pbdoc");
}
