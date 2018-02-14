#ifndef _DEFS_H_
#define _DEFS_H_

#include <armadillo>
#include <vector>

namespace tFFD {
	using arma::vec;
	using std::vector;
	using std::string;

	//Triangle of a mesh
	struct Triangle {
		int i;
		int j;
		int k;
	};

	//Read a mesh from a file
	//Create a mesh from std vector
	//In this class we do not allow the points or triangles to change once created
	class MeshtFFD {
	private:
		vector<vec>* pnts = nullptr; //The point cloud
		vector<Triangle>* tris = nullptr; //The triangulation
	public:
		MeshtFFD(const string& fileName); //Read from an OFF file
		MeshtFFD(const vector<vec>* in_pnts, const vector<Triangle>* in_tris);

		MeshtFFD(const MeshtFFD& mesh);
		MeshtFFD& operator=(const MeshtFFD& mesh);
		MeshtFFD(MeshtFFD&& mesh) = delete;
		MeshtFFD& operator=(const MeshtFFD&& mesh) = delete;
		const vector<vec>* getPnts() const {
			return pnts;
		}
		const vector<Triangle>* getTris() const {
			return tris;
		}

		virtual ~MeshtFFD() {
			delete pnts;
			delete tris;
		}
	};
}

#endif