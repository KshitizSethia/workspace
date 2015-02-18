//============================================================================
// Name        : day1.cpp
// Author      : 
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <algorithm>
#include <string>
#include <array>

using namespace std;


int main() {

	/*/Problem A
	int problemSize=10;
	int *vals = new int[problemSize];
	for(int counter=0;counter<10;counter++)
	{
		cin>>vals[counter];
	}

	sort(vals, vals+problemSize);

	for(int counter=9;counter>6;counter--)
	{
		cout<<vals[counter]<<endl;
	}*/

	/*/Problem B
	int answer = 2;
	int *vals = new int[4];
	for(int ctr=0;ctr<4;ctr++)
	{
		string str;
		getline(std::cin,str);
		vals[ctr]=str.length()-3;
	}
	bool firstFound = false;
	for(int tryThis=0;tryThis<4;tryThis++)
	{
		if( ((vals[tryThis]==vals[(tryThis+1)%4]?true:2*vals[tryThis]<vals[(tryThis+1)%4])
			&& (vals[tryThis]==vals[(tryThis+2)%4]?true:2*vals[tryThis]<vals[(tryThis+2)%4])
		    && (vals[tryThis]==vals[(tryThis+3)%4]?true:2*vals[tryThis]<vals[(tryThis+3)%4]))
			||
			((vals[tryThis]==vals[(tryThis+1)%4]?true:vals[tryThis]>=2*vals[(tryThis+1)%4])
			&&(vals[tryThis]==vals[(tryThis+2)%4]?true:vals[tryThis]>=2*vals[(tryThis+2)%4])
			&&(vals[tryThis]==vals[(tryThis+3)%4]?true:vals[tryThis]>=2*vals[(tryThis+3)%4]))
		  )
		{
			if(firstFound)
			{
				cout<<"C"<<endl;
				return 0;
			}
			answer=tryThis;
			firstFound=true;
		}
	}

	char ans = answer + 65;
	cout<<ans<<endl;

	*/

	/*/Problem C
	int numGroups,busCapacity;
	cin>>numGroups;
	cin>>busCapacity;

	int groupSize;
	int busTrips = 0;
	int seatsLeftInBus = busCapacity;
	for(int groupNum=0;groupNum<numGroups;groupNum++)
	{
		cin>>groupSize;
		if(seatsLeftInBus-groupSize>=0)
		{
			seatsLeftInBus-=groupSize;
		}
		else
		{
			busTrips++;
			seatsLeftInBus = busCapacity;
			seatsLeftInBus-=groupSize;
		}
	}

	cout<<busTrips+1;
	*/

	//Problem D
	/*int

    int i, j, flag = 1;    // set flag to 1 to start first pass
    int temp;             // holding variable
    int numLength = num.length( );
    for(i = 1; (i <= numLength) && flag; i++)
	   {
			flag = 0;
			for (j=0; j < (numLength -1); j++)
		   {
				 if (num[j+1] > num[j])      // ascending order simply changes to <
				{
					  temp = num[j];             // swap elements
					  num[j] = num[j+1];
					  num[j+1] = temp;
					  flag = 1;               // indicates that a swap occurred.
				 }
			}
	   }

	*/
	return 0;
}
