
// Copyright (c) 2023 Vitaly Dikov
// 
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE or copy at https://www.boost.org/LICENSE_1_0.txt)

#include<boost/circular_buffer.hpp>
#include<iostream>
#include<string>

void PrintBuffer(const boost::circular_buffer<int>& cb, const std::string& s)
{
    std::cout << "PrintBuffer: " << s << std::endl;
    for (int i = 0; i < cb.size(); ++i) {
        std::cout << "cb[" << i << "] == " << cb[i] << std::endl;
    }
}

int main()
{
    std::cout << "boost::circular_buffer" << std::endl;
    // Create a circular buffer with a capacity for 3 integers.
    boost::circular_buffer<int> cb(3);

    // Insert three elements into the buffer.
    cb.push_back(1);
    cb.push_back(2);
    cb.push_back(3);

    int a{ cb[0] };  // a == 1
    int b{ cb[1] };  // b == 2
    int c{ cb[2] };  // c == 3
    PrintBuffer(cb, "Case 1");

    // The buffer is full now, so pushing subsequent
    // elements will overwrite the front-most elements.

    cb.push_back(4);  // Overwrite 1 with 4.
    cb.push_back(5);  // Overwrite 2 with 5.

    // The buffer now contains 3, 4 and 5.
    int d{ cb[0] };  // d == 3
    int e{ cb[1] };  // e == 4
    int f{ cb[2] };  // f == 5
    PrintBuffer(cb, "Case 2");

    // Elements can be popped from either the front or the back.
    cb.pop_back();  // 5 is removed.
    cb.pop_front(); // 3 is removed.

    // Leaving only one element with value = 4.
    int g{ cb[0] };  // g == 4
    PrintBuffer(cb, "Case 3");

    return 0;
}
