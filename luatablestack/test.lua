trystack(1)
trystack(nil)
trystack("1")
trystack("1","2")
trystack({1,2})
trystack(true,false,3)

t={}
t[1]="1"
t[2]="2"
trystack("1",t)

t["1"]=1
t["2"]={3,4,5}
trystack(nil,t)

a={}
--a[a]="x"; --gotta make a check
a[t]=t;

trystack(a)