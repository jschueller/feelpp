/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t -*- vim: set syntax=cpp fenc=utf-8 ft=tcl et sw=4 ts=4 sts=4

  This file is part of the Feel library

  Author(s): Thomas Lantz
       Date: 2015-04-27

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/// [all]

#define USE_BOOST_TEST 1
#if defined(USE_BOOST_TEST)
#define BOOST_TEST_MODULE test_integrateQuadra
#include <testsuite/testsuite.hpp>
#endif


#include <feel/feelfilters/loadmesh.hpp>
#include <feel/feeldiscr/pch.hpp>
#include <feel/feelvf/msi.hpp>
#include <feel/feelvf/integrate.hpp>
#include <feel/feelvf/form.hpp>
#include <feel/feelvf/norml2.hpp>
#include <feel/feelvf/operators.hpp>
#include <feel/feelvf/operations.hpp>
#include <feel/feelvf/projectors.hpp>
#include <feel/feelpoly/multiscalequadrature.hpp>
#include <feel/feelvf/ginac.hpp>
#include <feel/feelfilters/exporter.hpp>

using namespace Feel;


inline
AboutData
makeAbout()
{
    AboutData about( "test_msi" ,
                     "test_msi" ,
                     "8.9e-3",
                     "test msi operator",
                     Feel::AboutData::License_GPL,
                     "Copyright (c) 2015 Feel++ Consortium" );

    about.addAuthor( "Thomas Lantz", "student", "", "" );
    return about;
}

class Test
{
 public :

        Test(int nx,int ny, std::string s)
        {
            ima=holo3_image<float>(ny+1,nx+1);
            ex=s;
            auto f = expr(s);
            for (int i=0;i<=nx;i++)
            {
                for (int j=0;j<=ny;j++)
                {
                    std::map<std::string,double> m {{"x",i*doption("msi.pixelsize")},{"y",j*doption("msi.pixelsize")}};
                    ima(j,i)=f.evaluate(m);
                }
            }
        }
       
           
     void resol( int level )

        {
          /// [mesh] 
        auto pas = pow(2.,level)*doption("msi.pixelsize");
        std::cout << pas << std::endl;  
        auto mesh = createGMSHMesh( _mesh=new Mesh<Hypercube<2>>,
                                    _h=pas, 
                                    _desc=domain(_name="msitest",
                                                 _h=pas,
                                                 _xmax=doption("msi.pixelsize")*(ima.cols()-1),
                                                 _ymax=doption("msi.pixelsize")*(ima.rows()-1)));
    std::cout << "h : " << pas << " xmax : " << doption("msi.pixelsize")*(ima.cols()-1) << " ymax :" << doption("msi.pixelsize")*(ima.rows()-1) << " cols : "<< ima.cols()-1 << " rows : " << ima.rows()-1 <<  std::endl;

  
        

        auto Xhc = Pch<1>( mesh );
        auto u=Xhc->element();
        auto v=Xhc->element();
         
        /// [expression]
        // our function to integrate 
        auto a = form2( _trial=Xhc, _test=Xhc );
        a=integrate( _range=elements( mesh ),
                     _expr=idt(u)*id(v) );


        auto l = form1( _test=Xhc );
        l= integrate( _range=elements( mesh ),
                      _expr=vf::msi<float>(ima,level)*id(v),
                      _quad=_Q<1,MultiScaleQuadrature>() ); 

      
        a.solve( _rhs=l, _solution=u );

        auto fP=vf::project(_space=Xhc,
                            _range=elements(mesh),
                            _expr=expr(ex));
        std::cout << "|| u-f ||^2 =" << normL2 ( _range=elements( mesh ), _expr=idv(u)-idv(fP)) << std::endl;
        auto e = exporter(_mesh=mesh);
        e->add("u",u);
        e->add("ex",fP);
        e->save();
        

       }

    private :

    holo3_image<float> ima;
    std::string ex;

};

#if defined(USE_BOOST_TEST)
FEELPP_ENVIRONMENT_WITH_OPTIONS( makeAbout(), feel_options() );
BOOST_AUTO_TEST_SUITE( msi_suite )

BOOST_AUTO_TEST_CASE( test_run0 )
{
    Test t0=Test(4,4,"x:x:y") ;
    t0.resol(ioption("msi.level"));
}



BOOST_AUTO_TEST_SUITE_END()
#else
std::cout << "USE_BOOST_TEST non define" << std::endl;
#endif
