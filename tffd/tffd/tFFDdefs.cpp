// Code created by Alexander Agathos feel free to use / modify 
 
#include "tFFDdefs.h"

namespace tFFD {
	//Create a mesh object from the point clowd and triangulation
	MeshtFFD::MeshtFFD(const vector<vec>* in_pnts, const vector<Triangle>* in_tris) {
		pnts = new vector<vec>(*in_pnts);
		tris = new vector<Triangle>(*in_tris);
	}

	//Create a mesh object reading an OFF file
	//For details on OFF files please see the specs of it
	//This implementation reads the specific OFF file
	//OFF
	//#Points #Triangles #Edges
	//xi yi zi //coordinates of points
	//.......
	//3 i j k //triangle with point indices
	//........
	MeshtFFD::MeshtFFD(const string& fileName) {
		string dummys;
		int dummyi, numbPnts, numbTris;
		ifstream inMesh(fileName);
		if (!inMesh)
		{
			std::cout << "Mesh could not be opened for reading!" << std::endl;
			std::exit(1);
		}
		inMesh >> dummys;
		inMesh >> numbPnts >> numbTris >> dummyi;
		pnts = new vector<vec>(numbPnts, vec(3));
		tris = new vector<Triangle>(numbTris, Triangle());
		for (int i = 0; i < numbPnts; i++)
			inMesh >> (*pnts)[i][0] >> (*pnts)[i][1] >> (*pnts)[i][2];
		for (int t = 0; t < numbTris; t++)
			inMesh >> dummyi >> (*tris)[t].i >> (*tris)[t].j >> (*tris)[t].k;
	}

	//Copy constructor
	MeshtFFD::MeshtFFD(const MeshtFFD& mesh) {
		pnts = new vector<vec>(*mesh.getPnts());
		tris = new vector<Triangle>(*mesh.getTris());
	}

	//Assignment operator 
	MeshtFFD& MeshtFFD::operator=(const MeshtFFD& mesh)
	{
		if (this != &mesh)
		{
			delete pnts;
			delete tris;
			pnts = new vector<vec>(*mesh.getPnts());
			tris = new vector<Triangle>(*mesh.getTris());
		}
		return *this;
	}

	//Write a mesh file in OFF format
	void MeshtFFD::writeMesh(const string& outMeshFile) const
	{
		ofstream outMesh(outMeshFile);
		if (!outMesh)
		{
			std::cout << "Mesh could not be opened for writting!" << std::endl;
			std::exit(1);
		}
		outMesh << "OFF" << std::endl;
		outMesh << pnts->size() << " " << tris->size() << " " << 0 << std::endl;
		for (const auto& pnt : *pnts)
		{
			outMesh << pnt[0] << " " << pnt[1] << " " << pnt[2] << std::endl;
		}
		for (const auto& tri : *tris)
		{
			outMesh << 3 << " " << tri.i << " " << tri.j << " " << tri.k << std::endl;
		}
	}

}