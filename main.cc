#include "set.hpp"
#include <cassert>

int main() {

    // Some miscellaneous code to get you started on testing your sets.
    // The following should work:

    // std_simple_set<int> R;
    // R += 3;
    // cout << "3 is " << (R.contains(3) ? "" : "not ") << "in R\n";
    // cout << "5 is " << (R.contains(5) ? "" : "not ") << "in R\n";

    // simple_set<double>* S = new std_simple_set<double>();
    // *S += 3.14;
    // cout << "pi is " << (S->contains(3.14) ? "" : "not ") << "in S\n";
    // cout << "e is " << (S->contains(2.718) ? "" : "not ") << "in S\n";

    // std_simple_set<string> U;
    // U += "hello";
    // cout << "\"hello\" is " << (U.contains("hello") ? "" : "not ") << "in U\n";
    // cout << "\"foo\" is " << (U.contains("foo") ? "" : "not ") << "in U\n";

    // range<string> r1("a", true, "f", true);
    // cout << "\"b\" is " << (r1.contains("b") ? "" : "not ") << "in r1\n";
    // cout << "\"aaa\" is " << (r1.contains("aaa") ? "" : "not ") << "in r1\n";
    // cout << "\"faa\" is " << (r1.contains("faa") ? "" : "not ") << "in r1\n";

    // range<const char*, lexico_less> r2("a", true, "f", true);
    // cout << "\"b\" is " << (r2.contains("b") ? "" : "not ") << "in r2\n";
    // cout << "\"aaa\" is " << (r2.contains("aaa") ? "" : "not ") << "in r2\n";
    // cout << "\"faa\" is " << (r2.contains("faa") ? "" : "not ") << "in r2\n";

    // // The following will not work correctly yet:

    // carray_range_set<int> A(0, 100);
    // A += range<int>(4, true, 9, false);
    // A += range<int>(11, true, 15, true);
    // A -= range<int>(15, true, 15, true);
    // cout << "4 is " << (A.contains(4) ? "" : "not ") << "in A\n";
    // cout << "5 is " << (A.contains(5) ? "" : "not ") << "in A\n";
    // cout << "9 is " << (A.contains(9) ? "" : "not ") << "in A\n";
    // cout << "15 is " << (A.contains(15) ? "" : "not ") << "in A\n";

    // range_set<int>* X = new std_range_set<int>();
    // *X += range<int>(5, true, 8, false);
    // if (X->contains(4)) cout << "4 is in X\n";
    // if (X->contains(5)) cout << "5 is in X\n";      // should print
    // if (X->contains(6)) cout << "6 is in X\n";      // should print
    // if (X->contains(7)) cout << "7 is in X\n";      // should print
    // if (X->contains(8)) cout << "8 is in X\n";
    // if (X->contains(9)) cout << "9 is in X\n";
    // *X -= range<int>(6, true, 10, false);
    // if (X->contains(4)) cout << "4 is now in X\n";
    // if (X->contains(5)) cout << "5 is now in X\n";      // should print
    // if (X->contains(6)) cout << "6 is now in X\n";
    // if (X->contains(7)) cout << "7 is now in X\n";
    // if (X->contains(8)) cout << "8 is now in X\n";
    // if (X->contains(9)) cout << "9 is now in X\n";

   simple_set<weekday>* V = new carray_simple_set<weekday>(mon, (weekday)5);

    // hashed_simple_set<int, cast_to_int<int>> H(100);
    // H+= 4; H+= 10; H+= 98;
    // cout << "10 is " << (H.contains(10) ? "" : "not ") << "in H\n";
    // cout << "19 is " << (H.contains(19) ? "" : "not ") << "in H\n";

    // hashed_simple_set<double, cast_to_int<double>> H1(20);
    // H1+= 3.14; H1+= 1.9;
    // cout << "pi is " << (H1.contains(3.14) ? "" : "not ") << "in H1\n";

    // bin_search_simple_set<double> J(100);
    //         J.showSet();

    // J += 1;
    //     J.showSet();
    // printf("--------------\n");
    // J -= 1;
    //         J.showSet();
    // printf("--------------\n");

    // J += 2;
    //     J.showSet();
    //         printf("--------------\n");


    // J += 3;
    //     J.showSet();
    //         printf("--------------\n");

    // J += 5;
    //     J.showSet();
    //         printf("--------------\n");


    // J += 4;
    //     J.showSet();
    //         printf("--------------\n");

    // J -= 1;
    //         J.showSet();
    //             printf("--------------\n");

    // J -= 3;
    //         J.showSet();
    //             printf("--------------\n");

    // cout << "4 is " << (J.contains(4) ? "" : "not ") << "in J\n";


   // bin_range_set<int> J(100);
  // s += range<double>(2.0, true, 4.0, true);
  // s += range<double>(6.0, true, 8.0, true);
  // s += range<double>(10.0, true, 11.0, true);
  // assert(!s.contains(5.0));
  // assert(s.contains(3.7));
  // s += range<double>(1.0, false, 10.7, false);
  // assert(s.contains(5.0));



}