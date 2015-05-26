/************************
Scheduler

Author : Rachita Hajela

*************************/


#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits.h>
using namespace std;

struct pte_struct
{
    unsigned int modified:1;
    unsigned int present : 1;
    unsigned int referenced : 1;
    unsigned int pagedout : 1;
    int index : 8;

    pte_struct()
    {
        modified = 0;
        present = 0;
        referenced = 0;
        pagedout = 0;
        index = 65;
    }

};

struct frame_table_struct
{
    int indexToPT=-1;
};


vector<pte_struct> pte(64) ;
vector<int> randoms;
vector<frame_table_struct> fTable;
vector<int> fQueue;   // frame queue
vector<unsigned int> ages; //counter for aging algorithm

int ofs = 0; //offset for the random file
int instCount =0 ;
int mapCount = 0;
int unmapCount =0;
int pageinCount=0;
int pageoutCount=0;
int zeroCount=0;
int nFrames;

int getRandomNumber(int nFrames)
{

    int r = randoms[ofs]%nFrames;
    ofs++;
    if(ofs == randoms.size()-1 )
    {
        ofs=0;
    }
    return r;

}
void setFrameNumber(int vp, int frameNum)
{
    pte[vp].index = frameNum;
}
int getFrameNumber(int vp)
{
    return pte[vp].index;
}
unsigned int getPageNumber(unsigned int frameNum)
{
    return fTable[frameNum].indexToPT;
}
void setPresent(int vp)
{
    pte[vp].present = 1;
}
void setModified(int vp)
{
    pte[vp].modified = 1;
}
void setReferenced(int vp)
{
    pte[vp].referenced = 1;
}
void setPagedOut(int vp)
{
    pte[vp].pagedout = 1;
}
void resetPresent(int vp)
{
    pte[vp].present = 0;
}
void resetModified(int vp)
{
    pte[vp].modified = 0;
}
void resetReferenced(int vp)
{
    pte[vp].referenced = 0;
}
void resetPagedOut(int vp)
{
    pte[vp].pagedout = 0;
}

int getPresent(int vp)
{
    return pte[vp].present;
}
int getModified(int vp)
{
    return pte[vp].modified;
}
unsigned int getReferenced(unsigned int vp)
{
    return pte[vp].referenced;
}
int getPagedOut(int vp)
{
    return pte[vp].pagedout;
}

class Algorithm
{
public:
    virtual unsigned int getReplacement() {}

    virtual void updateQueue(unsigned int vp) {}
}
;

class FIFO : public Algorithm
{

    unsigned int getReplacement()
    {
        unsigned int fNum;
        fNum = fQueue.front();
        fQueue.erase(fQueue.begin());
        fQueue.push_back(fNum);
        return fNum;
    }
    void updateQueue(unsigned int vp)
    {
    }
};

class Random : public Algorithm
{
    unsigned int getReplacement()
    {
        unsigned int fNum;
        fNum = getRandomNumber(nFrames);
        return fNum;
    }
    void updateQueue(unsigned int vp)
    {
    }
};

class SecondChance : public Algorithm
{
    unsigned int getReplacement()
    {
        bool found = false;
        unsigned int fNum;
        unsigned int pi;
        while(found!=true)
        {
            fNum = fQueue.front();
            pi=getPageNumber(fNum);
            if(getReferenced(pi)==0)
            {
                found=true;
                fQueue.erase(fQueue.begin());
                fQueue.push_back(fNum);

            }
            else
            {
                resetReferenced(pi);
                fQueue.erase(fQueue.begin());
                fQueue.push_back(fNum);
            }

        }
        return fNum;
    }
    void updateQueue(unsigned int vp)
    {
    }
};

class LRU : public Algorithm
{
    unsigned int getReplacement()
    {
        unsigned int fNum;
        fNum = fQueue.front();
        fQueue.erase(fQueue.begin());
        fQueue.push_back(fNum);
        
		printf("Indices: ");
		for (int frameIndex = 0; frameIndex < fQueue.size(); frameIndex++) {
			int pageNumber = fQueue[frameIndex];
			cout << pageNumber << " ";
		}
		cout << endl;
		return fNum;
    }
    void updateQueue(unsigned int vp)
    {
        int fNum;
        fNum=getFrameNumber(vp);
        for(int i=0; i<fQueue.size(); i++)
        {
            if(fQueue[i]==fNum)
            {
                fQueue.erase(fQueue.begin()+i);
                fQueue.push_back(fNum);
                break;
            }
        }
		
		printf("Indices: ");
		for (int frameIndex = 0; frameIndex < fQueue.size(); frameIndex++) {
			int pageNumber = fQueue[frameIndex];
			cout << pageNumber << " ";
		}
		cout << endl;

    }
};
class NRU : public Algorithm
{
    int counter=0;
    unsigned int getReplacement()
    {
        int fNum;
        vector<int> class0;
        vector<int> class1;
        vector<int> class2;
        vector<int> class3;

        for(int pi=0; pi<pte.size(); pi++)
        {
            if(getPresent(pi)==1)
            {
                if(getReferenced(pi)==0 && getModified(pi)==0)
                    class0.push_back(getFrameNumber(pi));
                else if(getReferenced(pi)==0 && getModified(pi)==1)
                    class1.push_back(getFrameNumber(pi));
                else if(getReferenced(pi)==1 && getModified(pi)==0)
                    class2.push_back(getFrameNumber(pi));
                else
                    class3.push_back(getFrameNumber(pi));

            }

        }
        if(class0.size()!=0)
        {
            int r = getRandomNumber(class0.size());
            fNum = class0[r];
            // cout<<" class : 0 "<<" size: "<<class0.size()<<" random num: "<<r<<" frame replaced: "<<fNum<<endl;
        }
        else if(class1.size()!=0)
        {
            int r = getRandomNumber(class1.size());
            fNum = class1[r];
            // cout<<" class : 1 "<<" size: "<<class1.size()<<" random num: "<<r<<" frame replaced: "<<fNum<<endl;
        }
        else if(class2.size()!=0)
        {
            int r = getRandomNumber(class2.size());
            fNum = class2[r];
            // cout<<" class : 2 "<<" size: "<<class2.size()<<" random num: "<<r<<" frame replaced: "<<fNum<<endl;
        }
        else
        {
            int r = getRandomNumber(class3.size());
            fNum = class3[r];
            //  cout<<" class : 3 "<<" size: "<<class3.size()<<" random num: "<<r<<" frame replaced: "<<fNum<<endl;
        }
        ++counter;
        if(counter==10)
        {
            counter=0;
            for(int pi=0; pi<pte.size(); pi++)
            {
                if(getPresent(pi)==1)
                {
                    resetReferenced(pi);
                }

            }
        }
        return fNum;
    }
    void updateQueue(unsigned int vp)
    {
    }

};
//based on physical frames
class Clock_P : public Algorithm
{
    int counter = 0;
    unsigned int getReplacement()
    {
        bool found = false;
        unsigned int fNum;
        unsigned int pi;
        while(found!=true)
        {
            fNum = fQueue[counter];
            pi=getPageNumber(fNum);
            if(getReferenced(pi)==0)
            {
                found=true;

            }
            else
            {
                resetReferenced(pi);
            }
            counter = (counter+1)%nFrames;

        }
        return fNum;
    }

    void updateQueue(unsigned int vp)
    {
    }

};
//based on virtual pages
class Clock_V : public Algorithm
{
    int counter=0;
    unsigned int getReplacement()
    {
        bool found = false;
        unsigned int fNum;
        unsigned int pi;
        while(found!=true)
        {
            if(getPresent(counter)==1)
            {

                if(getReferenced(counter)==0)
                {
                    found=true;
                    pi = counter;
                }
                else
                {
                    resetReferenced(counter);
                }
            }
            counter = (counter+1)% pte.size();

        }
        fNum = getFrameNumber(pi);
        return fNum;

    }

    void updateQueue(unsigned int vp)
    {
    }

};
// aging based on physical frames
class Aging_P : public Algorithm
{

    unsigned int getReplacement()
    {
        unsigned int minAge = UINT_MAX;
        unsigned int minFrame = -1;
        unsigned int minIndex = -1;

        //initializing
        if(ages.size()==0)
        {
            ages =  vector<unsigned int>(nFrames,0);
        }
        for (int i = 0; i < nFrames; i++)
        {

            ages[i] = (ages[i] >> 1) |
                      ((getReferenced(getPageNumber(i))) << 31);
            //cout<<" i: "<<i<<" ages "<<ages[i]<<" pageNum "<<getPageNumber(i)<<" refer :"<<getReferenced(getPageNumber(i))<<"\n";

            if (ages[i] < minAge)
            {
                minAge = ages[i];
                minFrame = i;
                minIndex = i;
            }

            resetReferenced(getPageNumber(i));

        }
        // clear  age counter
        ages[minIndex] = 0;

        return minFrame;
    }

    void updateQueue(unsigned int vp)
    {
    }

};
//aging based on virtual pages
class Aging_V : public Algorithm
{


    unsigned int getReplacement()
    {
        unsigned int minAge = UINT_MAX;
        unsigned int minIndex = -1;

        if(ages.size()==0)
        {
            ages = vector<unsigned int>(64,0);
        }

        for(int i=0; i<pte.size(); i++)
        {

            ages[i] = (ages[i] >> 1) | ((getReferenced(i)) << 31);

            if(getPresent(i)==1)
            {
                resetReferenced(i);
                if(ages[i]<minAge)
                {
                    minAge = ages[i];
                    minIndex = i;
                }
            }

        }
        ages[minIndex] = 0;
        return getFrameNumber(minIndex);

    }

    void updateQueue(unsigned int vp)
    {
    }

};
void mapFrame(Algorithm *algo, unsigned int rw, int vp,bool O)
{
    if(O)
    {
        printf("==> inst: %d %d\n", rw, vp);
    }
    unsigned int frameNum;

    // if not present in the memory
    if(getPresent(vp) == 0)
    {


        if(fTable.size()<nFrames)
        {
            frameNum = fTable.size();
            setFrameNumber(vp,frameNum);
            if(O)
            {
                printf("%d: ZERO     %4d\n", instCount , frameNum);
                printf("%d: MAP  %4d%4d\n", instCount, vp, frameNum);
            }
            zeroCount++;
            mapCount++;

            struct frame_table_struct newFrame;

            newFrame.indexToPT = vp; //reverse mapping
            fTable.push_back(newFrame);
            fQueue.push_back(frameNum);
        }
        else    //page fault
        {
            frameNum = algo->getReplacement();
            unsigned int previousPage = fTable[frameNum].indexToPT;
            //unmap
            if(O)
            {
                printf("%d: UNMAP%4d%4d\n", instCount, previousPage,
                       frameNum);
            }
            unmapCount++;
            resetPresent(previousPage);
            resetReferenced(previousPage);
            //check for modified bit
            if(getModified(previousPage)==1)
            {
                if(O)
                {
                    printf("%d: OUT  %4d%4d\n", instCount, previousPage,
                           frameNum);
                }
                pageoutCount++;
                resetModified(previousPage);
                setPagedOut(previousPage);
            }
            if(getPagedOut(vp)==1)  //in
            {
                if(O)
                {
                    printf("%d: IN   %4d%4d\n", instCount, vp, frameNum);
                }
                pageinCount++;
            }
            else  //zero
            {
                if(O)
                {
                    printf("%d: ZERO     %4d\n", instCount , frameNum);
                }
                zeroCount++;
            }

            //map
            if(O)
            {
                printf("%d: MAP  %4d%4d\n", instCount, vp, frameNum);
            }
            mapCount++;
            setFrameNumber(vp,frameNum);
            fTable[frameNum].indexToPT = vp;


        }
        setPresent(vp);
    }
    else
    {
        algo->updateQueue(vp);
    }


    setReferenced(vp);
    if(rw==1)
    {
        setModified(vp);
    }

	//print page table
	/*for (int pageIndex = 0; pageIndex < _NUM_PAGES; pageIndex++) {
		PageTableEntry& pte = virtualPageTable[pageIndex];
		if (pte.present) {
			cout << pageIndex << ":";
			pte.referenced ? cout << "R" : cout << "-";
			pte.modified ? cout << "M" : cout << "-";
			pte.pagedout ? cout << "S" : cout << "-";
		} else {
			pte.pagedout ? cout << "#" : cout << "*";
		}
		cout << " ";
	}
	cout << endl;*/
	
	//print frame table
	for (int frameIndex = 0; frameIndex < fTable.size(); frameIndex++) {
		int pageNumber = fTable[frameIndex].indexToPT;
		pageNumber == -1 ? cout << "* " : cout << pageNumber << " ";
	}
	cout << endl;
}
void printSummary(bool P,bool F, bool S)
{
    if(P)
    {
        for(int pi = 0; pi<pte.size(); pi++)
        {
            if(getPresent(pi)==1)
            {
                printf("%d:", pi);
                if (getReferenced(pi) == 1)
                {
                    printf("R");
                }
                else
                {
                    printf("-");
                }

                if (getModified(pi) == 1)
                {
                    printf("M");
                }
                else
                {
                    printf("-");
                }

                if (getPagedOut(pi) == 1)
                {
                    printf("S");
                }
                else
                {
                    printf("-");
                }
            }
            else
            {
                if (getPagedOut(pi) == 1)
                {
                    printf("# ");
                }
                else
                {
                    printf("* ");
                }
            }
        }
        printf("\n");
    }
    if(F)
    {
        for(int i=0; i<fTable.size(); i++)
        {
            if(getPageNumber(i)==-1)
                printf("* ");
            else
            {
                printf("%d ",getPageNumber(i));

            }

        }
        printf("\n");
    }
    if(S)
    {
        unsigned long long cost;

        cost = (long long)instCount+400 * (long long)(mapCount+unmapCount) +
               3000 * (long long)(pageinCount + pageoutCount)
               + 150 * (long long)zeroCount;
        printf("SUM %d U=%d M=%d I=%d O=%d Z=%d ===> %llu",
               instCount, unmapCount, mapCount, pageinCount,
               pageoutCount, zeroCount, cost);
    }
}
int main(int argc, char* argv[])
{
    ifstream input,r_file;
    string line;
    input.open(argv[argc - 2]);
    r_file.open(argv[argc - 1]);
    int rw;
    int vp;
    bool O,P,F,S;
    P=true;
    F=true;
    S=true;
    O=true;
    nFrames = 32;
    Algorithm* algo;
    //  algo = new FIFO();
    //  algo = new Random();
    // algo = new SecondChance();
    //algo = new LRU();
    //  algo = new NRU();
    // algo = new Clock_P();
    // algo = new Clock_V();
     algo = new Aging_P();
    //algo = new Aging_V();




    if(r_file.is_open())
    {
        getline(r_file,line);

        while(getline(r_file,line))
        {
            istringstream str(line);
            int num;
            str>>num;
            randoms.push_back(num);
        }
        r_file.close();
    }
    else
    {
        cout <<"can not open random file";
    }

    if(input.is_open())
    {
        while(getline(input,line))
        {
            if(line.substr(0,1).compare("#")==0)
            {
                continue;
            }
            istringstream str(line);
            str>>rw;
            str>>vp;

            mapFrame(algo,rw,vp,O);
            instCount++;

        }

    }
    printSummary(P,F,S);
    return 0;
}

