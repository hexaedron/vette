#pragma once

class tim2Encoder
{
    public:
        void init(uint32_t);
        tim2Encoder() {};
        tim2Encoder(uint32_t);
        int32_t getDelta();
    
    private:
        uint16_t count;
        uint16_t last_count;
};

tim2Encoder::tim2Encoder(uint32_t remapMode = AFIO_PCFR1_TIM2_REMAP_NOREMAP)
{
    this->init(remapMode);
    this->count      = TIM2->CNT;
    this->last_count = TIM2->CNT;
}

void tim2Encoder::init(uint32_t remapMode = AFIO_PCFR1_TIM2_REMAP_NOREMAP)
{
    // Enable GPIOC, TIM2, and AFIO *very important!*
	RCC->APB2PCENR |= RCC_APB2Periph_AFIO | RCC_APB2Periph_GPIOC;
	RCC->APB1PCENR |= RCC_APB1Periph_TIM2;

	AFIO->PCFR1 |= remapMode; //set remap mode 

	// PC2 is T2CH1_, Input w/ Pullup/down
	GPIOC->CFGLR &= ~(0xf<<(4*2)); //clear old values
	GPIOC->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*2); //set new ones
	//1 = pull-up, 0 = pull-down
	GPIOC->OUTDR |= 1<<2;

	// PC5 is T2CH2_, Input w/ Pullup/down
	GPIOC->CFGLR &= ~(0xf<<(4*5)); //clear values
	GPIOC->CFGLR |= (GPIO_CNF_IN_PUPD)<<(4*5); //set new ones
	//1 = pull-up, 0 = pull-down
	GPIOC->OUTDR |= 1<<5;
	
	// Reset TIM2 to init all regs
	RCC->APB1PRSTR |= RCC_APB1Periph_TIM2;
	RCC->APB1PRSTR &= ~RCC_APB1Periph_TIM2;
	
	// set TIM2 clock prescaler If you want to reduce the resolution of the encoder
	//TIM2->PSC = 0x0000;

	// set a automatic reload if you want the counter to wrap earlier than 0xffff
	//TIM2->ATRLR = 0xffff;

	// SMCFGR: set encoder mode SMS=011b
	TIM2->SMCFGR |= TIM_EncoderMode_TI12;

	// initialize timer
	TIM2->SWEVGR |= TIM_UG;

	// set count to about mid-scale to avoid wrap-around
	TIM2->CNT = 0x8fff;

	// Enable TIM2
	TIM2->CTLR1 |= TIM_CEN;
}

int32_t tim2Encoder::getDelta(void)
{
    this->count = TIM2->CNT;
    int32_t tmp = (int32_t)this->count - this->last_count;
    this->last_count = this->count;
    return tmp;
}