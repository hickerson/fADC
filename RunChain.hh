#ifndef __class_RunChain_hh__
#define __class_RunChain_hh__

#include <string>
#include <iostream>
#include "Run.hh"

using namespace std;

/**
 * RunChain
 *
 * Author: Kevin Peter Hickerson
 * Created: Sun Nov  6 10:06:04 PST 2011
 */
class RunChain : Run
{
public:
	RunChain();

	RunChain(const RunChain & copy)
	{
	}

	~RunChain();
	
	RunChain & operator = (const RunChain & other)
    {
        if (this != &other) // protect against invalid self-assignment
        {
            // 1: allocate new memory and copy the data
            // 2: deallocate old memory
            // 3: assign the new memory to the object
        }
        return *this;
    }
};

#endif
