cmake_minimum_required(VERSION 3.30)
project(upozn C)

set(CMAKE_C_STANDARD 11)

add_executable(upozn main.c
        ariphmetics_operations/ariphmeticValidators.c
        ariphmetics_operations/performCalculation.c
        ariphmetics_operations/rpnImplementation.c
        ariphmetics_operations/stackImplementation.c
        convert/performConvert.h
        validators.h
        validators.c
        convert/performConvert.c
        convert/convertValidator.h
        convert/convertValidator.c
        rota_minimization/setsImplementation.h
        rota_minimization/setsImplementation.c
        rota_minimization/rotaOperation.h
        rota_minimization/rotaOperation.c
        rota_minimization/performRota.h
        rota_minimization/performRota.c
        rota_minimization/rotaTable.c
        rota_minimization/rotaTable.h
        rota_minimization/fileHandling.c
)
