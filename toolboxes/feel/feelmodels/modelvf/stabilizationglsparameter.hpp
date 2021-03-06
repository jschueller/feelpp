#ifndef FEELPP_MODELS_VF_STABILIZATION_GLS_PARMATER_EXPR_H
#define FEELPP_MODELS_VF_STABILIZATION_GLS_PARMATER_EXPR_H 1

#include <feel/feelmodels/modelvf/exprtensorbase.hpp>

namespace Feel
{
namespace vf
{
namespace FeelModels
{

template<typename Geo_t, typename Basis_i_t, typename Basis_j_t,typename ShapeType,typename ExprType>
struct tensorStabGLSParameterEigenValue : public tensorBase<Geo_t,Basis_i_t,Basis_j_t,ShapeType,typename ExprType::value_type >
{
    typedef tensorBase<Geo_t,Basis_i_t,Basis_j_t,ShapeType,typename ExprType::value_type> super_type;
    typedef tensorStabGLSParameterEigenValue<Geo_t,Basis_i_t,Basis_j_t,ShapeType,ExprType> self_type;
    typedef ExprType expr_type;
    typedef typename ExprType::value_type value_type;
    typedef typename super_type::matrix_shape_type matrix_shape_type;

    typedef typename expr_type::expression_convection_type::template tensor<Geo_t,Basis_i_t,Basis_j_t> tensor_convection_type;
    typedef typename expr_type::expression_coeffdiffusion_type::template tensor<Geo_t,Basis_i_t,Basis_j_t> tensor_coeffdiffusion_type;

    tensorStabGLSParameterEigenValue( expr_type const& expr, Geo_t const& geom )
        :
        super_type( geom ),
        M_expr( expr ),
        M_tensorConvection( expr.exprConvection(),geom ),
        M_tensorCoeffDiffusion( expr.exprCoeffDiffusion(),geom )
        {}

    void update( Geo_t const& geom )
        {
            this->update( geom, mpl::bool_<expr_type::has_convection_expr>(), mpl::bool_<expr_type::has_coeffdiffusion_expr>() );
        }
    void update( Geo_t const& geom, uint16_type face ) { CHECK( false ) << "TODO"; }

    value_type
    evalq( uint16_type c1, uint16_type c2, uint16_type q ) const
        {
            return M_localMatrixInGeoContext[q](0,0);
        }
    matrix_shape_type const&
    evalq( uint16_type q ) const
        {
            return M_localMatrixInGeoContext[q];
        }
private :
    void update( Geo_t const& geom, mpl::true_, mpl::true_ )
        {
            this->setGmc( geom );
            M_tensorConvection.update( geom );
            M_tensorCoeffDiffusion.update( geom );
            M_localMatrixInGeoContext.resize( this->gmc()->nPoints() );
            size_type eltId = this->gmc()->id();
            value_type hSize = M_expr.stabGLSParameter().hSize( eltId );
            value_type mK = M_expr.stabGLSParameter().mK( eltId );
            for ( uint16_type q = 0; q < this->gmc()->nPoints(); ++q )
            {
                value_type norm_u_square = 0.;
                for (uint16_type c=0;c<tensor_convection_type::shape::M;++c)
                    norm_u_square += math::pow(M_tensorConvection.evalq( c,0,q),2);
                value_type norm_u = math::sqrt( norm_u_square );
                value_type evalCoeffDiffusion = M_tensorCoeffDiffusion.evalq(0,0,q);
                value_type Re = mK*norm_u*hSize/(2*evalCoeffDiffusion);
#if 0
                value_type xi_Re = std::min( 1., Re );
                M_localMatrixInGeoContext[q](0,0) = hSize*xi_Re/( std::max( 1e-10, 2*norm_u) );
#else
                if ( Re < 1. )
                    M_localMatrixInGeoContext[q](0,0) = math::pow(hSize,2)*mK/(4*evalCoeffDiffusion);
                else
                    M_localMatrixInGeoContext[q](0,0) = hSize/(2*norm_u);
#endif
            }
        }
    void update( Geo_t const& geom, mpl::false_, mpl::true_ )
        {
            this->setGmc( geom );
            M_tensorCoeffDiffusion.update( geom );
            M_localMatrixInGeoContext.resize( this->gmc()->nPoints() );
            size_type eltId = this->gmc()->id();
            value_type hSize = M_expr.stabGLSParameter().hSize( eltId );
            value_type mK = M_expr.stabGLSParameter().mK( eltId );
            for ( uint16_type q = 0; q < this->gmc()->nPoints(); ++q )
            {
                value_type evalCoeffDiffusion = M_tensorCoeffDiffusion.evalq(0,0,q);
                M_localMatrixInGeoContext[q](0,0) = math::pow(hSize,2)*mK/(4*evalCoeffDiffusion);
            }
        }
    void update( Geo_t const& geom, mpl::true_, mpl::false_ )
        {
            this->setGmc( geom );
            M_tensorConvection.update( geom );
            M_localMatrixInGeoContext.resize( this->gmc()->nPoints() );
            size_type eltId = this->gmc()->id();
            value_type hSize = M_expr.stabGLSParameter().hSize( eltId );
            for ( uint16_type q = 0; q < this->gmc()->nPoints(); ++q )
            {
                value_type norm_u_square = 0.;
                for (uint16_type c=0;c<tensor_convection_type::shape::M;++c)
                    norm_u_square += math::pow(M_tensorConvection.evalq( c,0,q),2);
                value_type norm_u = math::sqrt( norm_u_square );
                if ( math::abs( norm_u ) > 1e-10 )
                    M_localMatrixInGeoContext[q](0,0) = hSize/(2*norm_u);
                else
                    M_localMatrixInGeoContext[q](0,0) = 0.;
            }
        }

private :
    expr_type const& M_expr;
    tensor_convection_type M_tensorConvection;
    tensor_coeffdiffusion_type M_tensorCoeffDiffusion;
    std::vector<matrix_shape_type> M_localMatrixInGeoContext;
};

template<typename Geo_t, typename Basis_i_t, typename Basis_j_t,typename ShapeType,typename ExprType>
struct tensorStabGLSParameterDoublyAsymptoticApproximation : public tensorBase<Geo_t,Basis_i_t,Basis_j_t,ShapeType,typename ExprType::value_type >
{
    typedef tensorBase<Geo_t,Basis_i_t,Basis_j_t,ShapeType,typename ExprType::value_type> super_type;
    typedef tensorStabGLSParameterDoublyAsymptoticApproximation<Geo_t,Basis_i_t,Basis_j_t,ShapeType,ExprType> self_type;
    typedef ExprType expr_type;
    typedef typename ExprType::value_type value_type;
    typedef typename super_type::matrix_shape_type matrix_shape_type;

    typedef typename expr_type::expression_convection_type::template tensor<Geo_t,Basis_i_t,Basis_j_t> tensor_convection_type;
    typedef typename expr_type::expression_coeffdiffusion_type::template tensor<Geo_t,Basis_i_t,Basis_j_t> tensor_coeffdiffusion_type;

    tensorStabGLSParameterDoublyAsymptoticApproximation( expr_type const& expr, Geo_t const& geom )
        :
        super_type( geom ),
        M_expr( expr ),
        M_tensorConvection( expr.exprConvection(),geom ),
        M_tensorCoeffDiffusion( expr.exprCoeffDiffusion(),geom )
        {}

    void update( Geo_t const& geom )
        {
            this->update( geom, mpl::bool_<expr_type::has_convection_expr>(), mpl::bool_<expr_type::has_coeffdiffusion_expr>() );
        }
    void update( Geo_t const& geom, uint16_type face ) { CHECK( false ) << "TODO"; }

    value_type
    evalq( uint16_type c1, uint16_type c2, uint16_type q ) const
        {
            return M_localMatrixInGeoContext[q](0,0);
        }
    matrix_shape_type const&
    evalq( uint16_type q ) const
        {
            return M_localMatrixInGeoContext[q];
        }
private :
    void update( Geo_t const& geom, mpl::true_, mpl::true_ )
        {
            this->setGmc( geom );
            M_tensorConvection.update( geom );
            M_tensorCoeffDiffusion.update( geom );
            M_localMatrixInGeoContext.resize( this->gmc()->nPoints() );
            size_type eltId = this->gmc()->id();
            value_type hSize = M_expr.stabGLSParameter().hSize( eltId );
            value_type mK = (1./3.);
            for ( uint16_type q = 0; q < this->gmc()->nPoints(); ++q )
            {
                value_type norm_u_square = 0.;
                for (uint16_type c=0;c<tensor_convection_type::shape::M;++c)
                    norm_u_square += math::pow(M_tensorConvection.evalq( c,0,q),2);
                value_type norm_u = math::sqrt( norm_u_square );
                value_type evalCoeffDiffusion = M_tensorCoeffDiffusion.evalq(0,0,q);
#if 0
                value_type xi_Re = std::min(1.,(1./3.)*hSize*norm_u/(2*evalCoeffDiffusion));
                M_localMatrixInGeoContext[q](0,0) = hSize*xi_Re/( std::max( 1e-10, 2*norm_u) );
#else
                value_type Re = mK*hSize*norm_u/(2*evalCoeffDiffusion);
                if ( Re < 1 )
                    M_localMatrixInGeoContext[q](0,0) = math::pow(hSize,2)*mK/(4*evalCoeffDiffusion);
                else
                    M_localMatrixInGeoContext[q](0,0) = hSize/(2*norm_u);
#endif
            }
        }
    void update( Geo_t const& geom, mpl::false_, mpl::true_ )
        {
            this->setGmc( geom );
            M_tensorCoeffDiffusion.update( geom );
            M_localMatrixInGeoContext.resize( this->gmc()->nPoints() );
            size_type eltId = this->gmc()->id();
            value_type hSize = M_expr.stabGLSParameter().hSize( eltId );
            value_type mK = (1./3.);
            for ( uint16_type q = 0; q < this->gmc()->nPoints(); ++q )
            {
                value_type evalCoeffDiffusion = M_tensorCoeffDiffusion.evalq(0,0,q);
                M_localMatrixInGeoContext[q](0,0) = math::pow(hSize,2)*mK/(4*evalCoeffDiffusion);
            }
        }
    void update( Geo_t const& geom, mpl::true_, mpl::false_ )
        {
            this->setGmc( geom );
            M_tensorConvection.update( geom );
            M_localMatrixInGeoContext.resize( this->gmc()->nPoints() );
            size_type eltId = this->gmc()->id();
            value_type hSize = M_expr.stabGLSParameter().hSize( eltId );
            for ( uint16_type q = 0; q < this->gmc()->nPoints(); ++q )
            {
                value_type norm_u_square = 0.;
                for (uint16_type c=0;c<tensor_convection_type::shape::M;++c)
                    norm_u_square += math::pow(M_tensorConvection.evalq( c,0,q),2);
                value_type norm_u = math::sqrt( norm_u_square );
                if ( math::abs( norm_u ) > 1e-10 )
                    M_localMatrixInGeoContext[q](0,0) = hSize/(2*norm_u);
                else
                    M_localMatrixInGeoContext[q](0,0) = 0.;
            }
        }
private :
    expr_type const& M_expr;
    tensor_convection_type M_tensorConvection;
    tensor_coeffdiffusion_type M_tensorCoeffDiffusion;
    std::vector<matrix_shape_type> M_localMatrixInGeoContext;
};




template<typename StabilizationGLSParameterType, typename ExprConvectionType, typename ExprCoeffDiffusionType,
         bool HasConvectionExpr,bool HasCoeffDiffusionExpr,int QuadOrder>
class StabilizationGLSParameterExpr
{
public:

    typedef StabilizationGLSParameterExpr<StabilizationGLSParameterType,ExprConvectionType,ExprCoeffDiffusionType,HasConvectionExpr,HasCoeffDiffusionExpr,QuadOrder> this_type;
    typedef ExprConvectionType expression_convection_type;
    typedef ExprCoeffDiffusionType expression_coeffdiffusion_type;
    typedef StabilizationGLSParameterType stabilization_glsparameter_type;
    static const bool has_convection_expr = HasConvectionExpr;
    static const bool has_coeffdiffusion_expr = HasCoeffDiffusionExpr;

    static const size_type context = ExprConvectionType::context|ExprCoeffDiffusionType::context;
    static const bool is_terminal = true;
    static const uint16_type imorderDefaultConvectionDiffusion = ( ExprConvectionType::imorder > ExprCoeffDiffusionType::imorder )?ExprConvectionType::imorder:ExprCoeffDiffusionType::imorder;
    static const uint16_type imorderDefault = (has_convection_expr && has_coeffdiffusion_expr)? imorderDefaultConvectionDiffusion :
        ( ( has_convection_expr )?ExprConvectionType::imorder : ExprCoeffDiffusionType::imorder );
    static const uint16_type imorder = (QuadOrder>=0)?QuadOrder:imorderDefault;
    static const bool imIsPoly = false;
    typedef double value_type;
    typedef value_type evaluate_type;

    template<typename Func>
    struct HasTestFunction
    {
        static const bool result = false;
    };
    template<typename Func>
    struct HasTrialFunction
    {
        static const bool result = false;
    };
    using test_basis = std::nullptr_t;
    using trial_basis = std::nullptr_t;

    StabilizationGLSParameterExpr( stabilization_glsparameter_type const& stabGLSParameter,
                                   expression_convection_type const& exprConvection,
                                   expression_coeffdiffusion_type const& exprCoeffDiffusion )
        :
        M_stabGLSParameter( stabGLSParameter ),
        M_exprConvection( exprConvection ),
        M_exprCoeffDiffusion( exprCoeffDiffusion )
        {}

    stabilization_glsparameter_type const& stabGLSParameter() const { return M_stabGLSParameter; }
    expression_convection_type const& exprConvection() const { return M_exprConvection; }
    expression_coeffdiffusion_type const& exprCoeffDiffusion() const { return M_exprCoeffDiffusion; }

    template<typename Geo_t, typename Basis_i_t, typename Basis_j_t>
    struct tensor
    {
        typedef tensor<Geo_t,Basis_i_t,Basis_j_t> self_type;
        typedef typename mpl::if_<fusion::result_of::has_key<Geo_t,vf::detail::gmc<0> >, \
                                  mpl::identity<vf::detail::gmc<0> >,                         \
                                  mpl::identity<vf::detail::gmc<1> > >::type::type key_type;  \
        //typedef typename fusion::result_of::value_at_key<Geo_t,key_type>::type::element_type* gmc_ptrtype;
        typedef typename fusion::result_of::value_at_key<Geo_t,key_type>::type::element_type gmc_type;
        typedef typename gmc_type::value_type value_type;
        typedef  value_type evaluate_type;
        typedef Shape<gmc_type::NDim, Scalar, false,false> shape;
        struct is_zero { static const bool value = false; };

        typedef tensorBase<Geo_t, Basis_i_t, Basis_j_t,shape,value_type> tensorbase_type;
        typedef boost::shared_ptr<tensorbase_type> tensorbase_ptrtype;
        typedef typename tensorbase_type::matrix_shape_type matrix_shape_type;

        tensor( this_type const& expr,
                Geo_t const& geom, Basis_i_t const& fev, Basis_j_t const& feu )
            :
            self_type( expr,geom )
            {}
        tensor( this_type const& expr,
                Geo_t const& geom, Basis_i_t const& fev )
            :
            self_type( expr,geom )
            {}
        tensor( this_type const& expr, Geo_t const& geom )
            {
                if ( expr.stabGLSParameter().method() == "eigenvalue" )
                    M_tensorbase.reset( new tensorStabGLSParameterEigenValue<Geo_t, Basis_i_t, Basis_j_t, shape, this_type>(expr,geom) );
                else if ( expr.stabGLSParameter().method() == "doubly-asymptotic-approximation" )
                    M_tensorbase.reset( new tensorStabGLSParameterDoublyAsymptoticApproximation<Geo_t, Basis_i_t, Basis_j_t, shape, this_type>(expr,geom) );
                else
                    CHECK( false ) << "invalid method " << expr.stabGLSParameter().method();
            }
        tensor( tensor const& t )
            :
            M_tensorbase( t.M_tensorbase )
            {}

        template<typename IM>
        void init( IM const& im )
        {
            //M_tensor_expr.init( im );
        }
        void update( Geo_t const& geom, Basis_i_t const& /*fev*/, Basis_j_t const& /*feu*/ )
        {
            update(geom);
        }
        void update( Geo_t const& geom, Basis_i_t const& /*fev*/ )
        {
            update(geom);
        }
        void update( Geo_t const& geom )
        {
            M_tensorbase->update( geom );
        }
        void update( Geo_t const& geom, uint16_type face )
        {
            M_tensorbase->update( geom, face );
        }

        matrix_shape_type const&
        evalijq( uint16_type i, uint16_type j, uint16_type q ) const
        {
            return M_tensorbase->evalq( q );
        }
        value_type
        evalijq( uint16_type i, uint16_type j, uint16_type c1, uint16_type c2, uint16_type q ) const
        {
            return M_tensorbase->evalq( c1,c2,q );
        }

        value_type
        evaliq( uint16_type i, uint16_type c1, uint16_type c2, uint16_type q ) const
        {
            return M_tensorbase->evalq( c1,c2,q );
        }
        matrix_shape_type const&
        evaliq( uint16_type i, uint16_type q ) const
        {
            return M_tensorbase->evalq( q );
        }

        value_type
        evalq( uint16_type c1, uint16_type c2, uint16_type q ) const
        {
            return M_tensorbase->evalq( c1,c2,q );
        }
        matrix_shape_type const&
        evalq( uint16_type q ) const
        {
            return M_tensorbase->evalq( q );
        }

    private:
        tensorbase_ptrtype M_tensorbase;
    };

private :
    stabilization_glsparameter_type const& M_stabGLSParameter;
    expression_convection_type const& M_exprConvection;
    expression_coeffdiffusion_type const& M_exprCoeffDiffusion;


};

template<bool HasConvectionExpr=true,bool HasCoeffDiffusionExpr=true,int QuadOrder=-1,typename StabilizationGLSParameterType,typename ExprConvectionType, typename ExprCoeffDiffusionType >
inline
Expr< StabilizationGLSParameterExpr<StabilizationGLSParameterType,ExprConvectionType,ExprCoeffDiffusionType,HasConvectionExpr,HasCoeffDiffusionExpr,QuadOrder > >
stabilizationGLSParameterExpr( StabilizationGLSParameterType const& stabilizationGLSParameter,
                               ExprConvectionType const& exprConvection,
                               ExprCoeffDiffusionType const& exprCoeffDiffusion )
{
    typedef StabilizationGLSParameterExpr<StabilizationGLSParameterType,ExprConvectionType,ExprCoeffDiffusionType,HasConvectionExpr,HasCoeffDiffusionExpr,QuadOrder> expr_stabgls_parameter_t;
    return Expr< expr_stabgls_parameter_t >(  expr_stabgls_parameter_t( stabilizationGLSParameter,exprConvection,exprCoeffDiffusion ) );
}


} // namespace FeelModels
} // namespace vf
} // namespace Feel

#endif // FEELPP_MODELS_VF_STABILIZATION_GLS_PARMATER_EXPR_H
