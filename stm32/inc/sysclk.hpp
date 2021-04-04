/*
	>> Copyright Luke A.C.A. Rieff & Rien Dumore - Project-D ROC Nijmegen
*/

#include "inc.hpp"

#pragma once

#define __SYSCLOCK_ETHERNET 			(216U)
#define __SYSCLOCK_HCLK_MHZ				(216U)
#define __SYSCLOCK_SYSTEM				(216U)
#define __SYSCLOCK_FCLK					(216U)
#define __SYSCLOCK_APB1_PERIPHERALS		(54U)
#define __SYSCLOCK_APB1_TIMERS			(108U)
#define __SYSCLOCK_APB2_PERIPHERALS		(108U)
#define __SYSCLOCK_APB2_TIMERS			(216U)

class SysClk {
public:
	static void Init (void);
};
