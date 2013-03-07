#!/usr/bin/env python

from distutils.core import setup
from catkin_pkg.python_setup import generate_distutils_setup

d = generate_distutils_setup(
  scripts=['scripts/tracker_node.py'],
  packages=['indoor_tracker'],
  package_dir={'': 'src'}
)

setup(**d)
