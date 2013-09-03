#include "../types.h"
#include "../dinamic.h"
#include "check.h"
#include "stdio.h"

START_TEST(test_GetDistance)
{
	Molecule* firstMol = malloc( sizeof(Molecule) );
	Molecule* secondMol = malloc( sizeof(Molecule) );

	firstMol->pos[0] = 0.0;
	firstMol->pos[1] = 0.0;
	secondMol->pos[0] = 3.0;
	secondMol->pos[1] = 4.0;

	float trueDistance = 5.0;
	float epsilon = 0.0000000001;

	float sizeX = 10;
	float sizeY = 10;
	ck_assert_msg( fabs(GetDistance(firstMol, secondMol, sizeX, sizeY, DEFAULT) - trueDistance) < epsilon );
}
END_TEST

// START_TEST(test_ComputeForce)
// {
// 	float r = 2.0;
// 	float sigma = 0.3165555;
// 	float eps = 0.6501696;
// 	float trueForce = -0.00012266296447043247; /*thanks python*/

// 	float epsilon = 0.0000000001;

// 	ck_assert_msg( fabs(ComputeForce(r,sigma,eps) - trueForce) < epsilon );
// }
// END_TEST

START_TEST(test_ClearForceAccel)
{
	float sizex = 10;
	float sizey = 10;
	float maxRandomSpeed = 3;
	int moleculesNum = 10;

	Cell* cell = Cell_random_create(moleculesNum, sizex, sizey, maxRandomSpeed);
	// setting up force and accel in our cell
	for (int i = 0; i < moleculesNum; ++i)
	{
		cell->molecules[i]->force[0] = i;
		cell->molecules[i]->force[1] = i;
		cell->molecules[i]->accel[0] = i;
		cell->molecules[i]->accel[1] = i;
	}

	ClearForceAccel(cell);
	// checking force = 0 and accel = 0 in each molecule in cell
	float check = 0.0;
	for (int i = 0; i < moleculesNum; ++i)
	{
		check += cell->molecules[i]->force[0];
		check += cell->molecules[i]->force[1];
		check += cell->molecules[i]->accel[0];
		check += cell->molecules[i]->accel[1];
	}

	ck_assert_msg( check == 0.0 );
}
END_TEST

// START_TEST(test_CorrectPosition)
// {
// 	float valueX = -175.25;
// 	float sizeX = 10;
// 	float sizeY = 10;
// 	float maxRandomSpeed = 3;
// 	Molecule* mol = Molecule_random_create(sizeX, sizeY, maxRandomSpeed);
// 	mol->pos[0] = valueX;

// 	float trueValueX = -5.25;
// 	float trueValueY = mol->pos[1];

// 	CorrectPosition(mol, sizeX, sizeY);

// 	ck_assert_msg(trueValueX == mol->pos[0]);
// 	ck_assert_msg(trueValueY == mol->pos[1]);
// }
// END_TEST


// START_TEST(test_CorrectPosition_2)
// {
// 	float valueX = 175.25;
// 	float sizeX = 10;
// 	float sizeY = 10;
// 	float maxRandomSpeed = 3;
// 	Molecule* mol = Molecule_random_create(sizeX, sizeY, maxRandomSpeed);
// 	mol->pos[0] = valueX;

// 	float trueValueX = 5.25;
// 	float trueValueY = mol->pos[1];

// 	CorrectPosition(mol, sizeX, sizeY);

// 	ck_assert_msg(trueValueX == mol->pos[0]);
// 	ck_assert_msg(trueValueY == mol->pos[1]);
// }
// END_TEST

Suite* dinamic_suit(void)
{
	Suite* s = suite_create("Dinamic");

	/*Core test case*/

	TCase* tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_GetDistance);
	// tcase_add_test (tc_core, test_ComputeForce);
	tcase_add_test (tc_core, test_ClearForceAccel);
	// tcase_add_test(tc_core, test_CorrectPosition);
	// tcase_add_test(tc_core, test_CorrectPosition_2);
	suite_add_tcase(s, tc_core);

	return s;
} 

int main(int argc, char const *argv[])
{
	int number_failed;
	Suite* s = dinamic_suit();
	SRunner* sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}