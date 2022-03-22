#include<iostream>
#include<random>
#include<cmath>
#define PI  3.141592653589732

int main() {
    std::default_random_engine e;
	std::uniform_real_distribution<float> u(0, 1);
	for (int i = 0; i < 15; ++i)
	{
		float xi1 = u(e);
		float xi2 = u(e);
		std::cout<<xi1 * sin(2 * PI * xi2) <<" "<< xi1 * cos(2 * PI * xi2)<< " "<< xi1 * xi1<<std::endl;
	}
}