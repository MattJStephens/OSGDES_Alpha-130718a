The source code in OSGDES_Alpha was complied on a Windows platform with MS Visual Studio 10.
To compile this source code you will need to install various 3rd party libraries.
See the Libraries-130718 repository for information on how to obtain and install these libraries.

Create Environment variables (Can use OSG_CREATE_ENV_VAR if the libraries correlate)
“Environment variables are strings that contain information about the environment for the system, and the currently logged on user. Some software programs use the information to determine where to place files (such as temporary files). ”(Microsoft 2012a)
To view or change environment variables with Windows 7:
1.	Start Control Panel
2.	Click System and Security
3.	Click System
4.	Click Advanced system settings
5.	Click Environment variables.
6.	Click one of the following options, for either a user or a system variable:
•	Click New to add a new variable name and value.
•	Click an existing variable, and then click Edit to change its name or value.
•	Click an existing variable, and then click Delete to remove it.

The Path system variable can be edited to add/remove folders where (Dynamically Linked Libraries) DLLs can be found. If all the environment variables are correctly referencing folder paths to the required DLLs then the addition of the following string to the Path Environment Variable will facilitate the executable in finding the DLLs.
Another option is that all the DLLs sit in same folder as the EXE.

Once the Libraries have been installed, the Environment variables have been created then:

In MS Visual Studio 2010 Solution Explorer
RMButtonClick
Properties (on an empty C++ Project containing the file in the OSGDES_Alpha repository)

Property Pages>>Configuration Properties>>General>>Configuration Type:
Application (.exe)

Property Pages>>Configuration Properties>>General>>Use of MFC:
Use Standard Windows Libraries

Property Pages>>Configuration Properties>>General>>Use of ATL:
Not Using ATL

Property Pages>>Configuration Properties>>General>>Character Set:
Use Multi-Byte Character Set

Property Pages>>Configuration Properties>>General>>Common Language Runtime Support:
No Common Language Runtime Support

Optional:
Property Pages>>Configuration Properties>>Debugging >>Command Arguments:
Script-001.xml or any default script


Property Pages>>Configuration Properties>>VC++ Directories>>Executable Directories:
$(GLUT_BIN);$(GLEW_BIN);$(OSG_DEBUG_BIN);$(PHYSX_BIN);$(OPENAL_BIN);$(ExecutablePath)

Property Pages>>Configuration Properties>>VC++ Directories>>Include directories:
$(GLUT_INC);$(GLEW_INC);$(OSG_DEBUG_INC);$(PHYSX_FOUNDATION_INC);$(PHYSX_COOKING_INC);$(PHYSX_LOADER_INC);$(PHYSX_PHYSICS_INC);$(PHYSX_COMMON_INC);$(PHYSX_CHARACTER_INC);$(BOOST_ROOT_1_50_0);$(PYTHON_INC);$(RUBY_INC);$(OPENAL_INC);$(IncludePath)

Property Pages>>Configuration Properties>>VC++ Directories>>Library Directories:
$(GLUT_LIB);$(GLEW_LIB);$(OSG_DEBUG_LIB);$(PHYSX_LIB);$(BOOST_LIB_1_50_0);$(PYTHON_LIB);$(RUBY_LIB);$(OPENAL_LIB);$(LibraryPath)

Property Pages>>Configuration Properties>>Linker>>Input>>Additional dependencies:
PhysXLoader.lib;PhysXCore.lib;PhysXCooking.lib;NxCharacter.lib;kernel32.lib;user32.lib;gdi32.lib;winspool.lib;comdlg32.lib;advapi32.lib;shell32.lib;ole32.lib;oleaut32.lib;uuid.lib;odbc32.lib;odbccp32.lib;opengl32.lib;glu32.lib;glut32.lib;glew32.lib;osgPresentationd.lib;osgAnimationd.lib;osgManipulatord.lib;osgWidgetd.lib;osgSimd.lib;osgViewerd.lib;osgVolumed.lib;osgParticled.lib;osgTextd.lib;osgFXd.lib;osgShadowd.lib;osgTerraind.lib;osgGAd.lib;osgDBd.lib;osgUtild.lib;osgd.lib;OpenThreadsd.lib;Winmm.lib;Strmiids.lib;Ws2_32.lib;python27.lib;OpenAL32.lib

