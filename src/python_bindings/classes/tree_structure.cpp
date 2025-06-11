#include <pybind11/numpy.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <framework/tree_structure.hpp>
#include <pch.hpp>

namespace py = pybind11;


inline void bind_Node(py::module_ &m) {
    py::class_<Node, std::shared_ptr<Node>>(m, "Node")
        // Expose fields
        .def_readonly("id", &Node::id)
        .def_readonly("cost", &Node::cost)
        .def_readonly("children", &Node::children)
        .def_readonly("parent", &Node::parent)
        .def_readonly("size", &Node::size)
        .def_readonly("low", &Node::low)
        .def_readonly("high", &Node::high)
        .def_readonly("level", &Node::level)

        // Comparison operators
        .def("__lt__", [](const Node &self, const Node &other) { return self < other; })
        .def("__gt__", [](const Node &self, const Node &other) { return self > other; })

        // to_json method
        .def("to_json", &Node::to_json,
             py::arg("fast_index") = false,
             "Serialize the node to JSON, optionally including fast indexing information.");
}

inline void bind_Tree(py::module_ &m) {
    py::class_<Tree, std::shared_ptr<Tree>>(m, "Tree")

        // Expose fields (read-write access)
        .def_readonly("root", &Tree::root)
        .def_readonly("tree_type", &Tree::tree_type)
        .def_readonly("power", &Tree::power)
        .def_readonly("config", &Tree::config)
        .def_readonly("index_order", &Tree::index_order)
        .def_readonly("sorted_nodes", &Tree::sorted_nodes)
        .def_readonly("costs", &Tree::costs)
        .def_readonly("cost_decreases", &Tree::cost_decreases)

        // Expose method
        .def("get_elbow_k", &Tree::get_elbow_k, py::arg("triangle") = true, "Get elbow `k`.")
        .def("to_json", &Tree::to_json,
             py::arg("fast_index") = false,
             "Serialize the tree to JSON, optionally including fast indexing information.");
}
