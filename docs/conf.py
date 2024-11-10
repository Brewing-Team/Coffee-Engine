# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Coffee Engine'
copyright = '2024, Hugo Planell Moreno, Mario Dorado Martínez'
author = 'Hugo Planell Moreno, Mario Dorado Martínez'
release = '0.2.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [ "breathe" ]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
using_rtd_theme = True

html_theme_options = {
    "logo_only": True,
    "collapse_navigation": False,
}

html_context = {
    "display_github": True, # Integrate GitHub
    "github_user": "Brewing-Team", # Username
    "github_repo": "Coffee-Engine", # Repo name
    "github_version": "main", # Version
    "conf_py_path": "/docs/", # Path in the checkout to the docs root
}

html_static_path = ["_static"] # Do not include static files in the build directory (_static), it is not necessary and will output a warning

html_logo = "_static/Coffee-Docs.png"

html_css_files = [
    "custom.css",
]

html_js_files = [
    "custom.js",
]

# Breathe Configuration
breathe_projects = {
    "CoffeeEngine": "./docs/xml"
}

breathe_default_project = "CoffeeEngine"
