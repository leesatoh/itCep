#include<iostream>
#include<chrono>

using namespace std;


template <typename Func>
 void time_elapsed(Func func, int count)
{
    auto t0 = std::chrono::high_resolution_clock::now();
    for (int n = 0; n < count; n++) {
        func();
    }
    auto t1 = std::chrono::high_resolution_clock::now();

    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count() << " msec" << std::endl;
}


 template <typename Func>
 void foo(Func rmd) {
     rmd();
}

 template <typename Func>
 void foo1(Func& rmd) {
     rmd();
 }
 template <typename Func>
 void foo2(Func&& rmd) {
     rmd();
 }


auto main() -> int{
    time_elapsed([] {  foo([] {string s("abc"); });  }, 1000000);  // 起動直後の計測は捨てる（ゴミデータ）

    time_elapsed([] {  foo ([] {});  }, 10000000); 
//    time_elapsed([] {  foo1([] {});  }, 10000000);  // VCではなぜかとおる
    time_elapsed([] {  foo2([] {});  }, 10000000); 


    time_elapsed([] {  foo (std::move([] {}));  }, 10000000);  // Warning
//    time_elapsed([] {  foo1(std::move([] {}));  }, 10000000); // VCでは通る
    time_elapsed([] {  foo2(std::move([] {}));  }, 10000000);  // Warning


}

