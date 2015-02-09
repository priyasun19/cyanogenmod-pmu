#ifndef __ARMPMU_HEADER
#define __ARMPMU_HEADER
#define ARMV7_COUNTERS 4
enum armv7_counters
{
	ARMV7_COUNTER0,
	ARMV7_COUNTER1,
	ARMV7_COUNTER2,
	ARMV7_COUNTER3
};

enum armv7_pmu_events
{
	ARMV7_IFETCH_MISS = 0X01,
	ARMV7_ITLB_MISS = 0X02,
	ARMV7_DCACHE_REFILL = 0X03,
	ARMV7_DCACHE_ACCESS = 0X04,
	ARMV7_DTLB_REFILL = 0X05,
	ARMV7_DREAD = 0X06,
	ARMV7_DWRITE = 0X07,
	ARMV7_INSTR_EXECUTED = 0x08,
	ARMv7_BRANCH_MIS_PRED = 0x10,
	ARMV7_L2_CACHE_MISS = 0X44,
	ARMV7_L1_DATA_CACHE_MISS = 0X49,
	ARMV7_L1_INSTR_CACHE_MISS = 0X4A
}; 	
// PMCR = Performance Monitors Control Register
inline unsigned int read_pmcr(void) {
	unsigned int pmcr=0;
	//asm ("MRC p15, 0, %0, c9, c12, 0;"::"=r"(pmcr));
	asm volatile("mrc p15, 0, %0, c9, c12, 0" : "=r"(pmcr) );
	return pmcr;
}

inline void write_pmcr(unsigned int pmcr) {
	asm ("MCR p15, 0, %0, c9, c12, 0;":: "r" (pmcr));
}

// MUSERENR = Performance Monitors User Enable Register
inline unsigned int read_pmuserenr(void) {
	unsigned int pmuserenr;
	asm volatile("MRC p15, 0, %0, c9, c14, 0;": "=r" (pmuserenr));
	return pmuserenr;
}

inline void write_pmuserenr(unsigned int pmuserenr) {
	asm ("MCR p15, 0, %0, c9, c14, 0;":: "r" (pmuserenr));
}

// PMCCNTR = Performance Monitors Cycle Count Register
inline unsigned int read_pmccntr(void) {
	unsigned int pmccntr;
	asm volatile("MRC p15, 0, %0, c9, c13, 0;": "=r" (pmccntr));
	//printk(KERN_INFO "PMCCNTR\n");
	return pmccntr;
}

inline void write_pmccntr(unsigned int pmccntr) {
	asm ("MCR p15, 0, %0, c9, c13, 0;":: "r" (pmccntr));
}

/*
// PMCEID0 = Performance Monitors Common Event ID Register 0
inline int read_pmceid0(void) {
	int pmceid0;
	asm volatile("MRC p15, 0, %0, c9, c12, 6;": "=r" (pmceid0));
	return pmceid0;
}

// PMCEID1 = Performance Monitors Common Event ID Register 1
inline int read_pmceid1(void) {
	int pmceid1;
	asm ("MRC p15, 0, %0, c9, c12, 7;": "=r" (pmceid1));
	return pmceid1;
}
*/

// PMCNTENCLR = Performance Monitors Count Enable Clear Register
inline unsigned int read_pmcntenclr(void) {
	int pmcntenclr;
	asm volatile("MRC p15, 0, %0, c9, c12, 2;": "=r" (pmcntenclr));
	return pmcntenclr;
}

inline void write_pmcntenclr(unsigned int pmcntenclr) {
	asm ("MCR p15, 0, %0, c9, c12, 2;":: "r" (pmcntenclr));
}

// PMCNTENSET = Performance Monitors Count Enable Set Register
inline unsigned int read_pmcntenset(void) {
	unsigned int pmcntenset;
	asm volatile("MRC p15, 0, %0, c9, c12, 1;": "=r" (pmcntenset));
	return pmcntenset;
}

inline void write_pmcntenset(unsigned int pmcntenset) {
	asm ("MCR p15, 0, %0, c9, c12, 1;":: "r" (pmcntenset));
}

// PMINTENCLR = Performance Monitors Interrupt Enable Clear Register
inline unsigned int read_pmintenclr(void) {
	unsigned int pmintenclr;
	asm volatile("MRC p15, 0, %0, c9, c14, 2;": "=r" (pmintenclr));
	return pmintenclr;
}

inline void write_pmintenclr(unsigned int pmintenclr) {
	asm ("MCR p15, 0, %0, c9, c14, 2;":: "r" (pmintenclr));
}

// PMINTENSET = Performance Monitors Interrupt Enable Set Register
inline unsigned int read_pmintenset(void) {
	unsigned int pmintenset;
	asm volatile("MRC p15, 0, %0, c9, c14, 1;": "=r" (pmintenset) );
	return pmintenset;
}

inline void write_pmintenset(unsigned int pmintenset) {
	asm ("MCR p15, 0, %0, c9, c14, 1;":: "r" (pmintenset));
}

//perf counter selection PMNXSEL - 0,1,2,3
inline void write_pmnxsel(int counter)
{
	counter &= 0x1F;
	asm("MCR p15,0,%0,c9,c12,5;":: "r"(counter));
}

/*
inline int read_pmnxsel(void)
{
	int counter;
	asm volatile("MRC p15,0,%0,c9,c12,5;" : "=r"(counter) );
	return counter;
}
*/

//event selection register - EVTSEL
inline void write_evtsel(int event)
{
	int val;
	val=event & 0x7F;
	asm("MCR p15,0,%0,c9,c13,1;":: "r"(val));
}

/*
inline int read_evtsel(void)
{
	int event;
	asm volatile("MRC p15,0,%0,c9,c13,1;": "=r"(event));
	return event;
}
*/

//Perf monitor count registers - PMCNT
//r/w selected perf counter. 
/*
inline void write_pmcnt(int value)
{
	asm("MCR p15,0,%0,c9,c13,2;":: "r"(value));
}
*/
inline unsigned int read_pmcnt(enum armv7_counters counter)
{
	unsigned int value=0;
	write_pmnxsel(counter); //selecting counter
	asm volatile("MRC p15,0,%0,c9,c13,2;": "=r"(value));
	return value;
}

inline unsigned int getCPUCycleCount(void)
{
	return read_pmccntr();
} 

/*
//for now assuming counter 0 for instr count event
inline int getInstructionCount(void)
{
	return read_pmcnt(0);
}
*/

inline void setCounterEvent(enum armv7_counters counter,enum armv7_pmu_events event)
{
	write_pmnxsel(counter);
	write_evtsel(event);
}

inline void enableAndResetCounters(void)
{
	unsigned int pmcr;
	pmcr = read_pmcr();
	pmcr &= ~0x0000003Fu; //mask
	pmcr |= 0x00000017u;//E,P,C bits - 1
	write_pmcr(pmcr);
}
#endif
