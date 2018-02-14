// Code created by Alexander Agathos feel free to use / modify

#include "tFFD.h"

namespace tFFD {
	//deform the mesh prototype given the changed controlDeformed mesh
	MeshtFFD TFFD::deformMesh(const MeshtFFD& controlDeformed, const MeshtFFD& prototype)
	{
		const vector<vec>* pntsDefContr = controlDeformed.getPnts();
		const vector<Triangle>* trisDefContr = controlDeformed.getTris();

		const int numbPntsDef = (int)prototype.getPnts()->size();
		const int numbTrisDefContr = (int)(*trisDefContr).size();
		double* sumk = new double[numbPntsDef];
		
		for (int q = 0; q < numbPntsDef; q++) sumk[q] = 0.0;
		
		vector<vec>* pntsProtDef = new vector<vec>(numbPntsDef, vec(3));
		for (int i = 0; i < numbPntsDef; i++) (*pntsProtDef)[i].zeros();

		for (int i = 0; i < numbTrisDefContr; i++) {
			const vec pi = (*pntsDefContr)[(*trisDefContr)[i].i];
			const vec p1i = (*pntsDefContr)[(*trisDefContr)[i].j];
			const vec p2i = (*pntsDefContr)[(*trisDefContr)[i].k];

			const vec Ui = p1i - pi;
			const vec Vi = p2i - pi;

			if (norm(Ui) > eps && norm(Vi) > eps) {
				vec Wi = cross(Ui, Vi);
				Wi = Wi / norm(Wi);
				for (int q = 0; q < numbPntsDef; q++) {
					const vec qi = pi + tripEf(q, i, 0)*Ui + tripEf(q, i, 1)*Vi + tripEf(q, i, 2)*Wi;
					const double ki = tripEf(q, i, 3);
					sumk[q] += ki;
					(*pntsProtDef)[q] += ki*qi;
				}
			}
		}
		for (int q = 0; q < numbPntsDef; q++) {
			(*pntsProtDef)[q] /= sumk[q];
		}

		MeshtFFD mesh(pntsProtDef, prototype.getTris());
		delete pntsProtDef;
		delete[] sumk;
		return mesh;
	}

	//This is the main computational intensive task and needs to be computed only once,
	//when the constructor is called the mesh can be deformed then as many times the user wants
	void TFFD::acquireCoordinateTripletsEffect(const vector<vec>* pntsToMap, const vector<vec>* pntsControl,
		const vector<Triangle>* trisControl, double beta) {

		//Create the Vectors Ui, Vi, Wi for each q of pntsToMap
		//Look at the paper of t-FFD for formulas
		int numbPntsToMap = (int)(*pntsToMap).size();
		int numbTrisControl = (int)(*trisControl).size();
		//for (int q = 0; q < numbPntsToMap; q++)
		for (int i = 0; i < numbTrisControl; i++) {

			vec pi = (*pntsControl)[(*trisControl)[i].i];
			vec p1i = (*pntsControl)[(*trisControl)[i].j];
			vec p2i = (*pntsControl)[(*trisControl)[i].k];

			vec Ui = p1i - pi;
			vec Vi = p2i - pi;

			if (norm(Ui) > eps && norm(Vi) > eps) {
				//printf("%d ", i);
				vec Wi = cross(Ui, Vi);
				Wi = Wi / norm(Wi);
				vec Gi = (pi + p1i + p2i) / 3.0;
				double ri = (norm(pi - Gi) + norm(p1i - Gi) + norm(p2i - Gi)) / 3.0;
				mat A(3, 3);
				A(0, 0) = Ui[0]; A(0, 1) = Vi[0]; A(0, 2) = Wi[0];
				A(1, 0) = Ui[1]; A(1, 1) = Vi[1]; A(1, 2) = Wi[1];
				A(2, 0) = Ui[2]; A(2, 1) = Vi[2]; A(2, 2) = Wi[2];

				for (int q = 0; q < numbPntsToMap; q++) {
					vec b = (*pntsToMap)[q] - pi;
					//GET THE TRIPLETS ui, vi, wi in the sol vec
					vec sol = solve(A, b);

					// CALCULATE THE EFFECT ki	
					double normqGi = norm((*pntsToMap)[q] - Gi);
					//CALCULATE THE EFFECT FACTOR
					double ki = normqGi < beta*ri ? 1 - normqGi / (beta*ri) : 0.0;
					//if (!q) printf("%lf ", ki);
					tripEf(q, i, 0) = sol[0];
					tripEf(q, i, 1) = sol[1];
					tripEf(q, i, 2) = sol[2];
					tripEf(q, i, 3) = ki;
				}
			}
			else {
				for (int q = 0; q < numbPntsToMap; q++) {
					tripEf(q, i, 0) = 0.0;
					tripEf(q, i, 1) = 0.0;
					tripEf(q, i, 2) = 0.0;
					tripEf(q, i, 3) = 0.0;
				}
			}
		}

	}
}