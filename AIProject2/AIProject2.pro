TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        ga/Chromosome.cpp \
        ga/DistanceMatrix.cpp \
        ga/Fitness.cpp \
        ga/ParentSelection.cpp \
        ga/Reproduction.cpp \
        main.cpp

HEADERS += \
    ga/Chromosome.hpp \
    ga/DistanceMatrix.hpp \
    ga/Fitness.hpp \
    ga/ParentSelection.hpp \
    ga/Reproduction.hpp
