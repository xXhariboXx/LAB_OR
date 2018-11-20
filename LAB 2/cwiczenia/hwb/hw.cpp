#include <assert.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <thread>

#include "Integral.h"
#include "ParallelExercise.h"

#pragma warning (disable: 4700)  //warning C4700: uninitialized local variable 'prv' used
#pragma warning (disable: 4127)  //warning C4127: conditional expression is constant

const int defaultDelay = 10000000;

void doSth (unsigned int n);
void header (char * s);
void footer (void);
void printArr (const char * txtBefore, int a[], const int cnt, const char * txtAfter);

void PAR_get_num (){

	header("PAR_get_num");

	printf("omp_get_num_threads () == %d\n", omp_get_num_threads());
	printf("omp_get_thread_num  () == %d\n", omp_get_thread_num ());

	footer();
}

void PAR_parallel (){

	header("PAR_parallel");

	#pragma omp parallel
	{
		printf("omp_get_num_threads () == %d\n", omp_get_num_threads());
		printf("omp_get_thread_num  () == %d\n", omp_get_thread_num ());
	}

	footer();
}
void PAR_num_threads(int numThreads, int numThreads2){

	header("PAR_num_threads");

	#pragma omp parallel
	printf("[1] Thread #%d of %d\n", omp_get_thread_num (), omp_get_num_threads());

	putchar('\n');

	omp_set_num_threads(numThreads);

    #pragma omp parallel
	printf("[2] Thread #%d of %d\n", omp_get_thread_num (), omp_get_num_threads());

	putchar ('\n');

    #pragma omp parallel num_threads (numThreads2)
	printf("[3] Thread #%d of %d\n", omp_get_thread_num (), omp_get_num_threads());

	putchar ('\n');

    #pragma omp parallel
	printf("[4] Thread #%d of %d\n", omp_get_thread_num (), omp_get_num_threads());

	footer();
}

void PAR_in_parallel (){

	header("PAR_in_parallel");

	printf ("omp_in_parallel () == %d\n", omp_in_parallel ());

    #pragma omp parallel num_threads (1)
	printf ("omp_in_parallel () == %d\n", omp_in_parallel ());
		
	footer();
}

void PAR_dynamic (int numThreads){

	header("PAR_dynamic");

	int dynamic = omp_get_dynamic ();

	omp_set_dynamic (0);
	printf("omp_get_dynamic () == %d\n", omp_get_dynamic());

    #pragma omp parallel num_threads (numThreads)
	printf("Thread #%d of %d\n", omp_get_thread_num (), omp_get_num_threads());

	omp_set_dynamic (1);
	printf("omp_get_dynamic () == %d\n", omp_get_dynamic());

    #pragma omp parallel num_threads (numThreads)
	printf("Thread #%d of %d\n", omp_get_thread_num (), omp_get_num_threads());
	
	omp_set_dynamic (dynamic);

	footer();
}

void WS_for_basic (int numThreads, int numLoops){

	int i;

	header ("WS_for_basic");

	#pragma omp parallel for num_threads (numThreads)
	for (i = 0; i < numLoops; i++)
		printf ("Loop #%d executed by thread %d of %d\n", i, omp_get_thread_num (), omp_get_num_threads());

	footer();
}

void WS_for_schedule (int numThreads, int numLoops, int chunk){

	int i;
	const int delay = defaultDelay;
	int * tn = (int *)malloc (sizeof(int) * numLoops);
	assert (tn);

	header ("WS_for_schedule (may take a while ...)");

	printf ("\nSchedule static for chunk %d\n", chunk);

	for (i = 0; i < numLoops; i ++) 
		tn[i] = -1;

	#pragma omp parallel for schedule(static, chunk) num_threads (numThreads)
	for (i = 0; i < numLoops; i++){
		doSth(delay);
		tn[i] = omp_get_thread_num ();
	}

	for (i = 0; i < numLoops; i ++)
		printf ("%d ", tn[i]);
	putchar ('\n');

	printf ("\nSchedule dynamic for chunk %d\n", chunk);

	for (i = 0; i < numLoops; i ++)	
		tn[i] = -1;

	#pragma omp parallel for schedule(dynamic, chunk) num_threads (numThreads)
	for (i = 0; i < numLoops; i++){
		doSth(delay);
		tn[i] = omp_get_thread_num ();
	}

	for (i = 0; i < numLoops; i ++)
		printf ("%d ", tn[i]);
	putchar ('\n');

	printf ("\nSchedule guided for chunk %d\n", chunk);

	for (i = 0; i < numLoops; i ++)	
		tn[i] = -1;

	#pragma omp parallel for schedule(guided, chunk) num_threads (numThreads)
	for (i = 0; i < numLoops; i++){
		doSth(delay);
		tn[i] = omp_get_thread_num ();
	}

	for (i = 0; i < numLoops; i ++)
		printf ("%d ", tn[i]);
	putchar ('\n');

	free (tn);

	footer();
}

void WS_section_basics (int numThreads){

	header ("WS_section_basics");

	#pragma omp parallel sections num_threads (numThreads)
	{
		printf ("This code has been executed by thread %d of %d\n", omp_get_thread_num (), omp_get_num_threads());

		#pragma omp section
		printf ("This code has been executed by thread %d of %d\n", omp_get_thread_num (), omp_get_num_threads());

		#pragma omp section
		{
			printf ("This code has been executed by thread %d of %d\n", omp_get_thread_num (), omp_get_num_threads());
		}
		#pragma omp section
		{
			printf ("This code has been executed by thread %d of %d\n", omp_get_thread_num (), omp_get_num_threads());
		}
		#pragma omp section
		{
			printf ("This code has been executed by thread %d of %d\n", omp_get_thread_num (), omp_get_num_threads());
		}
		#pragma omp section
		{
			printf ("This code has been executed by thread %d of %d\n", omp_get_thread_num (), omp_get_num_threads());
		}
	} 

	footer();
}

void DA_defaults_std (int numThreads, int numLoops, int numLoops2){
	int i;
	int shrd = 1984;
	int shrd2 = 0;

	header ("DA_defaults_std");	

	#pragma omp parallel for num_threads (numThreads)
	for (i = 0; i < numLoops; i++){
		int j = 2 * i;

		printf ("Thread #%d: i == %d shrd == %d j == %d\n", omp_get_thread_num (), i, shrd, j);
		
	}

	putchar('\n');

	#pragma omp parallel for num_threads (numThreads)
	for (i = 0; i < numLoops2; i++){
		shrd2 += i;
		
		shrd2 -= i;
	}

	printf ("shrd2 == %d\n", shrd2);

	footer();
}

void DA_selective (int numThreads, int numLoops){
	int i, 
		prv = 1984, 
		frstPrv = 1984, 
		lstPrv = 1984,
		flPrv = 1984;

	header ("DA_selective");	

	printf ("Before parallel:prv == %11d lstPrv == %11d frstPrv == %5d flPrv == %5d\n\n", \
		prv, lstPrv, frstPrv, flPrv);

	#pragma omp parallel for num_threads (numThreads) private(prv) firstprivate(frstPrv, flPrv) \
		lastprivate(lstPrv, flPrv)
	for (i = 0; i < numLoops; i++){
		printf ("Thread #%d[1]: \tprv == %11d lstPrv == %11d frstPrv == %5d flPrv == %5d\n", \
			omp_get_thread_num (), prv, lstPrv, frstPrv, flPrv);

		prv = lstPrv = frstPrv = flPrv = i;
		
		printf ("Thread #%d[2]: \tprv == %11d lstPrv == %11d frstPrv == %5d flPrv == %5d\n", \
			omp_get_thread_num (), prv, lstPrv, frstPrv, flPrv);
	}
	printf ("\nAfter parallel: prv == %11d lstPrv == %11d frstPrv == %5d flPrv == %5d\n", \
			prv, lstPrv, frstPrv, flPrv);

	footer();
}

int thrPrv = 1984;
#pragma omp threadprivate(thrPrv)

void DA_threadprivate (int numThreads){
	int dynamic = omp_get_dynamic ();
	int prv = 1984;
	header("DA_threadprivate");

	omp_set_dynamic (0);
	printf ("STATE 0: \tprv == %10d thrPrv == %10d\n", prv, thrPrv);

	#pragma omp parallel num_threads (numThreads) private (prv)
	{
		int i = omp_get_thread_num();
		printf ("Thread #%d[1]: \tprv == %10d thrPrv == %10d\n", i, prv, thrPrv);
		prv = i;
		thrPrv = 10 + i;
		printf ("Thread #%d[2]: \tprv == %10d thrPrv == %10d\n", i, prv, thrPrv);
	}
	printf ("STATE 1: \tprv == %10d thrPrv == %10d\n", prv, thrPrv);

	#pragma omp parallel num_threads (numThreads) private (prv)
	{
		printf ("Thread #%d[1]: \tprv == %10d thrPrv == %10d\n", omp_get_thread_num (), prv, thrPrv);
	}
	printf ("STATE 2: \tprv == %10d thrPrv == %10d\n", prv, thrPrv);

	numThreads ++;
	#pragma omp parallel num_threads (numThreads) private (prv) //copyin(thrPrv) 
	// copyin - copy the value of the master thread’s variable thrPrv to the corresponding threadprivate variables
	{
		printf ("Thread #%d[1]: \tprv == %10d thrPrv == %10d\n", omp_get_thread_num (), prv, thrPrv);
	}

	omp_set_dynamic (dynamic);

	footer();
}

void R_reduction (int numThreads, int numLoops){
	
	int i, plus = 10, mult = 10, prv = 10;

	header("R_reduction");

	printf ("STATE 0: \tprv == %10d plus == %10d mult == %10d\n", prv, plus, mult);

	#pragma omp parallel for num_threads (numThreads) private(prv) reduction(+: plus) reduction(*: mult)
	for (i = 0; i < numLoops; i ++){
		int j = i + 1;
		printf ("Thread #%d[a]: \tprv == %10d plus == %10d mult == %10d j == %d\n", \
			omp_get_thread_num (), prv, plus, mult, j);
		plus = plus + j; 
		mult = mult * j;
		prv = j;
		printf ("Thread #%d[b]: \tprv == %10d plus == %10d mult == %10d j == %d\n", \
			omp_get_thread_num (), prv, plus, mult, j);
	}

	printf ("STATE 1: \tprv == %10d plus == %10d mult == %10d\n", prv, plus, mult);

	footer();
}

void SNC_for (int numThreads){
	int i;
	unsigned delay = 30 * defaultDelay;

	header ("SNC_for (may take a while ...)");
	#pragma omp parallel num_threads (numThreads) 
	{
		#pragma omp for 
		for (i = 0; i < numThreads; i++)
			doSth(i * delay);
		
		printf("Thread #%d after for #1\n", omp_get_thread_num());

		#pragma omp for nowait
		for (i = 0; i < numThreads; i++) 
			doSth(i * delay);
		
		printf("Thread #%d after for #2\n", omp_get_thread_num());

		#pragma omp for schedule (dynamic, 1)
		for (i = 0; i < numThreads; i++) 
			printf("Thread #%d is doing for #3\n", omp_get_thread_num());		

		printf("Thread #%d after for #3\n", omp_get_thread_num());
	}

	footer();
}

void SNC_sections (){
	unsigned delay = 90 * defaultDelay;

	header ("SNC_sections");
	#pragma omp parallel num_threads (3) 
	{
		#pragma omp sections
		{
			doSth(omp_get_thread_num() * delay);

			#pragma omp section
			doSth(omp_get_thread_num() * delay);

			#pragma omp section
			doSth(omp_get_thread_num() * delay);
		}

		printf("Thread #%d after sections #1\n", omp_get_thread_num());

		#pragma omp sections nowait
		{
			doSth(omp_get_thread_num() * delay);

			#pragma omp section
			doSth(omp_get_thread_num() * delay);

			#pragma omp section
			doSth(omp_get_thread_num() * delay);
		}

		printf("Thread #%d after sections #2\n", omp_get_thread_num());

		#pragma omp sections nowait
		{
			printf("Thread #%d in sections #3\n", omp_get_thread_num());

			#pragma omp section
			printf("Thread #%d in sections #3\n", omp_get_thread_num());

			#pragma omp section
			printf("Thread #%d in sections #3\n", omp_get_thread_num());
		}

		printf("Thread #%d after sections #3\n", omp_get_thread_num());

	}

	footer();
}

void SNC_mix (int numThreads){
	unsigned delay = 90 * defaultDelay;
	int i;

	header ("SNC_mix (may take a while...)");
	#pragma omp parallel num_threads (3) 
	{
		#pragma omp for nowait
		for (i = 0; i < numThreads; i++) 
			doSth(i * delay);
		
		printf("Thread #%d after for\n", omp_get_thread_num());

		#pragma omp sections nowait
		{
			printf("Thread #%d in sections\n", omp_get_thread_num());

			#pragma omp section
			printf("Thread #%d in sections\n", omp_get_thread_num());

			#pragma omp section
			printf("Thread #%d in sections\n", omp_get_thread_num());
		}
		printf("Thread #%d after sections\n", omp_get_thread_num());
	}
	footer();
}

void SNC_master_single(int numThreads, int numExp){
	
	int single1, single2;
	int master;
	int i, j;

	header ("SNC_master_single (may take a while...)");

	//header of the work table
	printf ("Work assigment\n");
	printf ("Run #\t Amount of work\t\t Single1 Single2 Master\n");
	printf ("\t");
	for (j = 0; j < numThreads; j ++)
		printf("%2d ", j);
	printf("\n\n");

	//experiments
	for (i = 0; i < numExp; i ++){
		//printing work assigment in the current experiment
		printf ("%2d\t", i);
		for (j = 0; j < numThreads; j ++)
			printf("%2d ", (i + j) % numThreads);

		//concurrent execution
		#pragma omp parallel num_threads (numThreads)
		{
			doSth(((i + omp_get_thread_num()) % numThreads) * defaultDelay);
			#pragma omp single nowait
			{
				doSth (defaultDelay);
				single1 = omp_get_thread_num();
			}
			#pragma omp single
			{
				doSth (defaultDelay);
				single2 = omp_get_thread_num();
			}
			#pragma omp master
			master = omp_get_thread_num();
		}
		//printing IDs of threads executing single or master code
		printf ("%6d %6d %6d\n", single1, single2, master);
	}

	footer ();
}

void SNC_explicit_mutual_ex (int numThreads){
	int i, 
		atom = 0, 
		cs_even = 0, 
		cs_odd = 0;

	header ("SNC_explicit_mutual_ex");

	#pragma omp parallel for num_threads (numThreads)
	for (i = 0; i < numThreads; i ++)
		#pragma omp atomic
		atom += i;
	printf("atom == %d\n", atom);

	#pragma omp parallel for num_threads (numThreads)
	for (i = 0; i < numThreads; i ++){
		if (i % 2)
			#pragma omp critical (odd)
			cs_odd ++;
		else
			#pragma omp critical (even)
			cs_even ++;
	}
	printf("cs_odd == %d, cs_even == %d\n", cs_odd, cs_even);
		
	footer();
}

void SNC_explicit_barrier (int numThreads){
#define NUMTHREAD_MAX 1000

	int i, idx1, idx2, idx3;
	int delay =  10 * defaultDelay;	
	int * loop1 = new int[numThreads];
	int * loop2 = new int[numThreads];
	int * loop3 = new int[numThreads];

	header ("SNC_explicit_barrier (may take a while ...)");

	idx1 = idx2 = idx3 = 0;
	assert (numThreads < NUMTHREAD_MAX);
	#pragma omp parallel num_threads (numThreads)
	{
		#pragma omp for nowait schedule(dynamic)
		for (i = 0; i < numThreads; i ++){
			doSth((i + 1) * delay);
			#pragma omp critical 
			loop1[idx1 ++] = omp_get_thread_num();
		}

		#pragma omp for nowait schedule(dynamic)
		for (i = 0; i < numThreads; i ++)
			#pragma omp critical 
			loop2[idx2 ++] = omp_get_thread_num();

		#pragma omp barrier

		#pragma omp for schedule(dynamic)
		for (i = 0; i < numThreads; i ++){
			doSth(delay);
			#pragma omp critical 
			loop3[idx3 ++] = omp_get_thread_num();
		}
	}
	
	printArr("loop1: ", loop1, numThreads, "\n");
	printArr("loop2: ", loop2, numThreads, "\n");
	printArr("loop3: ", loop3, numThreads, "\n");
	delete[] loop1;
	delete[] loop2;
	delete[] loop3;
	footer();
#undef NUMTHREAD_MAX
}

void SNC_explicit_ordered (int numThreads, int numExp){
	int i, idx, idx2;
	int numLoops = numThreads;
	int * loop = new int[numLoops];
	int * loop2 = new int[numLoops];

	header ("SNC_explicit_ordered (may take a while...)");

	printf ("Seq\t\t Unordered 1\t Unordered 2\t Ordered\n");

	while (numExp --){
		//sequential execution
		for (idx = 0, i = 0; i < numThreads; i ++){
			doSth((numThreads - i) * defaultDelay);
			loop[idx ++] = i;
		}
		printArr ("", loop, numLoops, "\t");

		//concurrent unordered execution
		idx = 0;
		#pragma omp parallel for num_threads (numThreads)
		for (i = 0; i < numThreads; i ++){
			doSth((numThreads - i) * defaultDelay);
			#pragma omp critical
			loop[idx ++] = i;
		}
		printArr ("", loop, numLoops, "\t");

		//concurrent ordered execution
		idx = idx2 = 0;
		#pragma omp parallel for num_threads (numThreads) ordered
		for (i = 0; i < numThreads; i ++)
		{
			doSth((numThreads - i) * defaultDelay);

			#pragma omp critical
			loop2[idx2++] = i;

			doSth((numThreads - i) * defaultDelay);
			
			#pragma omp ordered 
			{
				#pragma omp critical
				loop[idx ++] = i;
			}
		}
	
		printArr ("", loop2, numLoops, "\t");
		printArr ("", loop, numLoops, "\n");
	}

	delete[] loop;
	delete[] loop2;
	footer();
}

void SNC_explicit_locks (int numThreads){
	header("SNC_explicit_locks");
	omp_lock_t lock;

	omp_init_lock(&lock);
	#pragma omp parallel num_threads (numThreads)
	{
		if (! omp_test_lock (&lock)) {
			printf ("From thread #%d: the lock is currently owned by another thread\n",  omp_get_thread_num());	
			omp_set_lock(&lock);
		}
		printf("Thread #%d owns now the lock\n", omp_get_thread_num());
		doSth(10 * defaultDelay);
		omp_unset_lock(&lock);
	}
	omp_destroy_lock (&lock);

	footer();
}

void veryTimeConsumingCode (int numLoops, int loop[], int * idx){
	int i;

	//an orphaned directive
	#pragma omp for 
	for(i = 0; i < numLoops; i ++){
		doSth(i * 10 * defaultDelay);
		#pragma omp critical
		loop[(*idx)++] = omp_get_thread_num();
	}
	return;
}

void BP_scope (int numThreads){
	int * loop = new int[numThreads];
	int idx = 0;

	header("BP_scope (may take a while ...)");
	veryTimeConsumingCode(numThreads, loop, &idx);
	printArr ("Sequential loop: ", loop, numThreads, "\n");

	idx = 0;
	#pragma omp parallel num_threads (numThreads)
	veryTimeConsumingCode(numThreads, loop, &idx);

	printArr ("Parallel loop: ", loop, numThreads, "\n");

	delete [] loop;
	footer();
	return;
}

void testNested (int numThreads, int numNestedThreads){
	int threadCnt = 0,
		nestedThreadCnt = 0;
	int n, nNested;
	int * nestedThreadID = new int[numThreads * numNestedThreads];
	int * threadID = new int[numThreads];

	if (omp_get_nested())
		printf ("Nested parallelism is enabled.\n");
	else
		printf ("Nested parallelism is disabled.\n");

	#pragma omp parallel num_threads (numThreads)
	{
		#pragma omp single
		n = omp_get_num_threads();

		#pragma omp critical
		threadID[threadCnt ++] = omp_get_thread_num();

		#pragma omp parallel num_threads (numNestedThreads)
		{
			#pragma omp single
			nNested = omp_get_num_threads();

			#pragma omp critical
			nestedThreadID[nestedThreadCnt ++] = omp_get_thread_num();
		}
	}
	printf("\n");
	printf ("\t threadCnt == %d \n", threadCnt);
	printf ("\t omp_get_num_threads returned %d\n", n);
	printArr ("\t ThreadID: ", threadID, threadCnt, "\n");
	printf("\n");
	printf ("\t nestedThreadCnt == %d \n", nestedThreadCnt);
	printf ("\t nested omp_get_num_threads returned %d\n", nNested);
	printArr ("\t nestedThreadID: ", nestedThreadID, nestedThreadCnt, "\n");
	printf("\n");
	delete [] threadID;
	delete [] nestedThreadID;
	return;
}
void BP_nested (int numThreads){

	int nested = omp_get_nested();

	header("BP_nested");
	if (omp_in_parallel()){
		printf("BP_nested is executed in parallel. Can't call omp_set_nested.Quitting :-(\n");
		return;
	}

	omp_set_nested (1);
	testNested (numThreads, numThreads + 1);

	omp_set_nested (0);
	testNested (numThreads, numThreads + 1);

	omp_set_nested (nested);

	footer();
}

int main()
{
	double result, timeSpent = 0;

	Integral integral;
	integral.SequentialOne();
	integral.SequentialTwo();
	integral.ParallelOne(result, timeSpent);
	integral.ParallelTwo(result, timeSpent);
	integral.ParallelThree(result, timeSpent);
	integral.ParallelFour(result, timeSpent);

	ParallelExercise parallelExercise(15, 15);
	parallelExercise.DoExercise();

	system("pause");
}
