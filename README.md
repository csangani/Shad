Shad
======

Build Instructions
------------------

Currently, Shad is supported only on Windows 7 or above, with Visual Studio (2008+).

Currently, we use Premake to generate Visual Studio solution and project files. This helps in project
management across different versions of VS.

To create a solution, open a command window in the project directory and run the following command:

For Visual Studio 2010:

premake.exe vs2010

For Visual Studio 2008:

premake.exe vs2008

Open the solution and build the project. The built executable is present in build\${Configuration}\.
