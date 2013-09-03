#include <check.h>
#include "../types.h"
#include <stdlib.h>

#define SUITE_NAME "Molecule"

/* ----------------------------------------------------------------- */
/* TESTS */

START_TEST(test_molecule_create)
{
	float xmaxSize = 10;
	float ymaxSize = 10;
	float maxSpeed = 2;

	Molecule* mol = Molecule_random_create(xmaxSize,ymaxSize,maxSpeed);

	ck_assert_msg( mol->pos[0] < xmaxSize && mol->pos[1] < ymaxSize, "Molecule random position > max size" );
	ck_assert_msg( mol->pos[0] > 0 && mol->pos[1] > 0, "Molecule random position < 0" );
	ck_assert_msg( abs( mol->speed[0] ) <= maxSpeed && abs( mol->speed[1] ) <= maxSpeed, "Molecule speed not in range from -maxSpeed to maxSpeed");
}
END_TEST



Suite * create_suite(void) {
    Suite *s = suite_create( SUITE_NAME );

    /* Create test cases against library API */
    TCase *tc_core = tcase_create ("Core");
    tcase_add_test(tc_core, test_molecule_create);
    suite_add_tcase(s, tc_core);

    return s;
}

int main( void ) {
    int num_fail;
    Suite *s = create_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all (sr, CK_NORMAL);
    num_fail = srunner_ntests_failed (sr);
    srunner_free (sr);
    return (num_fail == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}