/* Code created by Alexander Agathos feel free to use / modify
 * the three triangulations below are in the debug folder (tffd\tffd\x64\Debug)
 * feel free to change the path and adjust accordingly the path to the meshes
 */

#include <iomanip>
#include <iostream>
#include "tFFDdefs.h"
#include "tFFD.h"

using tFFD::MeshtFFD;
using tFFD::TFFD;

int main()
{
	const MeshtFFD meshPrototype(R"(1st_akraia_falagga_L100_W100_H100_B0_T0.off)");
	const MeshtFFD meshControl(R"(1st_akraia_falagga_L100_W100_H100_B0_T0_simp.off)");
	const MeshtFFD meshControlDeformed(R"(1st_akraia_falagga_L93_W93_H93_B-1_T-4_simp.off)");
	TFFD tffd(meshPrototype, meshControl, 2.5);
	MeshtFFD deformed = tffd.deformMesh(meshControlDeformed, meshPrototype);
	deformed.writeMesh("Test.off");
}