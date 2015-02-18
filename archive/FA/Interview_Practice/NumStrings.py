# view question and math solution here: https://onedrive.live.com/edit.aspx/Documents/Interview%20Questions?cid=7e955140f7174ff1&id=documents?&wd=target%28Practice.one%7c77D26279-ED93-4598-BF51-7AB3F7EEA1D3%2fThere%20is%20a%20string%20of%20n%20characters.%20It%20can%20be%7c98DFCD31-0EDB-4245-8EF9-40DBEBDB0AA5%2f%29
# onenote:https://d.docs.live.net/7e955140f7174ff1/Documents/Interview%20Questions/Practice.one#There is a string of n characters. It can be&section-id={77D26279-ED93-4598-BF51-7AB3F7EEA1D3}&page-id={98DFCD31-0EDB-4245-8EF9-40DBEBDB0AA5}&end

def NumStrings(n, bUsed=False, cUsedInPrev=False, cSimultaneouslyUsed=False):
    countWithA=0
    countWithB=0
    countWithC=0
    if(n==1):
        countWithA=1
        if(not bUsed): countWithB=1
        if((not cUsedInPrev) or (not cSimultaneouslyUsed)): countWithC=1
    else:
        countWithA = NumStrings(n-1, bUsed, False, cSimultaneouslyUsed)
        if(not bUsed): countWithB = NumStrings(n-1, True, False, cSimultaneouslyUsed)
        if((not cUsedInPrev) or (not cSimultaneouslyUsed)):
            countWithC = NumStrings(n-1, bUsed, True, cSimultaneouslyUsed or cUsedInPrev)
            
    return countWithA+countWithB+countWithC

print NumStrings(4)
print NumStrings(3)