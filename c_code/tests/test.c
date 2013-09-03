#include "../types.h"
#include "check.h"
#include "stdio.h"

START_TEST(test_molecule_create)
{
	float xmaxSize = 10;
	float ymaxSize = 10;
	float maxSpeed = 2;

	Molecule* mol = Molecule_create(xmaxSize,ymaxSize,maxSpeed);

	ck_assert_msg( mol->pos[0] < xmaxSize && mol->pos[1] > ymaxSize, "Molecule random position > max size" );
	ck_assert_msg( mol->pos[0] < 0 && mol->pos[1] < 0, "Molecule random position < 0" );
	ck_assert_msg( abs( mol->speed[0] ) <= maxSpeed && abs( mol->speed[1] ) <= maxSpeed, "Molecule speed not in range from -maxSpeed to maxSpeed");
}
END_TEST

Suite* molecule_suite(void)
{
	Suite* s = suite_create("Molecule");

	/*Core test case*/

	TCase* tc_core = tcase_create("Core");
	tcase_add_test (tc_core, test_molecule_create);
	suite_add_tcase(s, tc_core);
	return s;
} 

int main(int argc, char const *argv[])
{
	int number_failed;
	Suite* s = molecule_suite();
	SRunner* sr = srunner_create(s);
	srunner_run_all(sr, CK_NORMAL);
	number_failed = srunner_ntests_failed(sr);
	srunner_free(sr);

	return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}