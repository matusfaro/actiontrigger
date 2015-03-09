======================================================================
=== actiontriggercommon
======================================================================

How to link the library to your application
===========================================

We assume that you have the library and your application located
in the same directory and both are opened in Eclipse workspace.

1) check "actiontriggercommon" on "Project References" page
   in Project properties.

2) Modify "YourApplication.pro" file in your application to contain
   the following lines:

  2a) when using shared library:

      include($$quote($$_PRO_FILE_PWD_)/../actiontriggercommon/shared.pri)

  2b) when using static library:

      include($$quote($$_PRO_FILE_PWD_)/../actiontriggercommon/static.pri)

  2c) when specifying manually:

      INCLUDEPATH *= </path/to/your/library/include>
      LIBS += -L</path/to/your/library> -l<libraryname>

3) In case you are using shared library, modify "bar-descriptor.xml" file
   in your application to contain the following:

  3a) in <configuration name="Device-Debug"> tag:
       <asset path="../actiontriggercommon/arm/so.le-v7-g/" entry="false" dest="lib"><include name="libactiontriggercommon.so.*"/></asset>

  3b) in <configuration name="Device-Release"> tag:

       <asset path="../actiontriggercommon/arm/so.le-v7/" entry="false" dest="lib"><include name="libactiontriggercommon.so.*"/></asset>

  3c) in <configuration name="Device-Profile"> tag:

       <asset path="../actiontriggercommon/arm-p/so.le-v7-g/" entry="false" dest="lib"><include name="libactiontriggercommon.so.*"/></asset>

  3d) in <configuration name="Simulator-Debug"> tag:

       <asset path="../actiontriggercommon/x86/so-g/" entry="false" dest="lib"><include name="libactiontriggercommon.so.*"/></asset>

How to build your application
=============================

Make sure that you build all required build configurations of your library
first. Then you can build your application and it will automatically pick
the correct version of the library. You can run, debug and profile
your application as usual.

How to build a static library
=============================

By default, the library is created as shared one. If you want to build
static one, add the following line to "actiontriggercommon.pro" file:

    CONFIG += staticlib

How to convert Cascades library into generic Qt library
=======================================================

To convert Cascades library into generic Qt library you need to remove
references to Cascades library and classes:

1) Remove the following lines in "cascades10_library.pri" file:

    LIBS += -lbbcascades
    QT   += declarative
    QT   -= gui

2) Replace "library.hpp" and "library.cpp" files with your own code
   that does not use Cascades classes.

How to debug a shared library
=============================

Debugging a static library works automatically. On the other hand,
GDB debugger would not be able to automatically find debug symbols
for your shared library. There are two approach to load them:

1) Via set solib-search-path GDB command:

  1a) Start debugging the application

      To ensure that all breakpoints are hit, check "Stop on startup at" checkbox
      in Debugger tab of "Launch Configuration Properties" dialog - step 2a) - 2c).

  1b) In Console view, select "YourApplication - gdb" log.

  1c) In the log output, write the following line to print GDB information
      about shared library:

      info sharedlibrary

  1d) In the log output, write the following line to add
      your shared library to the GDB shared-library search path:

    1da) For BB10 devices:

         set solib-search-path /ABSOLUTE/PATH/TO/WORKSPACE/actiontriggercommon/arm/so.le-v7-g/

    1db) For BB10 simulators:

         set solib-search-path /ABSOLUTE/PATH/TO/WORKSPACE/actiontriggercommon/x86/so-g/

2) Via Debugger Launch Configuration dialog:

  2a) In the main toolbar, select "Debug" launch mode, YourApplication project
      and your target device/simulator.

  2b) Click on Settings icon next to the YourApplication project in main toolbar.

  2c) In the Launch Configuration Properties dialog, switch to Debugger tab
      and the Shared Libraries inner-tab.

  2d) Use "Add..." button to specify path to your:

    2da) For BB10 devices, actiontriggercommon/arm/so.le-v7-g/ folder in workspace

    2db) For BB10 simulators, actiontriggercommon/x86/so-g/ folder in workspace

How to generate separate debug info
===================================

By default, the shared library contain debug info. If you want to build
a separate debug info file, add the following line to "actiontriggercommon.pro" file:

    CONFIG += separate_debug_info

How to add translations for additional languages
================================================

To add them, specify all required .ts files in "TRANSLATIONS" property
in "actiontriggercommon.pro" file e.g.:

    TRANSLATIONS += $${TARGET}_en_US.ts $${TARGET}_fr_FR.ts

Note that those .ts files will be generated in "translations" folder.

Note that "TRANSLATIONS" property is overridden by "config.pri" file
so your additions need to specified after the "include(config.pri)" statement
in "actiontriggercommon.pro" file.

How to add library translations to an application
=================================================

The build system of the library just creates a set of .qm files stored
in "actiontriggercommon/translations/qm/" directory.

For the following steps, we assume that YourApplication is based
on Cascades Project template i.e. it contains ApplicationUI class.

To use the library translations into your application you need to:

1) When using .qm files directly:

    1a) Add the following code in the "bar-descriptor.xml" file in your application
        to integrate your library translations to the application:

        <asset path="../actiontriggercommon/translations/qm" dest="qm">
            <include name="*.qm"/>
        </asset>

    1b) Add the following line "ApplicationUI" class:

        QTranslator *m_pLibraryTranslator;

    1c) Add the following line to "ApplicationUI" constructor:

        m_pLibraryTranslator = new QTranslator(this);

    1d) Add the following code to "ApplicationUI::onSystemLanguageChanged"
        function to load the library translations from the application:

        QCoreApplication::instance()->removeTranslator(m_pLibraryTranslator);
        file_name = QString("actiontriggercommon_%1").arg(locale_string);
        if (m_pLibraryTranslator->load(file_name, "app/native/qm")) {
            QCoreApplication::instance()->installTranslator(m_pLibraryTranslator);
        }

2) When using qm files via QRC file:

    2a) Uncomment the following line in "actiontriggercommon.pro" file:

        RESOURCES += translations/qm/actiontriggercommon_translations.qrc

    2b) Add the following line "ApplicationUI" class:

        QTranslator *m_pLibraryTranslator;

    2c) Add the following line to "ApplicationUI" constructor:

        m_pLibraryTranslator = new QTranslator(this);

    2d) Add the following code to "ApplicationUI::onSystemLanguageChanged"
        function to load the library translations from the qrc:

        QCoreApplication::instance()->removeTranslator(m_pLibraryTranslator);
        file_name = QString(":/actiontriggercommon_%1.qm").arg(locale_string);
        if (m_pLibraryTranslator->load(file_name)) {
            QCoreApplication::instance()->installTranslator(m_pLibraryTranslator);
        }

    2e) When using static library, you need to add the following line
        at the beginning of "main" function in "main.cpp" file in your application:

        Q_INIT_RESOURCES(actiontriggercommon_translations)
