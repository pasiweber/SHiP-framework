import os
import sys
import tomllib

sys.path.insert(0, os.path.abspath("."))


# Project Details
with open("../../pyproject.toml", "rb") as f:
    config = tomllib.load(f)

project = config["project"]["name"]
copyright = config["project"]["license"]["text"]
author = config["project"]["authors"][0]["name"]
release = config["project"]["version"]


# Extensions
extensions = [
    "breathe",
    "sphinx.ext.autodoc",
    "sphinx_autodoc_typehints",
    "sphinx.ext.intersphinx",
    "sphinx_math_dollar",
    "sphinx.ext.napoleon",  # for Google/NumPy docstrings
    "sphinx.ext.mathjax",   # for Latex math mode
    "myst_parser",          # for Markdown support
]
breathe_projects = {"SHiP": "../doxygen/xml"}
breathe_default_project = "SHiP"
breathe_default_members = ("members",)
autodoc_typehints = "description"
# autosectionlabel_prefix_document = False
autodoc_default_options = {
    "members": True,
    "undoc-members": True,
    "member-order": "groupwise",
    "special-members": "__init__",
    # "no-module": True,
}
myst_enable_extensions = [
    "amsmath",     # optional, enables AMS math environments
    "dollarmath",  # enables $...$ and $$...$$
]
myst_heading_anchors = 3


html_theme = "furo"  # "pydata_sphinx_theme"
html_theme_options = {
    "source_repository": "https://github.com/pasiweber/SHiP-framework/",
    "source_branch": "main",
    "source_directory": "docs/source/",
}
html_static_path = ["_static"]
