/* Code created by Alexander Agathos feel free to use / modify
* The class TFFD implements the basic deformation t-FFD algorithm presented in 
* K. G. Kobayashi, K. Ootsubo, t-ffd: Free-form deformation by using triangular mesh, 
* in: Proceedings of the Symposium on Solid Modeling and Applications, 2003, pp. 226–234
* It receives as input the Prototype triangulation and Control Triangulation and has 
* a public method deform which deforms a mesh according to the change of the control mesh
*/


#pragma once

#include <iostream>
#include <iomanip>
#include "tFFDdefs.h"
#include <armadillo>
#include <cassert>

namespace tFFD
{
	using arma::cube;
	using arma::mat;

	class TFFD
	{
	private:
		cube tripEf;
		void acquireCoordinateTripletsEffect(const vector<vec>* pntsToMap, const vector<vec>* pntsControl,
			const vector<Triangle>* trisControl, double beta);
		constexpr static double eps = 1e-10;
	public:
		TFFD(const MeshtFFD& inPrototype, const MeshtFFD& inControlPrototype, double beta) : 
		tripEf(cube(static_cast<arma::uword>(inPrototype.getPnts()->size()), 
			static_cast<arma::uword>(inControlPrototype.getTris()->size()), 4))
		{
			acquireCoordinateTripletsEffect(inPrototype.getPnts(), inControlPrototype.getPnts(), inControlPrototype.getTris(),
				beta);
		}
		
		MeshtFFD deformMesh(const MeshtFFD& ControlDeformed, const MeshtFFD& Prototype);
	};
}