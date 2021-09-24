#include <iostream>

using namespace std;


int main()
{
    int in1;
    int in2;

    cin >> in1;
    cin >> in2;

    int product;
    product = in1 * in2;

    int rem;
    rem = product % 10;

    int otherNum;
    otherNum = product / 10;

    cout << "The carry will be " << otherNum << endl;
    cout << "The result(in terms of the vector) will be " << rem << endl;
    
    //so lets say we are multiplying the numbers, we obviously needs to put the carry over, how do we do this?
    //this code will split the numbers, the max single digit we can multiply is 9 x 9, so 81
    //we wont have to worry about going into the triple digits.


    return 0;
}