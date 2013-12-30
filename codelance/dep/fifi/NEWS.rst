News for fifi
============

This file lists the major changes between versions. For a more detailed list
of every change, see the Git log.

Latest
------
* Minor: Updated to gauge version 5.x.y
* Minor: Updated to waf 1.7.10

9.1.0
------
* Minor: Updated to waf-tools version 2.x.y
* Bug: Include <cstdlib> instead of <cmath> to use std::abs(int)

9.0.0
-----
* Updated/added new functions to fifi_utils.hpp for converting between
  lengths, sizes and elements.
* Bumped sak to version 10
* Added project generator tool which allows Visual Studio solutions to be
  generated.

8.0.0
-----
* Updating waf to version 1.7.9 and start using the wurf_install_path tool.
* Rename default_field_impl.hpp to default_field.hpp

7.0.0
-----
* Updating to use new external waf tools

6.0.0
-----
* Bump sak to version 7

5.0.0
-----
* Upgrade to sak version 6.x.y

4.0.1
-----
* Removing benchmarks from the automatic unit test runner.

4.0.0
----
* Using .hpp extension instead of .h

3.0.0
-----
* Switched c++11 compiler flag on per default
* Adding optimal prime field
* Moving benchmarks to the gauge tool
* Moved fifi headers to the sub-directory src
* Upgraded to waf 1.7.2 and using waf unit tests tool (for automatically
  running unit tests during a build).
* Switched to sak version 4.x.y

2.0.0
-----
* Updated the Waf build system to handle dependencies better. The new tools
  used are not compatible with the old versions so we have to bump the major
  version.

1.0.2
-----
* Added bechmarks
* Added dependency on sak providing timer functionality
  used in benchmarks
* Bumped dependency version for gtest to 1.0.3-gtest_1_6_0
* Bumped dependency version for boost to 1.1.2-boost_1_48_0
* Bumped dependency version for sak to 1.1.1
* Fixed win32 link and cxx flags

