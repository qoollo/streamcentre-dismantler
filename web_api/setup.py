# -*- coding: utf-8 -*-
from setuptools import setup

setup(
    name='StreamCentre VideoSegmenter',
    version='0.1',
    long_description=__doc__,
    include_package_data=True,
    zip_safe=False,
    setup_requires=['Flask'],
    install_requires=['Flask', 'Flask-Script', 'requests', 'HTTPretty']
)
