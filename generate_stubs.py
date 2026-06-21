# generate_stubs.py
import subprocess
import sys

subprocess.run(
    [
        sys.executable,
        "-m",
        "pybind11_stubgen",
        "SHiP-framework",
        "--enum-class-locations",
        "UltrametricTreeType:SHiP.ultrametric_tree",
        "--enum-class-locations",
        "PartitioningMethod:SHiP.partitioning_method",
        "-o",
        ".",
    ],
    check=True,
)
