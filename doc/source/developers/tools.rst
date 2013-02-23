.. _developer_tools:

Tools
=====

Valgrind and Python
-------------------

Valgrind is a great tool for memory debugging, memory leak detection, and profiling.
However, both Python and NumPy floods the valgrind output with memory errors - it is therefore necessary to use a debug and valgrind friendly version of Python::

  sudo apt-get build-dep python
  PV=2.7.3
  sudo mkdir /opt/python
  cd /tmp
  wget http://www.python.org/ftp/python/$PV/Python-$PV.tgz
  tar xf Python-$PV.tgz
  cd Python-$PV
  ./configure --with-pydebug --without-pymalloc --with-valgrind --prefix /opt/python
  sudo make install
  sudo ln -s /opt/python/bin/python /usr/bin/dython

Build Bohrium with custom Python
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Build and install it::

  sudo make BH_PYTHON=dython
  sudo make install

Most Used Commands
~~~~~~~~~~~~~~~~~~

Valgrind can be used to detect memory errors by invoking it with::

  valgrind --vex-iropt-precise-memory-exns=yes dython <SCRIPT_NAME>

Narrowing the valgrind analysis, add the following to your source code::

  #include <valgrind/callgrind.h>
  ... your code ...
  CALLGRIND_START_INSTRUMENTATION;
  ... your code ...
  CALLGRIND_STOP_INSTRUMENTATION;
  CALLGRIND_DUMP_STATS;

Then run valgrind with the flag::

  --instr-atstart=no

Invoking valgrind to determine cache-utilization::

  --tool=callgrind --simulate-cache=yes <PROG> <PROG_PARAM>

Writing Documentation
---------------------

The documentation is written in `Sphinx <http://sphinx.pocoo.org/>`_.

You will need the following to write/build the documentation::

  sudo apt-get install doxygen python-sphinx python-docutils python-setuptools

As well as a python-packages **breathe** and **numpydoc** for integrating doxygen-docs with Sphinx::

  sudo easy_install breathe numpydoc

Overview of the documentatation files::

  bohrium/doc                 # Root folder of the documentation.
  bohrium/doc/source          # Write / Edit the documentation here.
  bohrium/doc/build           # Documentation is "rendered" and stored here.
  bohrium/doc/Makefile        # This file instructs Sphinx on how to "render" the documentation.
  bohrium/doc/make.bat        # ---- || ----, on Windows
  bohrium/doc/deploy_doc.sh   # This script pushes the rendered docs to http://bohrium.bitbucket.org.

Most used commands
~~~~~~~~~~~~~~~~~~

These commands assume that your current working dir is **bohrium/doc**.

Initiate doxygen::
 
  make doxy

Render a html version of the docs::

  make html

Push the html-rendered docs to http://bohrium.bitbucket.org, this command assumes that you have write-access to the doc-repos on bitbucket::

  make deploy

The docs still needs a neat way to integrate a full API-documentation of the Bohrium core, managers and engines.

Continuous Integration
----------------------

At some point a proper automated build/deploy/test/benchmark system will be setup/configured.

The basis of which will probably be `Jenkins <https://wiki.jenkins-ci.org/display/JENKINS/Installing+Jenkins+on+Ubuntu>`_.

Setup jenkins::

  wget -q -O - http://pkg.jenkins-ci.org/debian/jenkins-ci.org.key | sudo apt-key add -
  sudo sh -c 'echo deb http://pkg.jenkins-ci.org/debian binary/ > /etc/apt/sources.list.d/jenkins.list'
  sudo apt-get update
  sudo apt-get install jenkins

Then configure it via web-interface.
