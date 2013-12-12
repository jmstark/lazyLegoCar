#pragma once
#ifndef OBSERVER
#define OBSERVER

#include "communication.h"

namespace obs{

	class Observer{
		public:
			Observer();
			~Observer();
			
		private:
			com arduino, mobile;
	};
};
#endif