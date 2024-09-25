# Configuration file for the Sphinx documentation builder.
#
# For the full list of built-in configuration values, see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Project information -----------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#project-information

project = 'Tea Engine'
copyright = '2024, Hugo Planell Moreno, Mario Dorado Martínez'
author = 'Hugo Planell Moreno, Mario Dorado Martínez'
release = '0.1.0'

# -- General configuration ---------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#general-configuration

extensions = [ "breathe" ]

templates_path = ['_templates']
exclude_patterns = ['_build', 'Thumbs.db', '.DS_Store']



# -- Options for HTML output -------------------------------------------------
# https://www.sphinx-doc.org/en/master/usage/configuration.html#options-for-html-output

html_theme = 'sphinx_rtd_theme'
html_static_path = [] # Do not include static files in the build directory (_static), it is not necessary and will output a warning


# Breathe Configuration
breathe_projects = {
    "TeaEngine": "./xml"
}

breathe_default_project = "TeaEngine"

html_theme_options = {
    "sidebar_hide_name": False,  # Mostrar el nombre del proyecto en la barra lateral
    "navigation_with_keys": True,  # Habilitar la navegación con teclas
}