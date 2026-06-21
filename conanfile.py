from conan import ConanFile


class SHiP_Framework(ConanFile):
    name = "SHiP"
    settings = "os", "compiler", "build_type", "arch"
    requires = (
        "mlpack/[>=4.3 <5]",
        "armadillo/[>=12.0 <13]",
        "hdf5/[>=1.14 <2]",
        "pybind11/[>=2.11 <3]",
        "cnpy/cci.20180601",
        "fmt/[>=11 <12]",
        "simdjson/[>=3.10 <4]",
        # "llvm-openmp/18.1.8",
        ## Math libraries ##
        # "onetbb/2022.0.0",
        # "eigen/3.4.0",
        # "xtensor/0.25.0",
        # "blaze/3.8.2",
        # "openblas/0.3.25",
    )

    # default_options = {
    #     "openblas/*:build_cblas": True
    # }

    generators = "CMakeToolchain", "CMakeDeps"
