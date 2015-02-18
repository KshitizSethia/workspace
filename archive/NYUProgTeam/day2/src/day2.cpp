//============================================================================
// Name        : day2.cpp
// Author      : Kshitiz Sethia
// Version     :
// Copyright   : 
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <stdio.h>
#include <vector>
#include <set>
#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>

using namespace std;

struct web_page
{
	char* url[10000];
	int relevance;
};

bool web_page_sorter(web_page const& lhs, web_page const& rhs) {
	return lhs.relevance<rhs.relevance;
}

int main() {

	/*/ Problem A
	int numRooms;
	cin>>numRooms;

	int validRooms=0;
	for(int counter=0;counter<numRooms;counter++)
	{
		int occupied, capacity;
		scanf("%d %d", &occupied, &capacity);
		if(capacity-occupied>=2)
		{
			validRooms++;
		}
	}
	printf("%d", validRooms);
	*/

	/*/Problem B
	signed int numLevels;
	cin>>numLevels;

	//read x's abilities
	int numAbilities;
	cin>>numAbilities;
	std::vector<int> x(numAbilities, 0);
	int temp;
	for(int counter=0;counter<numAbilities;counter++)
	{
		cin>>temp;
		//x_array.insert(counter, x_array.begin(), x_array.end());
		x[counter] = temp;
	}
	std::sort(x.begin(), x.end());

	//read y's abilities
	cin>>numAbilities;
	std::vector<int> y(numAbilities, 0);
	for(int counter=0;counter<numAbilities;counter++)
	{
		cin>>y[counter];
	}
	std::sort(y.begin(), y.end());

	std::vector<int> unionHolder;
	std::set_union(x.begin(), x.end(),
					y.begin(), y.end(),
					std::back_inserter(unionHolder));
	if(unionHolder.size()==numLevels)
	{ cout<<"I become the guy."; }
	else
	{ cout<<"Oh, my keyboard!"; }
	*/

	/*/Problem C
	int numTestCases;
	cin>>numTestCases;

	for(int counter=0;counter<numTestCases;counter++)
	{
		//read in all pages
		std::vector<web_page> pages(10);
		int maxRelevance = -1;
		for(int ctr2=0;ctr2<10;ctr2++)
		{
			scanf("%s %d", pages[ctr2].url, &pages[ctr2].relevance);
			if(pages[ctr2].relevance>maxRelevance)
			{maxRelevance = pages[ctr2].relevance; }
		}

		//print out the most relevant
		cout<<"Case #"<<(counter+1)<<":"<<endl;
		for(int ctr2=0;ctr2<10;ctr2++)
		{
			if(pages[ctr2].relevance==maxRelevance)
			{printf("%s\n",pages[ctr2].url);}
		}
	}
	*/

	//Problem D

	return 0;
}
