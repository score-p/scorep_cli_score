import setuptools
import subprocess
import sys

if sys.version_info < (3, 5):
    sys.exit('Python 3.4 or older is not supported.')

scorep_include_dir = subprocess.check_output(["scorep-config", "--cppflags"]).decode('utf-8').split()[0][2:]
scorep_library_dir = subprocess.check_output(["scorep-config", "--ldflags"]).decode('utf-8').split()[0][2:]

setuptools.setup(name='scorep-cli-score',
                 version='0.1',
                 author='Marcel Achtert',
                 author_email='marcel.achtert@tu-dresden.de',
                 description='A Score-P-score based filter creation tool',
                 url='https://github.com/score-p/scorep-score-gui',
                 packages=['scorep_cli_score'],
                 python_requires='~=3.5',
                 scripts=['scorep_cli_score/scorep-cli-score'],
                 classifiers=[
                     'Development Status :: 4 - Beta',
                     'Intended Audience :: Developers',
                     'Topic :: Software Development :: Build Tools',
                     'License :: OSI Approved :: BSD License 2.0',
                     'Programming Language :: Python :: 3',
                     'Programming Language :: Python :: 3.5',
                     'Programming Language :: Python :: 3.6',
                     'Programming Language :: Python :: 3.7',
                 ],
                 ext_modules=[
                     setuptools.Extension('bind',
                                          sources=['scorep_cli_score/bind.cpp'],
                                          include_dirs=[scorep_include_dir, '{}/cubelib'.format(scorep_include_dir)],
                                          library_dirs=[scorep_library_dir],
                                          libraries=['z', 'cube4', 'scorep_estimator'],
                                          language='c++'),
                     setuptools.Extension('scorep_profile',
                                          sources=['scorep_cli_score/scorep_profile.cpp'],
                                          include_dirs=[scorep_include_dir, '{}/cubelib'.format(scorep_include_dir)],
                                          library_dirs=[scorep_library_dir],
                                          libraries=['z', 'cube4', 'scorep_estimator'],
                                          extra_compile_args=['-std=c++14'],
                                          language='c++'),
                 ])
