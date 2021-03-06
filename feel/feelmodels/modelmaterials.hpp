/* -*- mode: c++; coding: utf-8; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; show-trailing-whitespace: t  -*-

 This file is part of the Feel++ library

 Author(s): Christophe Prud'homme <christophe.prudhomme@feelpp.org>
 Date: 16 Mar 2015

 Copyright (C) 2015 Feel++ Consortium

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */
#ifndef FEELPP_MODELMATERIALS_HPP
#define FEELPP_MODELMATERIALS_HPP 1


#include <vector>
#include <feel/feelvf/expr.hpp>
#include <feel/feelvf/ginac.hpp>

#include <boost/property_tree/ptree.hpp>

namespace Feel {

namespace pt =  boost::property_tree;

struct FEELPP_EXPORT ModelMaterial
{
    static const uint16_type expr_order = 2;
    typedef scalar_field_expression<expr_order> expr_scalar_type;
    typedef vector_field_expression<2,1,expr_order> expr_vectorial2_type;
    typedef vector_field_expression<3,1,expr_order> expr_vectorial3_type;
    typedef std::tuple< boost::optional<double>, boost::optional<expr_scalar_type>, boost::optional<expr_vectorial2_type>,boost::optional<expr_vectorial3_type> > mat_property_expr_type;

    ModelMaterial( WorldComm const& worldComm = Environment::worldComm() );
    ModelMaterial( ModelMaterial const& ) = default;
    ModelMaterial( ModelMaterial&& ) = default;
    ModelMaterial& operator=( ModelMaterial const& ) = default;
    ModelMaterial& operator=( ModelMaterial && ) = default;
    ModelMaterial( std::string const& name, pt::ptree const& p,
                   WorldComm const& worldComm = Environment::worldComm(),
                   std::string const& directoryLibExpr = "" );

    std::string const& name() const { return M_name; }
    std::string const& physics() const { return M_physics; }
    /*! Set Name
     */
    void setName( std::string const& name ) { M_name = name; }

    void setDirectoryLibExpr( std::string const& directoryLibExpr ) { M_directoryLibExpr = directoryLibExpr; }

    void setPhysics( std::string const s) { M_physics = s; }

    void setProperty( std::string const& property, pt::ptree const& p );
    void setProperty( std::string const& property, double val );

    bool hasPropertyConstant( std::string const& prop ) const;
    bool hasPropertyExprScalar( std::string const& prop ) const;
    bool hasPropertyExprVectorial2( std::string const& prop ) const;
    bool hasPropertyExprVectorial3( std::string const& prop ) const;
    double propertyConstant( std::string const& prop ) const;
    expr_scalar_type const& propertyExprScalar( std::string const& prop ) const;
    expr_vectorial2_type const& propertyExprVectorial2( std::string const& prop ) const;
    expr_vectorial3_type const& propertyExprVectorial3( std::string const& prop ) const;

    /*! Material mass density
     */
    double rho() const { return this->propertyConstant( "rho" ); }
    void setRho( double v ) { this->setProperty( "rho", v ); }

    /*! Molecular(dynamic) viscosity
     */
    double mu() const { return this->propertyConstant( "mu" ); }
    void setMu( double v ) { this->setProperty( "mu", v ); }

    /*! Specify the constant-pressure specific heat Cp.
     */
    double Cp() const { return this->propertyConstant( "Cp" ); }
    void setCp( double v ) { this->setProperty( "Cp", v ); }
    /*! Specify the constant-volume specific heat Cv.
     */
    double Cv() const { return this->propertyConstant( "Cv" ); }
    void setCv( double v ) { this->setProperty( "Cv", v ); }

    /*! heat diffusion coefficients
     */
    double k11() const {  return this->propertyConstant( "k11" ); }
    void setK11( double v ) { this->setProperty( "k11", v ); }
    double k12() const { return this->propertyConstant( "k12" ); }
    void setK12( double v ) { this->setProperty( "k12", v ); }
    double k13() const { return this->propertyConstant( "k13" ); }
    void setK13( double v ) { this->setProperty( "k13", v ); }
    double k22() const { return this->propertyConstant( "k22" ); }
    void setK22( double v ) { this->setProperty( "k22", v ); }
    double k23() const { return this->propertyConstant( "k23" ); }
    void setK23( double v ) { this->setProperty( "k23", v ); }
    double k33() const { return this->propertyConstant( "k33" ); }
    void setK33( double v ) { this->setProperty( "k33", v ); }

    /*! Material Reference temperature
     */
    double Tref() const { return this->propertyConstant( "Tref" ); }
    void setTref( double v ) { this->setProperty( "Tref", v ); }

    /*! Material coefficient for thermal expansion
     */
    double beta() const { return this->propertyConstant( "beta" ); }
    void setBeta( double v ) { this->setProperty( "beta", v ); }

    /*! heat capacity
     */
    double C() const { return this->propertyConstant( "C" ); }
    void setC( double v ) { this->setProperty( "C", v ); }

    double Cs() const { return this->propertyConstant( "Cs" ); }
    double Cl() const { return this->propertyConstant( "Cl" ); }
    double L() const { return this->propertyConstant( "L" ); }
    double Ks() const { return this->propertyConstant( "Ks" ); }
    double Kl() const { return this->propertyConstant( "Kl" ); }
    double Tsol() const { return this->propertyConstant( "Tsol" ); }
    double Tliq() const { return this->propertyConstant( "Tliq" ); }
    void setCs( double v ) { this->setProperty( "Cs", v ); }
    void setCl( double v ) { this->setProperty( "Cl", v ); }
    void setL( double v ) { this->setProperty( "L", v ); }
    void setKs( double v ) { this->setProperty( "Ks", v ); }
    void setKl( double v ) { this->setProperty( "Kl", v ); }
    void setTsol( double v ) { this->setProperty( "Tsol", v ); }
    void setTliq( double v ) { this->setProperty( "Tliq", v ); }

    // Mechanical properties
    /*! Young's Modulus
     */
    double E() const { return this->propertyConstant( "E" ); }
    void setE( double v ) { this->setProperty( "E", v ); }

    /*! Poisson's ratio
     */
    double nu() const { return this->propertyConstant( "nu" ); }
    void setNu( double v ) { this->setProperty( "nu", v ); }
    /*! Electrical conductivity
     */
    double sigma() const { return this->propertyConstant( "sigma" ); }
    void setSigma( double v ) { this->setProperty( "sigma", v ); }

    void load( std::string const& );

    void setParameterValues( std::map<std::string,double> const& mp );

    /**
     *
     */
    std::string getString( std::string const& key ) {
        try { return M_p.get<std::string>( key ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }

    /**
     *
     */
    int getInt( std::string const& key ) {
        try { return M_p.get<int>( key ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }

    /**
     *
     */
    double getDouble( std::string const& key ) {
        try { return M_p.get<double>( key ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    /**
     *
     */
    Expr<GinacEx<2> > getScalar( std::string const& key ) {
        try { return expr( M_p.get<std::string>( key ) ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    // Expr<GinacEx<2> > getScalar( std::string const& key, std::pair<std::string,double> const& params ) { return expr( M_p.get( key, "0" ), params ); }
    /**
     *
     */
    Expr<GinacEx<2> > getScalar( std::string const& key, std::map<std::string,double> const& params ) {
        try { return expr( M_p.get<std::string>( key ), params ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }

    /**
     *
     */
    template<typename ExprT> Expr<GinacExVF<ExprT> > getScalar( std::string const& key,
                                                                std::string const& sym, ExprT e ) {
        try { return expr( M_p.get<std::string>( key ), sym, e ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }

    /**
     *
     */
    template<typename ExprT> Expr<GinacExVF<ExprT> > getScalar( std::string const& key,
                                                                std::initializer_list<std::string> const& sym,
                                                                std::initializer_list<ExprT> e ) {
        try { return expr( M_p.get<std::string>( key ), sym, e ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }

    /**
     *
     */
    template<typename ExprT> Expr<GinacExVF<ExprT> > getScalar( std::string const& key,
                                                                std::vector<std::string> const& sym,
                                                                std::vector<ExprT> e ) {
        try { return expr( M_p.get<std::string>( key ), sym, e ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }

    /**
     *
     */
    template<typename ExprT> Expr<GinacExVF<ExprT> > getScalar( std::string const& key,
                                                                std::initializer_list<std::string> const& sym,
                                                                std::initializer_list<ExprT> e,
                                                                std::map<std::string, double> params ) {
        try {
            auto ex = expr( M_p.get<std::string>( key ), sym, e );
            ex.setParameterValues( params );
            return ex;
        }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    template<int T> Expr<GinacMatrix<T,1,2> > getVector( std::string const& key ) {
        try { return expr<T,1>( M_p.get<std::string>( key ) ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    template<int T> Expr<GinacMatrix<T,1,2> > getVector( std::string const& key,
                                                         std::pair<std::string,double> const& params ) {
        try { return expr<T,1>( M_p.get<std::string>( key ), params ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    template<int T> Expr<GinacMatrix<T,1,2> > getVector( std::string const& key,
                                                         std::map<std::string,double> const& params ) {
        try { return expr<T,1>( M_p.get<std::string>( key ), params ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    // template<int T, typename ExprT> Expr<GinacMatrixVF<ExprT> > getVector( std::string const& key, std::string const& sym, ExprT e );
    // template<int T, typename ExprT> Expr<GinacMatrix<T,1,2> > getVector( std::string const& key, std::initializer_list<std::string> const& sym, std::initializer_list<ExprT> e );
    // template<int T, typename ExprT> Expr<GinacMatrix<T,1,2> > getVector( std::string const& key, std::vector<std::string> const& sym, std::vector<ExprT> e );
    template<int T1, int T2=T1> Expr<GinacMatrix<T1,T2,2> > getMatrix( std::string const& key ) {
        try { return expr<T1,T2>( M_p.get<std::string>( key ) ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    template<int T1, int T2=T1> Expr<GinacMatrix<T1,T2,2> > getMatrix( std::string const& key,
                                                                       std::pair<std::string,double> const& params ) {
        try { return expr<T1,T2>( M_p.get<std::string>( key ), params ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    template<int T1, int T2=T1> Expr<GinacMatrix<T1,T2,2> > getMatrix( std::string const& key,
                                                                       std::map<std::string,double> const& params ) {
        try { return expr<T1,T2>( M_p.get<std::string>( key ), params ); }
        catch( pt::ptree_error e ) {
            cerr << "key " << key << ": " << e.what() << std::endl;
            exit(1);
        }
    }
    // template<int T1, int T2, typename ExprT> Expr<GinacExVF<ExprT> > getMatrix( std::string const& key, std::string const& sym, ExprT e );
    // template<int T1, int T2, typename ExprT> Expr<GinacExVF<ExprT> > getMatrix( std::string const& key, std::initializer_list<std::string> const& sym, std::initializer_list<ExprT> e );
    // template<int T1, int T2, typename ExprT> Expr<GinacExVF<ExprT> > getMatrix( std::string const& key, std::vector<std::string> const& sym, std::vector<ExprT> e );

private:

    WorldComm const * M_worldComm;
    std::string M_name; /*!< Material name*/
    pt::ptree M_p;
    std::string M_directoryLibExpr;

    //! mat propeteries
    std::map<std::string, mat_property_expr_type > M_materialProperties;
    //! material physics
    std::string M_physics;
};

std::ostream& operator<<( std::ostream& os, ModelMaterial const& m );

/**
 * @brief a set of materials
 * key: mesh marker
 * name -> name of the materials - can be different
 */
class FEELPP_EXPORT ModelMaterials: public std::map<std::string,ModelMaterial>
{
public:
    using value_type = std::map<std::string,ModelMaterial>::value_type;
    ModelMaterials( WorldComm const& worldComm = Environment::worldComm() );
    ModelMaterials( pt::ptree const& p, WorldComm const& worldComm = Environment::worldComm() );
    virtual ~ModelMaterials() = default;
    void setPTree( pt::ptree const& _p ) { M_p = _p; setup(); }
    ModelMaterial loadMaterial( std::string const& );
    ModelMaterial getMaterial( pt::ptree const& );

    ModelMaterial const&
    material( std::string const& m ) const
        {
            auto it = this->find( m );
            if ( it == this->end() )
                throw std::invalid_argument( std::string("ModelMaterial: Invalid material marker ") + m );
            return it->second;

        }

    void setDirectoryLibExpr( std::string const& directoryLibExpr ) { M_directoryLibExpr = directoryLibExpr; }

    void setParameterValues( std::map<std::string,double> const& mp );

    void saveMD(std::ostream &os);
private:
    void setup();
private:
    WorldComm const* M_worldComm;
    pt::ptree M_p;
    std::string M_directoryLibExpr;

};

FEELPP_EXPORT inline ModelMaterial
material( ModelMaterials::value_type const& m )
{
    return m.second;
}

FEELPP_EXPORT inline std::string
marker( ModelMaterials::value_type const& m )
{
    return m.first;
}

}
#endif
