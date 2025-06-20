/*
 * Copyright (c) 2023 - 2025 Chair for Design Automation, TUM
 * Copyright (c) 2025 Munich Quantum Software Company GmbH
 * All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Licensed under the MIT License
 */

#include "ir/Definitions.hpp"
#include "ir/Permutation.hpp"
#include "ir/operations/Control.hpp"

// These includes must be the first includes for any bindings code
// clang-format off
#include <pybind11/pybind11.h>
#include <pybind11/stl.h> // NOLINT(misc-include-cleaner)

#include <pybind11/attr.h>
#include <pybind11/cast.h>
#include <pybind11/operators.h>
#include <pybind11/pytypes.h>
// clang-format on

#include <iterator>
#include <sstream>

namespace mqt {

namespace py = pybind11;
using namespace pybind11::literals;

// NOLINTNEXTLINE(misc-use-internal-linkage)
void registerPermutation(py::module& m) {
  py::class_<qc::Permutation>(m, "Permutation")
      .def(py::init<>())
      .def(py::init([](const py::dict& p) {
             qc::Permutation perm;
             for (const auto& [key, value] : p) {
               perm[key.cast<qc::Qubit>()] = value.cast<qc::Qubit>();
             }
             return perm;
           }),
           "perm"_a, "Create a permutation from a dictionary.")
      .def("apply",
           py::overload_cast<const qc::Controls&>(&qc::Permutation::apply,
                                                  py::const_),
           "controls"_a)
      .def("apply",
           py::overload_cast<const qc::Targets&>(&qc::Permutation::apply,
                                                 py::const_),
           "targets"_a)
      .def("clear", [](qc::Permutation& p) { p.clear(); })
      .def("__getitem__",
           [](const qc::Permutation& p, const qc::Qubit q) { return p.at(q); })
      .def("__setitem__", [](qc::Permutation& p, const qc::Qubit q,
                             const qc::Qubit r) { p[q] = r; })
      .def("__delitem__",
           [](qc::Permutation& p, const qc::Qubit q) { p.erase(q); })
      .def("__len__", &qc::Permutation::size)
      .def("__iter__",
           [](const qc::Permutation& p) {
             return py::make_key_iterator(p.begin(), p.end());
           })
      .def(
          "items",
          [](const qc::Permutation& p) {
            return py::make_iterator(p.begin(), p.end());
          },
          py::keep_alive<0, 1>())
      .def(py::self == py::self) // NOLINT(misc-redundant-expression)
      .def(py::self != py::self) // NOLINT(misc-redundant-expression)
      .def(hash(py::self))
      .def("__str__",
           [](const qc::Permutation& p) {
             std::stringstream ss;
             ss << "{";
             for (auto it = p.cbegin(); it != p.cend(); ++it) {
               ss << it->first << ": " << it->second;
               if (std::next(it) != p.cend()) {
                 ss << ", ";
               }
             }
             ss << "}";
             return ss.str();
           })
      .def("__repr__", [](const qc::Permutation& p) {
        std::stringstream ss;
        ss << "Permutation({";
        for (auto it = p.cbegin(); it != p.cend(); ++it) {
          ss << it->first << ": " << it->second;
          if (std::next(it) != p.cend()) {
            ss << ", ";
          }
        }
        ss << "})";
        return ss.str();
      });
  py::implicitly_convertible<py::dict, qc::Permutation>();
}

} // namespace mqt
