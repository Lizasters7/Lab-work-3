#include <chrono>
#include <iostream>
#include <latch>
#include <syncstream>
#include <thread>
#include <vector>

constexpr int A = 9, B = 5, C = 7, D = 8, E = 9, F = 5, G = 5, H = 9, I = 6, J = 7;
 
void f(char x, int i) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::osyncstream(std::cout) << "From  set " << x << " the action done is " << i << ".\n";
}

std::latch La(A), Lb(B), Lc(C), Ld(D), Le(E), Lg(G), Lh(H), Li(I), Lj(J);
 
void act(char x, int i, std::latch& done) {
    f(x, i);
    done.count_down();
}

void wait_i() { Lg.wait(); Lc.wait(); }            
void wait_h() { Le.wait(); Lb.wait(); }            
void wait_j() { La.wait(); Ld.wait(); }            
void wait_f() { Lj.wait(); Lh.wait(); Li.wait(); } 
 
void thread1() {
    act('a', 1, La);  
    act('a', 6, La);  
    act('b', 2, Lb);  
    act('c', 2, Lc);  
    act('c', 7, Lc);  
    act('d', 5, Ld);  
    act('e', 2, Le);  
    act('e', 7, Le);  
    act('g', 3, Lg);  
    wait_h(); act('h', 3, Lh);  
    act('h', 8, Lh);  
    wait_i(); act('i', 4, Li);  
    wait_j(); act('j', 3, Lj);  
    wait_f(); f('f', 1);  
}
 
void thread2() {
    act('a', 2, La);  
    act('a', 7, La);  
    act('b', 3, Lb);  
    act('c', 3, Lc);  
    act('d', 1, Ld);  
    act('d', 6, Ld);  
    act('e', 3, Le);  
    act('e', 8, Le);  
    act('g', 4, Lg);  
    wait_h(); act('h', 4, Lh); 
    act('h', 9, Lh);  
    wait_i(); act('i', 5, Li);  
    wait_j(); act('j', 4, Lj);  
    wait_f(); f('f', 2);  
}
 
void thread3() {
    act('a', 3, La); 
    act('a', 8, La);  
    act('b', 4, Lb);  
    act('c', 4, Lc);  
    act('d', 2, Ld);  
    act('d', 7, Ld);  
    act('e', 4, Le);  
    act('e', 9, Le);  
    act('g', 5, Lg);  
    wait_h(); act('h', 5, Lh);  
    wait_i(); act('i', 1, Li);  
    act('i', 6, Li);  
    wait_j(); act('j', 5, Lj);  
    wait_f(); f('f', 3);  
}
 
void thread4() {
    act('a', 4, La);  
    act('a', 9, La); 
    act('b', 5, Lb); 
    act('c', 5, Lc); 
    act('d', 3, Ld);  
    act('d', 8, Ld);  
    act('e', 5, Le);  
    act('g', 1, Lg);  
    wait_h(); act('h', 1, Lh);  
    act('h', 6, Lh); 
    wait_i(); act('i', 2, Li);  
    wait_j(); act('j', 1, Lj);  
    act('j', 6, Lj); 
    wait_f(); f('f', 4);  
}
 
void thread5() {
    act('a', 5, La);  
    act('b', 1, Lb);  
    act('c', 1, Lc);  
    act('c', 6, Lc);  
    act('d', 4, Ld);  
    act('e', 1, Le);  
    act('e', 6, Le);  
    act('g', 2, Lg);  
    wait_h(); act('h', 2, Lh);  
    act('h', 7, Lh);  
    wait_i(); act('i', 3, Li);  
    wait_j(); act('j', 2, Lj);  
    act('j', 7, Lj);  
    wait_f(); f('f', 5); 
}
 
int main() {
    std::osyncstream(std::cout) << "Calculation started.\n";
    auto start = std::chrono::steady_clock::now();
 
    std::vector<std::thread> threads;
    threads.emplace_back(thread1);
    threads.emplace_back(thread2);
    threads.emplace_back(thread3);
    threads.emplace_back(thread4);
    threads.emplace_back(thread5);
    for (auto& t : threads) t.join();
 
    std::chrono::duration<double> elapsed = std::chrono::steady_clock::now() - start;
    std::osyncstream(std::cout)
        << "Calculation completed. Duration: " << elapsed.count() << " seconds.\n";
}