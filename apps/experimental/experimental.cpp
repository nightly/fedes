#include <iostream>
#include <utility>
#include <thread>
#include <chrono>
#include <format>
#include <vector>

#include <thread_pool.hpp>

#include "fedes/common/log.h"
#include "fedes/model/examples.h"


int main() {

	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 4);
}


/*
	fedes::Model source, target;
	fedes::SetExampleModels(source, target, 4);
	std::cout << "x";
*/