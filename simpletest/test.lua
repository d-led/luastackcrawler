assert(trystack(1)=='1 ')
assert(trystack(nil)=='nil ')
assert(trystack("1")=='1 ')
assert(trystack("1","2")=='1 2 ')
assert(trystack({1,2})=='{1->1 2->2} ')
assert(trystack(true,false,3)=='true false 3 ')

t={}
t[1]="1"
t[2]="2"
assert(trystack("1",t)=='1 {1->1 2->2} ')

t["1"]=1
t["2"]={3,4,5}
assert(trystack(nil,t)=='nil {1->1 2->2 1->1 2->{1->3 2->4 3->5}} ')

a={}
a[a]="x";
a[t]=t;
a[{1,2}]={3}

local res=trystack(a)
local expected=[[{table*->{1->1 2->2 1->1 2->{1->3 2->4 3->5}} table*->x {1->1 2->2}->{1->3}} ]]
--print(expected)
assert(res==expected)

res=trystack2(1,2)
expected='1 2 '
assert(res==expected)

res=tryextension("begin",a,"end")
expected=[[begin {table*->{1->1 2->2 1->1 2->{1->3 2->4 3->5}} table*->x {1->1 2->2}->{1->3}} ]]
assert(res==expected)

res=trystackextension("begin",a,"end") --should produce the same result
assert(res==expected)