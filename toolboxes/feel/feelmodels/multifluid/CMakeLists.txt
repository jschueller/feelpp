# Fluid: PN+1PNG1, levelset : P1G1
foreach( _DIM 2 3)
  feelpp_max( MAX_ORDER 2 ${FEELPP_INSTANTIATION_ORDER_MAX})
  message(STATUS "[feelpp] MULTIFLUID ${_DIM}D PN+1PNG1_P1G1 N=1..${MAX_ORDER}")
  foreach( _FLUID_U_ORDER RANGE 2 ${MAX_ORDER} )
    math(EXPR _FLUID_P_ORDER '${_FLUID_U_ORDER}-1')
    genLibMultifluid(
      DIM ${_DIM}
      FLUID_U_ORDER ${_FLUID_U_ORDER}
      FLUID_P_ORDER ${_FLUID_P_ORDER}
      FLUID_P_CONTINUITY Continuous
      LEVELSET_ORDER 1
      GEO_ORDER  1
      ADD_CMAKE_INSTALL
      )
  set(FEELPP_MODELS_LIBRARIES_MULTIFLUID  ${FEELPP_MODELS_LIBRARIES_MULTIFLUID} ${LIBBASE_NAME}  )
  endforeach()
endforeach()
# Fluid: PN+1PNG1, levelset : P2G1
foreach( _DIM 2 3)
  feelpp_max( MAX_ORDER 2 ${FEELPP_INSTANTIATION_ORDER_MAX})
  message(STATUS "[feelpp] MULTIFLUID ${_DIM}D PN+1PNG1_P2G1 N=1..${MAX_ORDER}")
  foreach( _FLUID_U_ORDER RANGE 2 ${MAX_ORDER} )
    math(EXPR _FLUID_P_ORDER '${_FLUID_U_ORDER}-1')
    genLibMultifluid(
      DIM ${_DIM}
      FLUID_U_ORDER ${_FLUID_U_ORDER}
      FLUID_P_ORDER ${_FLUID_P_ORDER}
      FLUID_P_CONTINUITY Continuous
      LEVELSET_ORDER 2
      GEO_ORDER  1
      ADD_CMAKE_INSTALL
      )
  set(FEELPP_MODELS_LIBRARIES_MULTIFLUID  ${FEELPP_MODELS_LIBRARIES_MULTIFLUID} ${LIBBASE_NAME}  )
  endforeach()
endforeach()

add_library(feelpp_model_multifluid SHARED libmultifluid.cpp)
target_link_libraries(feelpp_model_multifluid ${FEELPP_MODELS_LIBRARIES_MULTIFLUID})
