	"tasker"
	
	back to school!

	I made a "taskmanager" of some sort for managing all tasks and appointments during upcoming studies.
	it started out as a doubly linked list but ended up as a "dynamic array" because I didn't managed to
	wove the linked list properly with fwrite/fread :/ However - it was a fun, instructive little project!

	the core logic throughout the program is to copy the original data to a buffer, kill original, then
	rebuild it with the buffer as blueprint (with or without changes) before rewriting files. this repeats
	over and again until the user quits or everything crashes. the program is actually kinda stable though.

	most noobish solution among others might be storing data in two files (because reasons) and these files
	are still hard coded. mark tasks as completed works but barely userfriendly and quite prone to bugs
	because of how id:s being updated at the moment but at least it's a start.

	---

	from earlier:

	"standard input"
	a module I made for taking care of basic input in my projects.
   
	"quest"
	quiz-application of some sort. created during my first encounter with static and dynamic libraries.
