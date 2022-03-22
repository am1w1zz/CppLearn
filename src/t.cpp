#include<iostream>
#include<vector>
#include<algorithm>


void f (float x){
    std::cout<< x <<std::endl;

}
int main(int argc, const char** argv){
    // // std::vector<int> depth_buffer_mass ;
    // // depth_buffer_mass.push_back(1);
    // // depth_buffer_mass.push_back(2);
    // // depth_buffer_mass.push_back(3);
    // // depth_buffer_mass.push_back(4);
    // // depth_buffer_mass.push_back(5);
    // // auto min_depth = depth_buffer_mass[std::min_element(depth_buffer_mass.begin(), depth_buffer_mass.end())-depth_buffer_mass.begin()];
    // // std::cout<<min_depth<<std::endl;
    // // for (int i =0 ; i<10 ; ++i) {
    // //     f(i+0.5);
    // // }
    // std::cout << argc <<std::endl;
    // std::cout<< argv[0] <<std::endl;
    // std::cout<< argv[1] <<std::endl;
    // std::cout<< argv[2] <<std::endl;
    // std::cout<< argv[3] <<std::endl;
    // std::cout<< argv[4] <<std::endl;
    // float offsets[] = {0.25, 0.75, 0.25, 0.75};
    // for(int i=0;i<4;++i){
    //     std::cout<< offsets[i%2] << offsets[1-i%2] << std::endl;
    // }
    // int i=10;
    // for (auto ii : i) {

    // }
    std::vector v {1,2,3,4,5,6};
    for(int i = 0; i < v.size() ; ++i) {
            for(int j = 0; j < i; ++j) {
                std::cout << "j" << j << std::endl;    
            }
            for(int k = i; k < v.size() ; ++k) {
                std::cout << "k" << k << std::endl;
            }    
        }
}
