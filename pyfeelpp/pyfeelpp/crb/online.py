# -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t  -*- vim:fenc=utf-8:ft=cpp:et:sw=4:ts=4:sts=4
#
# This file is part of the Feel++ library
#
# This library is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation; either
# version 2.1 of the License, or (at your option) any later version.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Lesser General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public
# License along with this library; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
#
# @file
# @author Christophe Prud'homme <christophe.prudhomme@feelpp.org>
# @date 23 Jun 2017
# @copyright 2017 Feel++ Consortium
#

def online(modelname,mu):
    plugin=factoryCRBPlugin("/usr/local/lib","heat3d")
    plugin.loadDB(crb.crbdb+"heat3d/adb74343-793a-407d-bc5f-1fe3e80b97d7/heat3d.crb.json");
    print plugin.name();
    r=plugin.run(mu, 1e-3, 1, False )
    print "s=",r.output(), " error=",r.errorBound()
