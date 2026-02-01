# Script VS Source
Scripts and source files are for two different kinds of execution, scripts are interpreted and good for testing in a debugger by using hot-reloading to test changes, while source files are compiled into binaries for optimization and testing with libraries written in different languages or don't have a source code packed with them.

# Scripts (Raw code for interpretation)
Scripts have the file extension '.script', but can also get packet into a script section of '.soft' files. Scripts can be changed and reloaded at runtime, making them perfect to test small things or use as API layer. But scripts need the interpreter to run, so if used in releases they must be packed with a compiler added installer for the interpreter if not installed already. Also it is good for AI code agents to use because they can change the code fast.

# Source files (Compiled code)
Source files have the extension '.source', and are compiled into binaries that can be run as standalone programs. They can also get hot-reloaded in debuggers that support it, but they need to get recompiled first, but you shouldn't see a big difference in speed when hot-reloading. They can link to libraries written in other languages or libraries that don't have source code packed with them. The compiled code can get used for most things, ranging from "Hello world!" programs to operating systems.

# Next steps
The scripting tutorial can be found here: https://github.com/CaptainBear079/System-079/blob/main/doc/ScriptingBasics/FirstScript.md

The source code tutorial can be found here: https://github.com/CaptainBear079/System-079/blob/main/doc/CodingBasics/FirstSource.md

If you want to write a custom interpreter you can read this interpreter basics guide: https://github.com/CaptainBear079/System-079/blob/main/doc/Interpreting/LetsWriteAInterpreter.md

Also you can read the DEV Log here: https://github.com/CaptainBear079/System-079/blob/main/doc/HCB_DEVLog/Entry2.md

- by CaptainBear079 aka. Hacklord Captain Bear