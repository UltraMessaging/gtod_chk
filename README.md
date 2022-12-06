# gtod

Test program to see if gettimeofday() ever returns a bad tv_sec value.

Run the "tst.sh" script to test it with three threads.

````
Usage: gtod_chk [-h] [-t num_threads]
where:
-h - prints help.
-t threads - number of parallel threads to run.
````
