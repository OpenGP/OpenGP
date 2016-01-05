#--- Qt5 Needs to know where to find its cmake files
# @see http://qt-project.org/doc/qt-5/cmake-manual.html

# @see problem fix for homebrew: https://github.com/Homebrew/homebrew/issues/29938
#    brew link --force qt5 && 
#    ln -s /usr/local/Cellar/qt5/5.5.0/mkspecs /usr/local/mkspecs 
#    ln -s /usr/local/Cellar/qt5/5.5.0/plugins /usr/local/plugins

if(APPLE)
    set(QTDIR $ENV{QTDIR}) 
    if(NOT QTDIR) 
        message(WARNING "QTDIR must not be empty!!")
    else()
        set(CMAKE_PREFIX_PATH ${QTDIR})
    endif()
endif()

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5OpenGL REQUIRED) 
find_package(Qt5Xml REQUIRED)

set(CMAKE_AUTOMOC ON)
