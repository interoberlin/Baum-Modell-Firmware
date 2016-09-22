

int main(void)
{
    init_ledstrips();
    setup_timers();

	while(true)
	{
	 //   asm("wfi"); // sleep: wait for interrupt
	    __WFI();
	    __SEV();
	    __WFE();
	}
}
