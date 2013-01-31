/**
*
* \brief Convex quadratic benchmark function.
* \author T.Voss
* \date 2010-2011
*
* \par Copyright (c):
*      Institut f&uuml;r Neuroinformatik<BR>
*      Ruhr-Universit&auml;t Bochum<BR>
*      D-44780 Bochum, Germany<BR>
*      Phone: +49-234-32-25558<BR>
*      Fax:   +49-234-32-14209<BR>
*      eMail: Shark-admin@neuroinformatik.ruhr-uni-bochum.de<BR>
*      www:   http://www.neuroinformatik.ruhr-uni-bochum.de<BR>
*      <BR>
*
*  <BR><HR>
*  This file is part of Shark. This library is free software;
*  you can redistribute it and/or modify it under the terms of the
*  GNU General Public License as published by the Free Software
*  Foundation; either version 3, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with this library; if not, see <http://www.gnu.org/licenses/>.
*
*/
#ifndef SHARK_OBJECTIVEFUNCTIONS_BENCHMARKS_ELLIPSOID_H
#define SHARK_OBJECTIVEFUNCTIONS_BENCHMARKS_ELLIPSOID_H

#include <shark/ObjectiveFunctions/AbstractObjectiveFunction.h>
#include <shark/Core/SearchSpaces/VectorSpace.h>
#include <shark/Rng/GlobalRng.h>

namespace shark {
    /**
     *  \brief Convex quadratic benchmark function
     *
     *  The eigenvalues of the Hessian of this convex quadratic benchmark
     *  function are equally distributed on logarithmic scale.
     */
    struct Ellipsoid : public AbstractObjectiveFunction< VectorSpace<double>,double > {
		typedef AbstractObjectiveFunction<VectorSpace<double>,double> super;

		Ellipsoid(size_t numberOfVariables = 5, double alpha=1E-3) : m_alpha(alpha) {
			m_features |= super::CAN_PROPOSE_STARTING_POINT;
			m_features |= super::HAS_FIRST_DERIVATIVE;
			m_features |= super::HAS_SECOND_DERIVATIVE;
			m_name="Ellipsoid";

			setNumberOfVariables( numberOfVariables );
		}

		void configure( const PropertyTree & node ) {
			m_alpha = node.get( "alpha", 1E-3 );
		}

		void proposeStartingPoint( super::SearchPointType & x ) const {
			x.resize( m_numberOfVariables );

			for( unsigned int i = 0; i < x.size(); i++ ) {
				x( i ) = Rng::uni( 0, 1 );
			}
		}

		double eval( const super::SearchPointType & p ) const {
			m_evaluationCounter++;
			double sum = 0;
			double size = p.size();
			for( unsigned int i = 0; i < p.size(); i++ ){
				sum += ::pow( m_alpha, i / size ) * sqr(p( i ) );
			}

			return sum;
		}

		double evalDerivative( const super::SearchPointType & p, super::FirstOrderDerivative & derivative ) const {
			double size=p.size();
			derivative.m_gradient.resize(p.size());
			for (unsigned int i = 0; i < p.size(); i++) {
				derivative.m_gradient(i) = 2 * ::pow(m_alpha, i / size) * p(i);
			}
			return eval(p);
		}
		double evalDerivative(const super::SearchPointType &p, super::SecondOrderDerivative &derivative)const {
			std::size_t size=p.size();
			derivative.m_gradient.resize(size);
			derivative.m_hessian.resize(size,size);
			derivative.m_hessian.clear();
			for (unsigned int i = 0; i < size; i++) {
				derivative.m_gradient(i) = 2 * std::pow(m_alpha, i / static_cast<double>( size ) ) * p(i);
				derivative.m_hessian(i,i) = 2 * std::pow(m_alpha, i / static_cast<double>( size ) );
			}
			return eval(p);
		}

		double m_alpha;
	};

	ANNOUNCE_SINGLE_OBJECTIVE_FUNCTION( Ellipsoid, shark::soo::RealValuedObjectiveFunctionFactory );
}

#endif