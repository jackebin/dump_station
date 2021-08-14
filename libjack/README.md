	added "tasker" to libjack.
	
	back to school!
	
	I made a "taskmanager" of some sort for managing all tasks and appointments during upcoming studies.
	it started out as a doubly linked list but ended up as a "dynamic array" because I didn't managed to
	wove the linked list properly with fwrite/fread :/ However - it was a fun, instructive little project!

	the core logic throughout the program is to copy the original data to a buffer, kill original, then
	rebuild it with the buffer as blueprint (with or without changes) before rewriting files. this repeats
	over and again until the user quits or everything crashes. the program is actually kinda stable though.
	
	Coded most on the fly why I'm from now on definitely going to PLAN my projects better before I begin but
	even though I kill and rebuild connections all the time in a weird manner I kinda like it, the feeling of
	everything being get, set, written and killed before "starting over". At least Valgrind doesn't complain.
	
	most noobish solution among others might be storing data in two files. long story short - amateurism,
	and these files are still hard coded. Mark Tasks as completed works but barely userfriendly and quite
	prone to bugs because of how id:s being updated at the moment but at least it's a start.
