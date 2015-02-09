#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/armpmu.h>

MODULE_AUTHOR("Priya Sundaresan");
MODULE_DESCRIPTION("Enables user-mode access to ARMv7 performance counters");
MODULE_LICENSE("Dual BSD/GPL");

static void cpucounter_enable(void* reserved) {
	/*
	int pmuserenr, pmcr;
	int x;
	
	write_pmintenclr(0x8000000Fu);
	write_pmcntenclr(0x8000000Fu);
	
	pmcr = read_pmcr();
	pmcr &= ~0x0000003Fu; //mask
	pmcr |= 0x00000017u;//E,P,C bits - 1
	write_pmcr(pmcr);

	write_pmcntenset(0x8000000Fu); 

	pmuserenr = read_pmuserenr();
	pmuserenr |= 0x00000001u;//EN bit - 1
	write_pmuserenr(pmuserenr);
	
	//Setting events for perf counters
	//For now - COunter 0 - instruction count
	write_pmnxsel(0);
	write_evtsel(0x08);
	*/

	unsigned int pmuserenr;
	//Disable Performance counters & interrupt enable clear for counters
	write_pmintenclr(0x8000000Fu);
	write_pmcntenclr(0x8000000Fu);	
	//Set event for each counter
	setCounterEvent(ARMV7_COUNTER0,ARMV7_DCACHE_REFILL);
	setCounterEvent(ARMV7_COUNTER1,ARMv7_BRANCH_MIS_PRED);
	setCounterEvent(ARMV7_COUNTER2,ARMV7_DREAD);
	setCounterEvent(ARMV7_COUNTER3,ARMV7_DWRITE);
	//Set cycle count tick rate
	//default - every cycle
	//Give user-enable access to counters
	pmuserenr = read_pmuserenr();
	pmuserenr |= 0x00000001u;//EN bit - 1
	write_pmuserenr(pmuserenr);
}


static void cpucounter_disable(void* reserved) {
	unsigned int pmuserenr, pmcr;
//testing
	//int x=read_pmcnt(1);
	//printk(KERN_INFO "Reading counter val= %d\n",x);
//end testing
	
	write_pmcntenclr(0x8000000Fu);

	pmcr = read_pmcr();
	pmcr &= ~0x0000003Fu;
	write_pmcr(pmcr);

	pmuserenr = read_pmuserenr();
	// PMUSERENR[bit 0] = EN (user-mode access to performance counters)
	pmuserenr &= ~0x00000001u;
	write_pmuserenr(pmuserenr); 
}


static int cpucounter_init(void) {
	printk(KERN_INFO "Performance counter Init..\n");
	cpucounter_enable(NULL);
	// Execute cpucounter_enable on all other cores, block until finished
	//smp_call_function(cpucounter_enable, NULL, 1);
	return 0;
}


static void cpucounter_exit(void) {
	printk(KERN_INFO "Performance counter exit..\n");
	cpucounter_disable(NULL);
	// Execute cpucounter_disable on all other cores, block until finished
	//smp_call_function(cpucounter_disable, NULL, 1);
}

module_init(cpucounter_init);
module_exit(cpucounter_exit);
