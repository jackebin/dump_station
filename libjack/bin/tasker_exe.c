#include <ljack/dbg.h>
#include <ljack/tasker.h>

int main(void) {

	char rc = 0;
	
	rc = tasker_start();
	check(rc == 0, "run went wrong somehow");
	

	return 0;
error:
	return 1;
}
