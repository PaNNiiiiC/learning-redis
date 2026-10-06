#pragma once
#include<iostream>

inline void die(const char* s) {
    std::cerr<<s<<'\n';
    abort();
}