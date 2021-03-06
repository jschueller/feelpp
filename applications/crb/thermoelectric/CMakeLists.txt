###  TEMPLATE.txt.tpl; coding: utf-8 ---

#  Author(s): Christophe Prud'homme <christophe.prudhomme@feelpp.org>
#       Date: 2011-04-23
#
#  Copyright (C) 2011 Université Joseph Fourier (Grenoble I)
#
# Distributed under the GPL(GNU Public License):
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
#
#

crb_add_library( thermoelectric SRCS thermoelectric.cpp EXEC CRB_LIB_THERMOELECTRIC)
set(FEELPP_CRB_THERMOELECTRIC_LIBRARIES ${CRB_LIB_THERMOELECTRIC} PARENT_SCOPE)
set(FEELPP_CRB_LIBRARIES ${FEELPP_CRB_LIBRARIES} FEELPP_CRB_THERMOELECTRIC_LIBRARIES PARENT_SCOPE)

crb_add_library( nlthermoelectric SRCS nlthermoelectric.cpp EXEC CRB_LIB_NLTHERMOELECTRIC)
set(FEELPP_CRB_NLTHERMOELECTRIC_LIBRARIES ${CRB_LIB_NLTHERMOELECTRIC} PARENT_SCOPE)
set(FEELPP_CRB_LIBRARIES ${FEELPP_CRB_LIBRARIES} FEELPP_CRB_NLTHERMOELECTRIC_LIBRARIES PARENT_SCOPE)

crb_add_library( alphathermoelectric SRCS alphathermoelectric.cpp EXEC CRB_LIB_ALPHATHERMOELECTRIC)
set(FEELPP_CRB_ALPHATHERMOELECTRIC_LIBRARIES ${CRB_LIB_ALPHATHERMOELECTRIC} PARENT_SCOPE)
set(FEELPP_CRB_LIBRARIES ${FEELPP_CRB_LIBRARIES} FEELPP_CRB_ALPHATHERMOELECTRIC_LIBRARIES PARENT_SCOPE)

install( FILES thermoelectric.hpp nlthermoelectric.hpp alphathermoelectric.hpp
         DESTINATION include/feelpp/crb/thermoelectric COMPONENT Devel)


feelpp_add_application( thermoelectricCRB
                        SRCS thermoelectric.hpp thermoelectric_main.cpp
                        LINK_LIBRARIES ${CRB_LIB_THERMOELECTRIC} INSTALL )
feelpp_add_application( nl-thermoelectricCRB
                        SRCS nlthermoelectric.hpp nlthermoelectric_main.cpp
                        LINK_LIBRARIES ${CRB_LIB_NLTHERMOELECTRIC} INSTALL )
feelpp_add_application( alpha-thermoelectricCRB
                        SRCS alphathermoelectric.hpp alphathermoelectric_main.cpp
                        LINK_LIBRARIES ${CRB_LIB_ALPHATHERMOELECTRIC} INSTALL )

# feelpp_add_application( testthCRB SRCS test.cpp )
# feelpp_add_application( thermoelectric_geo_FE_2D
#                         SRCS thermoelectric_geo_FE.cpp DEFS FEELPP_DIM=2 )
# feelpp_add_application( thermoelectric_geo_FE_3D
#                         SRCS thermoelectric_geo_FE.cpp DEFS FEELPP_DIM=3 )


feelpp_add_testcase( th-crb-quarterturn CATEGORY CRB)
feelpp_add_testcase( th-crb-alpha-hel CATEGORY CRB)
feelpp_add_testcase( te-bancmesure CATEGORY CRB)
