//Ararat Mesrobian
//CS302 Assignment 4
//9/20/2021
//Using threading
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
#include <vector>

using namespace std;

//function declarations
int convertToInt(string input, int i);
int returnCarry(int product);  //takes the product of multiplication, and returns the carry-over
int returnAnswer(int product);
void fillVector(string input, vector<short> & vectorToFill);
void longMultiplication(int threadNum, int currPlaceVal, int n);
void rearrangeAndPlaceZeroes(vector<vector<short>> resultVector,vector<vector<short>> & finalResultVector);
void alignVectors(vector<vector<short>> & finalResultVector);
void longAddition(vector<vector<short>> & finalResultVector, vector<short> & additionVector);


mutex t_lock;   //Will be used in conjunction with the threads to lock them
vector<short> topNums;  //multiplicand
vector<short> botNums;  //multiplier
vector<vector<short>> resultVector;   //Stores the result of the individual multiplication at the place value
vector<vector<short>> finalResultVector; //the sorted and correct vector
vector<short> fillResult;
vector<thread> threadPool;  //this will help us keep track of the threads and maintain what they are assigned to, as well as make it easier to clear the thread
vector<short> additionVector;   //this is the final final output 

unsigned int maxThreads = thread::hardware_concurrency();    //will be using this to check how many threads the pc running this program has. Need to use this throughout the program.
int main(int argc, char* arg[])
{
    string input; //going to read the numbers then convert

    cout << "Enter number1: ";
    cin >> input;
    fillVector(input, topNums);
    
    cout << "Enter Number2: ";
    cin >> input;
    fillVector(input, botNums);

    //to start the threads, we need to make a for loop that'll push the multiplicatoin function, with "i" being the bottom number in question
    int threadNum;
    int n = 0;
    for(int i = botNums.size() - 1; i >= 0; i--)
    {
        int botNum = botNums[i];
        threadPool.push_back(thread(longMultiplication, botNum, i, n));
        n++;
        //cout << "This is the bottom number in the loop: " << botNums[i] << endl;

        if(threadPool.size() % maxThreads == 0)
        {
            for(int j = 0; j < threadPool.size(); j++)
            {
                threadPool[j].join();
            }
            threadPool.clear(); //this will join all the threads together, and then clear the threads
        }
    }

    for(int i = 0; i < threadPool.size(); i++)
    {
        threadPool[i].join();   //This will make sure that all of the threads are done accessing the function and everything will be put into the vector
    }
    threadPool.clear();

    cout << "This is the result vector: " << endl;
    for(int i = 0; i < resultVector.size(); i++)
    {
        for(int j = 0; j < resultVector[i].size(); j++)
        {
            cout << resultVector[i][j];
        }
        cout << endl;
    }

    //longAddition(resultVector, additionVector);
    /*
    int zeroCount = 0;
    for(int i = 0; i < additionVector.size(); i++)
    {
        if(additionVector[i] != 0)
        {
            zeroCount++;
        }
    }

    cout << "The product is: ";
    if(zeroCount > 0)
    {
        for(int i = 0; i < additionVector.size(); i++)
        {
            cout << additionVector[i];
        }
    }
    else
    {
        cout << 0 << endl;
    }*/

    //longMultiplication(threadNum, botNums, topNums, fillResult, resultVector);
  
    /*rearrangeAndPlaceZeroes(resultVector, finalResultVector);

    alignVectors(finalResultVector);

    longAddition(finalResultVector, additionVector);

    int zeroCount = 0;
    for(int i = 0; i < additionVector.size(); i++)
    {
        if(additionVector[i] != 0)
        {
            zeroCount++;
        }
    }
    
    cout << "The product is: ";
    if(zeroCount > 0)
    {
        for(int i = 0; i < additionVector.size(); i++)
        {
            cout << additionVector[i];
        }
    }
    else
    {
        cout << 0 << endl;
    }*/
    

return 0;
}

//this will take in the input and convert it into an int
int convertToInt(string input, int i)
{
    int size = input.size();
    char temp2;
    int temp;

    temp2 = input[i];
    temp = temp2;
    temp -= 48;

    return temp;
}

//returns the int that we will be carrying over to ADD to the next place value result
int returnCarry(int product)
{
    int carry;
    carry = product / 10;
    return carry;
}

//returns the palce value's current answer that will be put into the 2d vector
int returnAnswer(int product)
{
    int answer;
    answer = product % 10;
    return answer;
}

//fills the given vector with the characters from the input
void fillVector(string input, vector<short> & vectorToFill)
{
    int size;
    size = input.size();
    for(int i = 0; i < size; i++)
    {
        vectorToFill.push_back(convertToInt(input, i));
    }
}

//takes the filled vectors and multiplies them using long division into a 2d vector
//This will be threaded
//Because it is threaded, when we write we need to make sure it is being locked
//when we are doing the actual multiplication we do not need to worry
void longMultiplication(int threadNum, int currPlaceVal, int n)
{
    int carry = 0;
    int answer = 0;

    int currentBotNum = 0;
    t_lock.lock();
    currentBotNum = threadNum;
    t_lock.unlock();

    t_lock.lock();
    int topSize = topNums.size();
    t_lock.unlock();

    //threadNUm is the currect place value of the multiplier, we need to multiply this throughout the entire loop
    t_lock.lock();
    for(int j = topNums.size() - 1; j >= 0; j--)
    {
        int currentTopNum = 0; 
        currentTopNum = topNums[j];
        
        int product = currentBotNum * currentTopNum;
        answer = product;
        if(carry > 0)
        {
            answer = answer + carry;
        }
        else
        {
            carry = 0;
        }

        if(product > 9)
        {
            carry = returnCarry(answer);
            answer = returnAnswer(answer);
        }
        if(j == 0)
        {
            //we're checking to see if we need to store the carry, if we are on the last operation
            if(carry == 0)
            {
                fillResult.insert(fillResult.begin(), answer);
            }
            else
            {
                //store the answer first and then the carry, we will rearrange the vector.
                fillResult.insert(fillResult.begin(), answer);
                if(currPlaceVal == 0)
                {
                    fillResult.insert(fillResult.begin(), carry);
                }
            }
        }
        else
        {
            fillResult.insert(fillResult.begin(), answer);
        }
        if(n > 0)
        {
            while(n != 0)
            {
                fillResult.push_back(0);
                n--;
            }
        }

        //for(int i = 0; i < fillResult.size(); i++)
        //{
            //cout << fillResult[i] << " ";
        //}
        //cout << endl;
        //cout << "Next..." << endl;
        //cout << "This is the product: " << answer << endl;
        //cout << "This is the carry: " << carry << endl;
    }
    t_lock.unlock();
    t_lock.lock();
    cout << "Now we are at the result Vector: " << endl;
    t_lock.unlock();
    t_lock.lock();
    resultVector.push_back(fillResult);
    t_lock.unlock();
    fillResult.clear();
    alignVectors(resultVector);
    carry = 0;
}

//I was getting the incorrect output so i created this to rearrange the numbers so that they werent backwards
void rearrangeAndPlaceZeroes(vector<vector<short>> resultVector,vector<vector<short>> & finalResultVector)
{
    vector<short> tempVec;
    for(int i = 0; i < resultVector.size(); i++)
    {
        for(int j = resultVector[i].size() - 1; j >= 0; j--)
        {
            tempVec.push_back(resultVector[i][j]);
        }
        finalResultVector.push_back(tempVec);
        tempVec.clear();
    }
    
    for(int i = 0; i < finalResultVector.size(); i++)
    {
        if(i < finalResultVector.size())
        {
            for(int k = i; k > 0; k--)
            {
                finalResultVector[i].push_back(0);
            }
        }
    }
}

//this will add the leading zeroes to place align everything so that the result is correct
void alignVectors(vector<vector<short>> & finalResultVector)
{
    int finalRow;   //grab the final row which will have the highest offset(since we put zeroes into the back)
    finalRow = finalResultVector.size() - 1;
    //access the last row with finalRow
    int finalRowSize = 0;
    finalRowSize = finalResultVector[finalRow].size();
    //cout << "This is the size of the final row in finalRowVector: " << finalRowSize << endl; 
    for(int i = 0; i < finalResultVector.size(); i++)
    {
        if(finalResultVector[i].size() != finalRowSize)
        {
            while(finalResultVector[i].size() != finalRowSize)
            {
                finalResultVector[i].insert(finalResultVector[i].begin(), 0);
            }
        }
    }
}

//this will not have to be threaded
//this will do long addition on the 2d vector, place the results into a 1d vector that will be outputted as the result of everything
void longAddition(vector<vector<short>> & finalResultVector, vector<short> & additionVector)
{
    int carry = 0;
    int sum = 0;
    int placeVal = 0;
    for(int col = finalResultVector[1].size() - 1; col >= 0; col--)
    {
        int sum = 0;
        for(int row = 0; row < finalResultVector.size(); row++)
        {
            sum += finalResultVector[row][col];
        }
        sum += carry;

        if(sum > 9)
        {
            carry = returnCarry(sum);
            returnAnswer(placeVal);
        }
        else
        {
            carry = 0;
            placeVal = sum;
        }
      
       if(col == 0)
       {
           if(carry > 0)
           {
               additionVector.insert(additionVector.begin(), placeVal);
               additionVector.insert(additionVector.begin(), placeVal);
           }
           else
           {
               additionVector.insert(additionVector.begin(), placeVal);
           }
       }
       else
       {
           additionVector.insert(additionVector.begin(), placeVal);
       }
    }

}