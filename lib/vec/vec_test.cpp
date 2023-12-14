#include "Vec.hpp"

void func1(vec3f& v){
    v[1] = 2.0f;
}

int main(){
    vec3f v = vec3f(0,0,0);
    func1(v);
    std::cout << v << "\n";
}